#include "myfileload.h"
#include <QUrl>

void MyFileLoad::prepareFile(qint64 length)
{
    qint64 kBytes=(length+1023)>>10;
    qint32 count=(kBytes+7)>>3;
    qDebug() << kBytes << "kBytes" << count;

    m_map.resize(count);
    for(qint32 i=0;i<count;i++) m_map[i]=char(0xff);

    kBytes=kBytes&0x7;
    if(kBytes)
    {
        quint8 mask=(quint8(1)<<kBytes)-1;
        m_map[count-1]=char(mask);
    }

    if(m_file.exists() && loadTorrent())
    {
        m_file.open(QIODevice::ReadWrite);
        count=0;
        for(qint32 i=0;i<m_map.size();i++)
            if(m_map.at(i)!=0) count++;
    }
    else
    {
        m_file.open(QIODevice::ReadWrite);
        m_file.resize(length);
    }

    m_countitem=count>>11;
    if(m_countitem>8) m_countitem=8;
}

bool MyFileLoad::isLoad()
{
    if(m_state==0) return false;
    for(qint32 i=0;i<m_map.size();i++)
        if(m_map.at(i)!=0) return false;

    return true;
}

void MyFileLoad::getBlock(MyFileLoadBlock *item)
{
    qint64 pos=-1;
    quint8 mask;
    for(qint64 i=0;i<m_map.size();i++)
    {
        m_map_current++;
        if(m_map_current>=m_map.size()) m_map_current=0;
        mask=quint8(m_map.at(m_map_current));
        if(mask!=0)
        {
            pos=m_map_current;
            break;
        }
    }

    if(pos==-1) mask=0;
    item->setBlock(pos,mask);
}

void MyFileLoad::loaded()
{

    for(qint32 i=0;i<m_item.size();i++)
    {
        disconnect(m_item.at(i),0,0,0);
        delete m_item.at(i);
    }

    m_file.close();
    if(isLoad())
    {
        removeTorrent();
        m_state=2;
        qDebug() << "loaded OK";
    }
    else
    {
        if(m_state!=0) saveTorrent();
        m_state=3;
        qDebug() << "not loaded";
    }
}

void MyFileLoad::saveTorrent()
{
    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());
    file.open(QIODevice::ReadWrite);

    file.write(m_map);
    file.close();
}

bool MyFileLoad::loadTorrent()
{
    bool result=false;
    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());

    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QByteArray data=file.readAll();
        file.close();

        if(data.size()==m_map.size())
        {
            m_map=data;
            result=true;
        }
    }
    return result;
}

void MyFileLoad::removeTorrent()
{
    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());
    file.remove();
}

void MyFileLoad::addConnect()
{
    MyFileLoadBlock *item=new MyFileLoadBlock(this);
    connect(item, SIGNAL(statusBlock(MyFileLoadBlock*,qint32)),
            this, SLOT  (statusClient(MyFileLoadBlock*,qint32)));
    connect(item, SIGNAL(blockCompleted(MyFileLoadBlock *)),
            this, SLOT  (blockCompleted(MyFileLoadBlock *)));

    item->init(m_config,m_userID,m_messageID,m_filename);
    m_item.append(item);
}

void MyFileLoad::delConnect(MyFileLoadBlock *item)
{
    for(qint32 i=0;i<m_item.size();i++)
    {
        if(m_item.at(i)==item)
        {
            m_item.removeAt(i);
            break;
        }
    }
    delete item;
}

MyFileLoad::MyFileLoad(QObject *parent) :
    QObject(parent)
{
}

void MyFileLoad::load(MyConfig *config, QString filename)
{
    qDebug() << "load" << filename;

    m_config=config;
    m_filename=filename;
    QUrl path(m_filename);
    m_file.setFileName(path.toLocalFile());

    // отметим прием файла
    QString fname=path.fileName();

    qint32 def1, def2;
    def1=fname.indexOf("-",0);
    def2=fname.indexOf("-",def1+1);

    m_userID=fname.mid(0,def1).toInt();
    m_messageID=fname.mid(def1+1,def2-def1-1).toInt();

    addConnect();

    m_state=0;
    m_countitem=0;
    m_map_current=-1;
}

qint32 MyFileLoad::getState()
{
    return m_state;
}

qint32 MyFileLoad::getUserID()
{
    return m_userID;
}

QString MyFileLoad::getFileName()
{
    return m_filename;
}

void MyFileLoad::blockCompleted(MyFileLoadBlock *item)
{
    qint64 pos=item->getBlock();
    quint8 mask=item->getMask();

    QByteArray data;

    if((pos>=0) && (pos<m_map.size()) && mask)
    {
        quint8 oldmask=quint8(m_map.at(qint32(pos)));
        qint64 file_pos=pos<<13;
        quint8 kByte=0x1;
        for(qint32 i=0;i<8;i++)
        {
            if((mask&kByte) && (oldmask&kByte))
            {
                data=item->getData(i);
                if(data.size()>1024) data.resize(1024);
                m_file.seek(file_pos);
                m_file.write(data);
                oldmask^=kByte;
            }
            file_pos+=1024;
            kByte<<=1;
        }
        m_map[qint32(pos)]=char(oldmask);

        getBlock(item);
    }
}

void MyFileLoad::statusClient(MyFileLoadBlock *item, qint32 status)
{
    qDebug() << "status item" << status;

    switch(status)
    {
        case 2:
            if(m_state==0)
            {
                prepareFile(item->getFileLength());
                m_state=1;
            }
            getBlock(item);

            if(m_countitem>0)
            {
                addConnect();
                m_countitem--;
            }
            break;
        case 3:
            delConnect(item);
            if(m_item.size()==0) loaded();
            break;
    }
}
