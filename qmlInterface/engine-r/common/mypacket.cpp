#include "mypacket.h"
#include <QTime>

QByteArray MyPacket::decodeRSA(QByteArray &Datagram, QByteArray &key)
{
    QByteArray baOut;
    MyRSA rsa;
    QBuffer bfIn;
    QBuffer bfOut;

    bfIn.setBuffer(&Datagram);
    bfIn.open(QBuffer::ReadWrite);
    rsa.LoadKey(&key,0);
    bfOut.setBuffer(&baOut);
    bfOut.open(QBuffer::ReadWrite);
    rsa.Decode(&bfIn,&bfOut);
    bfOut.close();
    bfIn.close();

    return baOut;
}

QByteArray MyPacket::encodeRSA(QByteArray &Datagram, QByteArray &key)
{
    QByteArray baOut;
    MyRSA rsa;
    QBuffer bfIn;
    QBuffer bfOut;

    bfIn.setBuffer(&Datagram);
    bfIn.open(QBuffer::ReadWrite);
    rsa.LoadKey(&key,1);
    bfOut.setBuffer(&baOut);
    bfOut.open(QBuffer::ReadWrite);
    rsa.Encode(&bfIn,&bfOut);
    bfOut.close();
    bfIn.close();

    return baOut;
}

QByteArray MyPacket::decodeRC5(QByteArray &Datagram, QByteArray &key)
{
    QByteArray baOut=Datagram;
    QByteArray work;
    MyRC5 rc5;
    rc5.DecryptCopy(work,Datagram.right(Datagram.size()-4),key);
    baOut.resize(4);
    baOut.append(work);

    return baOut;
}

QByteArray MyPacket::encodeRC5(QByteArray &Datagram, QByteArray &key)
{
    QByteArray baOut=Datagram;
    QByteArray work;
    MyRC5 rc5;
    rc5.EncryptCopy(work,Datagram.right(Datagram.size()-4),key);
    baOut.resize(4);
    baOut.append(work);

    return baOut;
}

bool MyPacket::decodeDatagram(QByteArray &Datagram)
{
    qint32 length=Datagram.size();
    char *data=Datagram.data();

    Clear();
    if(length<HeaderLength) return false;
    memcpy((void *)&m_signature,data,4); data+=4;
    memcpy((void *)&m_timestump,data,4); data+=4;
    memcpy((void *)&m_cmd,data,2);       data+=2;
    memcpy((void *)&m_exitcode,data,2);  data+=2;
    memcpy((void *)&m_userID,data,4);    data+=4;
    memcpy((void *)&m_sessionID,data,4); data+=4;
    memcpy((void *)&m_status,data,4);    data+=4;
    length-=HeaderLength;

    MyPreHeader hdr;
    while(length>0)
    {
        memcpy((void *)&hdr,data,4);
        data+=4;
        length-=4;
        if(length<hdr.length) break;

        switch(hdr.id)
        {
            case pctIDanyid:
                if(hdr.length!=4) break;
                b_anyID=true;
                memcpy((void *)&m_anyID,data,hdr.length);
                break;
            case pctIDipaddres:
                if(hdr.length!=sizeof(MyIPv6Address)) break;
                b_ipv6address=true;
                memcpy((void *)&m_ipv6address,data,hdr.length);
                break;
            case pctIDconnectblock:
                if(hdr.length!=sizeof(MyConnectBlock)) break;
                b_connectblock=true;
                memcpy((void *)&m_connectblock,data,hdr.length);
                break;
            case pctIDfileblock:
                if(hdr.length!=sizeof(MyFileBlock)) break;
                b_fileblock=true;
                memcpy((void *)&m_fileblock,data,hdr.length);
                break;
            case pctIDaudio:
                if(hdr.length!=sizeof(MyAudioBlock)) break;
                b_audio=true;
                memcpy((void *)&m_audio,data,hdr.length);
                break;
            case pctIDvideo:
                if(hdr.length!=sizeof(MyVideoBlock)) break;
                b_video=true;
                memcpy((void *)&m_video,data,hdr.length);
                break;
            case pctIDdata:
                if(hdr.length<0) break;
                b_data=true;
                m_data.resize(hdr.length);
                memcpy(m_data.data(),data,hdr.length);
                break;
            case pctIDkey:
                if(hdr.length<0) break;
                b_key=true;
                m_key.resize(hdr.length);
                memcpy(m_key.data(),data,hdr.length);
                break;
            default: length=-1; hdr.length=0;
        }
        data+=hdr.length;
        length-=hdr.length;
    }
    return (length==0);
}

