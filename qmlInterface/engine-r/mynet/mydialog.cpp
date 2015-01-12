#include "mydialog.h"

QString Alfavit("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@._");
QString LatinUp("QWERTYUIOPASDFGHJKLZXCVBNM");
QString LatinDown("qwertyuiopasdfghjklzxcvbnm");
QString RusUp=("ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁ");
QString RusDown=("йцукенгшщзхъфывапролджэячсмитьбюё");
QString Number=("1234567890");
QString Symbol=("~!@#$%^&*()_-+=|\\[]{};:,.<>?/");

QString MyDialog::getParam(QString param, qint32 n)
{
    qint32 ifrom=0;
    qint32 ito=0;
    n--;
    while(n>0)
    {
        ifrom=param.indexOf(",",ifrom);
        if(ifrom==-1) return "";
        ifrom++;
        n--;
    }
    ito=param.indexOf(",",ifrom);
    if(ito<0) ito=param.size()+1;

    return param.mid(ifrom,ito-ifrom);
}

// 0 - OK
// 6 - fail
qint16 MyDialog::checkPassword(QString password)
{
    if(password.length()<8) return 1;
    bool bUp=false;
    bool bDown=false;
    bool bNumber=false;
    bool bSymbol=false;
    for(qint32 i=0;i<password.length();i++)
    {
        if(LatinUp.indexOf(password.at(i))>=0) bUp=true; else
            if(LatinDown.indexOf(password.at(i))>=0) bDown=true; else
                if(RusUp.indexOf(password.at(i))>=0) bUp=true; else
                    if(RusDown.indexOf(password.at(i))>=0) bDown=true; else
                        if(Number.indexOf(password.at(i))>=0) bNumber=true; else
                            if(Symbol.indexOf(password.at(i))>=0) bSymbol=true;
    }

    return (bUp & bDown & bNumber & bSymbol)?0:1;
}

// 0 - OK
// 3 - fail
qint16 MyDialog::checkEMail(QString email)
{
    qint32 length=email.length();
    if(length<5) return 1;

    qint32 dogPos=email.indexOf('@');
    if(dogPos<1) return 1;

    qint32 dotPos=email.indexOf('.');
    if((dotPos<=(dogPos+1)) || (dotPos==(length-1))) return 4;

    qint32 n;
    QChar c;
    for(qint32 i=0;i<length;i++)
    {
        c=email.at(i);
        n=Alfavit.indexOf(c);
        if(n==-1) break;
    }
    return (n==-1)?1:0;
}

MyDialog::MyDialog(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer30Second()));
}

MyDialog::~MyDialog()
{
    m_timer.stop();
    disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer30Second()));

    deleteLater();
}

void MyDialog::execute(qint16 cmd,
                       qint32 sessionID,
                       QString param,
                       QByteArray rsa)
{
    QString temp;
    qint16 exitCode=0;

    switch(cmd)
    {
        case cmdLogon:
            temp=param+",3,4,5,6,7,8,9,10";
            m_Login=getParam(param,1);
            m_Password=getParam(param,2);
            break;
        case cmdLogoff:
            temp=QString::number(sessionID)+",2,3,4,5,6,7,8,9,10";
            break;
        case cmdRegistr:
            temp=param+",4,5,6,7,8,9,10";
            exitCode=checkEMail(getParam(param,2));
            break;
        case cmdRestorePswd:
            temp="1,"+param+",3,4,5,6,7,8,9,10";
            exitCode=checkEMail(param);
            break;
        case cmdChangePswd:
            temp=param+",5,6,7,8,9,10";
            exitCode=checkPassword(getParam(param,3));
            if((exitCode==0) && (getParam(param,3)!=getParam(param,4)))
                exitCode=1;
            if(getParam(param,1)=="")
                exitCode=6;

            m_Login=getParam(param,1);
            m_Password=getParam(param,3);
            break;
        case cmdUpdateDynam:
            temp=param+",,";
            break;
        case cmdAddAbonBook:
        case cmdDelAbonBook:
            temp=param;
            break;
        default:
            exitCode=1;
    }

    QByteArray ba;
    if(exitCode==0)
    {
        m_packet.Clear();
        m_packet.setSignature(signature_server_RSA);
        m_packet.setCmd(cmd);
        m_packet.setSessionID(sessionID);
        if((cmd==cmdAddAbonBook)||(cmd==cmdDelAbonBook))
        {
            m_packet.setAnyID(temp.toInt());
        }
        else
        {
            ba.clear();
            ba=temp.toUtf8();
            m_packet.setData(ba);
        }

        m_packet.setKey(rsa);

        m_step=1;
        m_count=0;
        m_timer.start(10000);

        if(cmd==cmdLogoff)
        {
            emit SessionID(0);
            emit Logon(QString(""),QString(""),0);
            emit Exit(this,cmd,exitCode);
        }
        else
            emit sendMessage(m_packet);
    }
    else
        emit Exit(this,cmd,exitCode);
}

void MyDialog::receiveMessage(MyPacket &packet)
{
    qint32 sessionID;
    quint16 cmd=packet.getCmd();
    quint16 m_cmd=m_packet.getCmd();

    if((cmd&0x0fff)!=m_cmd) return;

    if((m_cmd==cmdLogoff)||(m_cmd==cmdUpdateDynam)||(packet.getExitCode()))
    {
        emit Exit(this,m_cmd,packet.getExitCode());
    }
    else
    {
        if(m_step==1)
        {
            if(cmd&cmdMailRu)
            {
                if((m_cmd==cmdAddAbonBook)||(m_cmd==cmdDelAbonBook))
                {
                    emit Exit(this,m_cmd,packet.getExitCode());
                }
                else
                {
                    m_step=2;
                    m_count=0;
                }
            }
        }
        else
        {
            if((cmd&cmdAnswer)&&(packet.isAnyID()))
            {
                sessionID=packet.getSessionID();
                packet.setSignature(signature_server_RSA);
                packet.setCmd(cmdAnswer);
                emit sendMessage(packet);
                if((packet.getExitCode()==0)&&((m_cmd==cmdLogon)||(m_cmd==cmdChangePswd)))
                {
                    emit Logon(m_Login,m_Password,sessionID);
                    if(m_cmd==cmdLogon)
                    {
                        emit SessionID(sessionID);
                    }
                }
                emit Exit(this,m_cmd,packet.getExitCode());
            }
        }
    }
}

void MyDialog::Timer30Second()
{
    m_count++;
    if(m_step==1)
    {
        if(m_count>2)
        {
            emit Exit(this,m_packet.getCmd(),99);
        }
        else
        {
            emit sendMessage(m_packet);
        }
    }
    else
    {
        if(m_count>3) emit Exit(this,m_packet.getCmd(),99);
    }
}
