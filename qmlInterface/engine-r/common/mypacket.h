#ifndef MYPACKET_H
#define MYPACKET_H

#include <qglobal.h>
#include <QByteArray>
#include <QHostAddress>
#include "../MyGlobal.h"
#include "../crypto/myrsa.h"
#include "../crypto/myrc5.h"

#define pctIDanyid        10
#define pctIDipaddres     11
#define pctIDconnectblock 12
#define pctIDfileblock    13
#define pctIDdata         14
#define pctIDkey          15
#define pctIDaudio        16
#define pctIDvideo        17

//---------------------------------------------------------------------------
// сигнатуры
#define signature_server_RSA  0xa1b2c3d4	// пакет шифрованный ключом сервера
#define signature_client_RSA  0xa4b3c2d1	// пакет шифрованный ключом клиента
#define signature_client_RC5  0xa1b1c1d1	// пакет шифрованный ключом RC5
#define signature_no_RSA   	  0xa0b0c0d0	// пакет не шифрованный

//---------------------------------------------------------------------------
// типы передаваемых данных
//#define t_dataUnknown 0
//#define t_dataSMS 1
//#define t_dataAudio 2
//#define t_dataVideo 3
//#define t_dataFile 4

#define typeSMS     0
#define typeAudio   1
#define typeVideo   2
#define typeFile    3
//---------------------------------------------------------------------------
// размерности буферов
#define MaxDataLength 1024

#pragma pack(1)
//---------------------------------------------------------------------------
// описание структур данных
//---------------------------------------------------------------------------
// подзаголовок блока
typedef struct
{
   qint16 id;
   qint16 length;
} MyPreHeader;

// блок параметров connecta
typedef struct
{
   qint32 clientID;
   qint32 userID;
   qint32 sessionID;
   qint32 nRec;
   MyIPv6Address wan_addr;
   MyIPv6Address wlan_addr;
   MyIPv6Address lan_addr;
   quint16    port_mapping;
} MyConnectBlock;

// блок параметров обмена файлами
typedef struct
{
   qint32 fileID;
   qint64 pos;
   quint8 mask;
} MyFileBlock;

// блок параметров обмена аудио данных
typedef struct
{
   qint32 userID;
   qint32 ttl;
   qint32 samplerate;
   qint16 channels;
   qint16 samplesize;
} MyAudioBlock;

// блок параметров обмена видео данных
typedef struct
{
   qint32 kadr;
   qint64 length;
   qint32 block;
} MyVideoBlock;

#pragma pack(8)
#define HeaderLength 24
class MyPacket
{
private:
   quint32 m_signature;
   quint32 m_timestump;
   quint16 m_cmd;
   quint16 m_exitcode;
   qint32  m_userID;
   qint32  m_sessionID;
   qint32  m_status;

   bool b_anyID;
   qint32 m_anyID;

   bool b_ipv6address;
   MyIPv6Address m_ipv6address;

   bool b_connectblock;
   MyConnectBlock m_connectblock;

   bool b_fileblock;
   MyFileBlock m_fileblock;

   bool b_audio;
   MyAudioBlock m_audio;

   bool b_video;
   MyVideoBlock m_video;

   bool b_key;
   QByteArray m_key;

   bool b_data;
   QByteArray m_data;

   QByteArray decodeRSA(QByteArray &Datagram, QByteArray &key);
   QByteArray encodeRSA(QByteArray &Datagram, QByteArray &key);

   QByteArray decodeRC5(QByteArray &Datagram, QByteArray &key);
   QByteArray encodeRC5(QByteArray &Datagram, QByteArray &key);

   bool decodeDatagram(QByteArray &Datagram);
   QByteArray encodeDatagram();

public:
   MyPacket();

   void Clear();
   bool Decode(QByteArray &Datagram, QByteArray &key);
   QByteArray Encode(QByteArray &key);

   quint32 getSignature();
   void    setSignature(quint32 Signature);

   quint16 getCmd();
   void    setCmd(quint16 Cmd);

   quint16 getExitCode();
   void    setExitCode(quint16 ExitCode);

   qint32 getUserID();
   void   setUserID(qint32 UserID);

   qint32 getSessionID();
   void   setSessionID(qint32 SessionID);

   qint32 getStatus();
   void   setStatus(qint32 Status);

   bool   isAnyID();
   qint32 getAnyID();
   void   setAnyID(qint32 AnyID);

   bool          isIPv6Address();
   MyIPv6Address getIPv6Address();
   void          setIPv6Address(const MyIPv6Address &IPv6Address);

   bool           isConnectBlock();
   MyConnectBlock getConnectBlock();
   void           setConnectBlock(const MyConnectBlock &ConnectBlock);
   void           setConnectBlock(qint32 ClientID,
                                  qint32 UserID,
                                  qint32 SessionID,
                                  quint16 nRec,
                                  const MyIPv6Address &WAN,
                                  const MyIPv6Address &WLAN,
                                  const MyIPv6Address &LAN,
                                  quint16 PortMapping);

   bool        isFileBlock();
   MyFileBlock getFileBlock();
   void        setFileBlock(const MyFileBlock &FileBlock);
   void        setFileBlock(qint32 FileID, qint64 Pos, quint8 Mask);

   bool         isAudio();
   MyAudioBlock getAudio();
   void         setAudio(const MyAudioBlock &Audio);
   void         setAudio(qint32 UserID,
                         qint32 ttl,
                         qint32 SampleRate,
                         qint16 Channels,
                         qint16 SampleSize);

   bool         isVideo();
   MyVideoBlock getVideo();
   void         setVideo(const MyVideoBlock &Video);
   void         setVideo(qint32 Kadr, qint64 Length, qint32 Block);

   bool       isKey();
   QByteArray getKey();
   void       setKey(const QByteArray &Key);

   bool       isData();
   QByteArray getData();
   void       setData(const QByteArray &Data);
};
#endif // MYPACKET_H