QByteArray MyPacket::encodeDatagram()
{
    QByteArray ba;
    qint32 length=HeaderLength;

    QTime tm=QTime::currentTime();
    m_timestump=tm.msec()+1000*(tm.second()+60*(tm.minute()+60*tm.hour()));

    if(b_anyID)         length+=(4+sizeof(qint32));
    if(b_ipv6address)   length+=(4+sizeof(MyIPv6Address));
    if(b_connectblock)  length+=(4+sizeof(MyConnectBlock));
    if(b_fileblock)     length+=(4+sizeof(MyFileBlock));
    if(b_audio)         length+=(4+sizeof(MyAudioBlock));
    if(b_video)         length+=(4+sizeof(MyVideoBlock));
    if(b_data)          length+=(4+m_data.size());
    if(b_key)           length+=(4+m_key.size());

    ba.resize(length);
    char *data=ba.data();
    memcpy(data,(void *)&m_signature,4); data+=4;
    memcpy(data,(void *)&m_timestump,4); data+=4;
    memcpy(data,(void *)&m_cmd,2);       data+=2;
    memcpy(data,(void *)&m_exitcode,2);  data+=2;
    memcpy(data,(void *)&m_userID,4);    data+=4;
    memcpy(data,(void *)&m_sessionID,4); data+=4;
    memcpy(data,(void *)&m_status,4);    data+=4;

    MyPreHeader hdr;
    if(b_anyID)
    {
        hdr.id=pctIDanyid;
        hdr.length=4;
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_anyID,hdr.length);
        data+=hdr.length;
    }

    if(b_ipv6address)
    {
        hdr.id=pctIDipaddres;
        hdr.length=sizeof(MyIPv6Address);
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_ipv6address,hdr.length);
        data+=hdr.length;
    }

    if(b_connectblock)
    {
        hdr.id=pctIDconnectblock;
        hdr.length=sizeof(MyConnectBlock);
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_connectblock,hdr.length);
        data+=hdr.length;
    }

    if(b_fileblock)
    {
        hdr.id=pctIDfileblock;
        hdr.length=sizeof(MyFileBlock);
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_fileblock,hdr.length);
        data+=hdr.length;
    }

    if(b_audio)
    {
        hdr.id=pctIDaudio;
        hdr.length=sizeof(MyAudioBlock);
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_audio,hdr.length);
        data+=hdr.length;
    }

    if(b_video)
    {
        hdr.id=pctIDvideo;
        hdr.length=sizeof(MyVideoBlock);
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,(void *)&m_video,hdr.length);
        data+=hdr.length;
    }

    if(b_data)
    {
        hdr.id=pctIDdata;
        hdr.length=m_data.size();
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,m_data.data(),hdr.length);
        data+=hdr.length;
    }

    if(b_key)
    {
        hdr.id=pctIDkey;
        hdr.length=m_key.size();
        memcpy(data,(void *)&hdr,4);
        data+=4;
        memcpy(data,m_key.data(),hdr.length);
        data+=hdr.length;
    }

    return ba;
}

MyPacket::MyPacket()
{
    Clear();
}

void MyPacket::Clear()
{
    m_signature=signature_no_RSA;
    m_timestump=0;
    m_cmd=0;
    m_exitcode=0;
    m_userID=0;
    m_sessionID=0;
    m_status=0;

    b_anyID=false;
    m_anyID=0;

    b_ipv6address=false;
    memset((void *)&m_ipv6address,0,sizeof(MyIPv6Address));

    b_connectblock=false;
    memset((void *)&m_connectblock,0,sizeof(MyConnectBlock));

    b_fileblock=false;
    memset((void *)&m_fileblock,0,sizeof(MyFileBlock));

    b_audio=false;
    memset((void *)&m_audio,0,sizeof(MyAudioBlock));

    b_video=false;
    memset((void *)&m_video,0,sizeof(MyVideoBlock));

    b_key=false;
    m_key.resize(0);

    b_data=false;
    m_data.resize(0);
}

bool MyPacket::Decode(QByteArray &Datagram, QByteArray &key)
{
    QByteArray ba;
    quint32 signature;
    memcpy((void *)&signature,Datagram.data(),4);

    switch(signature)
    {
        case signature_client_RSA:
            ba=decodeRSA(Datagram,key);
            break;
        case signature_client_RC5:
            ba=decodeRC5(Datagram,key);
            break;
        default:
            ba=Datagram;
    }

    return decodeDatagram(ba);
}

QByteArray MyPacket::Encode(QByteArray &key)
{
    QByteArray ba=encodeDatagram();
    QByteArray baOut;

    switch(m_signature)
    {
        case signature_server_RSA:
            baOut=encodeRSA(ba,key);
            break;
        case signature_client_RC5:
            baOut=decodeRC5(ba,key);
            break;
        default:
            baOut=ba;
    }
    return baOut;
}

quint32 MyPacket::getSignature()
{
    return m_signature;
}

void MyPacket::setSignature(quint32 Signature)
{
    m_signature=Signature;
}

quint16 MyPacket::getCmd()
{
    return m_cmd;
}

void MyPacket::setCmd(quint16 Cmd)
{
    m_cmd=Cmd;
}

quint16 MyPacket::getExitCode()
{
    return m_exitcode;
}

void MyPacket::setExitCode(quint16 ExitCode)
{
    m_exitcode=ExitCode;
}

qint32 MyPacket::getUserID()
{
    return m_userID;
}

void MyPacket::setUserID(qint32 UserID)
{
    m_userID=UserID;
}

qint32 MyPacket::getSessionID()
{
    return m_sessionID;
}

void MyPacket::setSessionID(qint32 SessionID)
{
    m_sessionID=SessionID;
}

qint32 MyPacket::getStatus()
{
    return m_status;
}

void MyPacket::setStatus(qint32 Status)
{
    m_status=Status;
}

bool MyPacket::isAnyID()
{
    return b_anyID;
}

qint32 MyPacket::getAnyID()
{
    return m_anyID;
}

void MyPacket::setAnyID(qint32 AnyID)
{
    b_anyID=true;
    m_anyID=AnyID;
}

bool MyPacket::isIPv6Address()
{
    return b_ipv6address;
}

MyIPv6Address MyPacket::getIPv6Address()
{
    return m_ipv6address;
}

void MyPacket::setIPv6Address(const MyIPv6Address &IPv6Address)
{
    b_ipv6address=true;
    m_ipv6address=IPv6Address;
}

bool MyPacket::isConnectBlock()
{
    return b_connectblock;
}

MyConnectBlock MyPacket::getConnectBlock()
{
    return m_connectblock;
}

void MyPacket::setConnectBlock(const MyConnectBlock &ConnectBlock)
{
    b_connectblock=true;
    m_connectblock=ConnectBlock;
}

void MyPacket::setConnectBlock(qint32 ClientID,
                               qint32 UserID,
                               qint32 SessionID,
                               quint16 nRec,
                               const MyIPv6Address &WAN,
                               const MyIPv6Address &WLAN,
                               const MyIPv6Address &LAN,
                               quint16 PortMapping)
{
    b_connectblock=true;
    m_connectblock.clientID=ClientID;
    m_connectblock.userID=UserID;
    m_connectblock.sessionID=SessionID;
    m_connectblock.nRec=nRec;
    m_connectblock.wan_addr=WAN;
    m_connectblock.wlan_addr=WLAN;
    m_connectblock.lan_addr=LAN;
    m_connectblock.port_mapping=PortMapping;
}

bool MyPacket::isFileBlock()
{
    return b_fileblock;
}

MyFileBlock MyPacket::getFileBlock()
{
    return m_fileblock;
}

void MyPacket::setFileBlock(const MyFileBlock &FileBlock)
{
    b_fileblock=true;
    m_fileblock=FileBlock;
}

void MyPacket::setFileBlock(qint32 FileID, qint64 Pos, quint8 Mask)
{
    b_fileblock=true;
    m_fileblock.fileID=FileID;
    m_fileblock.pos=Pos;
    m_fileblock.mask=Mask;
}

bool MyPacket::isAudio()
{
    return b_audio;
}

MyAudioBlock MyPacket::getAudio()
{
    return m_audio;
}

void MyPacket::setAudio(const MyAudioBlock &Audio)
{
    b_audio=true;
    m_audio=Audio;
}

void MyPacket::setAudio(qint32 UserID,
                        qint32 ttl,
                        qint32 SampleRate,
                        qint16 Channels,
                        qint16 SampleSize)
{
    b_audio=true;
    m_audio.userID=UserID;
    m_audio.ttl=ttl;
    m_audio.channels=Channels;
    m_audio.samplerate=SampleRate;
    m_audio.samplesize=SampleSize;
}

bool MyPacket::isVideo()
{
    return b_video;
}

MyVideoBlock MyPacket::getVideo()
{
    return m_video;
}

void MyPacket::setVideo(const MyVideoBlock &Video)
{
    b_video=true;
    m_video=Video;
}

void MyPacket::setVideo(qint32 Kadr, qint64 Length, qint32 Block)
{
    b_video=true;
    m_video.kadr=Kadr;
    m_video.length=Length;
    m_video.block=Block;
}

bool MyPacket::isKey()
{
    return b_key;
}

QByteArray MyPacket::getKey()
{
    return m_key;
}

void MyPacket::setKey(const QByteArray &Key)
{
    b_key=true;
    m_key=Key;
}

bool MyPacket::isData()
{
    return b_data;
}

QByteArray MyPacket::getData()
{
    return m_data;
}

void MyPacket::setData(const QByteArray &Data)
{
    b_data=true;
    m_data=Data;
}
