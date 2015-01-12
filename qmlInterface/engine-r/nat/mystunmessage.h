#ifndef MYSTUNMESSAGE_H
#define MYSTUNMESSAGE_H
#include "../MyGlobal.h"

#define STUN_MAX_STRING 256
#define STUN_MAX_UNKNOWN_ATTRIBUTES 8
#define STUN_MAX_MESSAGE_SIZE 2048

// define a structure to hold a stun address
const quint8 IPv4Family = 0x01;
const quint8 IPv6Family = 0x02;

// флаги
const quint32 ChangeIpFlag   = 0x04;
const quint32 ChangePortFlag = 0x02;

// stun аттрибуты
const quint16 MappedAddress    = 0x0001;
const quint16 ResponseAddress  = 0x0002;
const quint16 ChangeRequest    = 0x0003;
const quint16 SourceAddress    = 0x0004;
const quint16 ChangedAddress   = 0x0005;
const quint16 Username         = 0x0006;
const quint16 Password         = 0x0007;
const quint16 MessageIntegrity = 0x0008;
const quint16 ErrorCode        = 0x0009;
const quint16 UnknownAttribute = 0x000A;
const quint16 ReflectedFrom    = 0x000B;
const quint16 XorMappedAddress = 0x8020;
const quint16 XorOnly          = 0x0021;
const quint16 ServerName       = 0x8022;
const quint16 SecondaryAddress = 0x8050;

// типы stun сообщений
const quint16 BindRequestMsg               = 0x0001;
const quint16 BindResponseMsg              = 0x0101;
const quint16 BindErrorResponseMsg         = 0x0111;
const quint16 SharedSecretRequestMsg       = 0x0002;
const quint16 SharedSecretResponseMsg      = 0x0102;
const quint16 SharedSecretErrorResponseMsg = 0x0112;

typedef struct
{
   quint16 msgType;
   quint16 msgLength;
   quint8  id[16];
} StunMsgHdr;

typedef struct
{
   quint16 type;
   quint16 length;
} StunAtrHdr;

typedef struct
{
   quint8 pad;
   quint8 family;
   MyIPv6Address ipv6;
} StunAtrAddress6;

typedef struct
{
   quint32 value;
} StunAtrChangeRequest;

typedef struct
{
   quint16 pad;
   quint8 errorClass;
   quint8 number;
   char reason[STUN_MAX_STRING];
   quint16 sizeReason;
} StunAtrError;

typedef struct
{
   quint16 attrType[STUN_MAX_UNKNOWN_ATTRIBUTES];
   quint16 numAttributes;
} StunAtrUnknown;

typedef struct
{
   char value[STUN_MAX_STRING];
   quint16 sizeValue;
} StunAtrString;

typedef struct
{
   char hash[20];
} StunAtrIntegrity;

typedef enum
{
   HmacUnkown=0,
   HmacOK,
   HmacBadUserName,
   HmacUnkownUserName,
   HmacFailed,
} StunHmacStatus;

typedef struct
{
   StunMsgHdr msgHdr;

   bool hasMappedAddress;
   StunAtrAddress6  mappedAddress;

   bool hasResponseAddress;
   StunAtrAddress6  responseAddress;

   bool hasChangeRequest;
   StunAtrChangeRequest changeRequest;

   bool hasSourceAddress;
   StunAtrAddress6 sourceAddress;

   bool hasChangedAddress;
   StunAtrAddress6 changedAddress;

   bool hasUsername;
   StunAtrString username;

   bool hasPassword;
   StunAtrString password;

   bool hasMessageIntegrity;
   StunAtrIntegrity messageIntegrity;

   bool hasErrorCode;
   StunAtrError errorCode;

   bool hasUnknownAttributes;
   StunAtrUnknown unknownAttributes;

   bool hasReflectedFrom;
   StunAtrAddress6 reflectedFrom;

   bool hasXorMappedAddress;
   StunAtrAddress6  xorMappedAddress;

   bool xorOnly;

   bool hasServerName;
   StunAtrString serverName;

   bool hasSecondaryAddress;
   StunAtrAddress6 secondaryAddress;
} StunMessage;

class MySTUNmessage
{
private:
   quint32 _ntohl(const quint32 value);
   quint16 _ntohs(const quint16 value);
   quint32 _htonl(const quint32 value);
   quint16 _htons(const quint16 value);

   void computeHmac(char* hmac/*, const char* input, qint32 length, const char* key, qint32 sizeKey*/);

   bool decodeAtrAddress      (char *body, quint16 hdrLen, StunAtrAddress6& result);
   bool decodeAtrChangeRequest(char *body, quint16 hdrLen, StunAtrChangeRequest& result);
   bool decodeAtrError        (char *body, quint16 hdrLen, StunAtrError& result);
   bool decodeAtrUnknown      (char *body, quint16 hdrLen, StunAtrUnknown& result);
   bool decodeAtrString       (char *body, quint16 hdrLen, StunAtrString& result);
   bool decodeAtrIntegrity    (char *body, quint16 hdrLen, StunAtrIntegrity& result);

   char* encode16(char *buf, quint16 data);
   char* encode32(char *buf, quint32 data);
   char* encode  (char *buf, const char *data, quint32 length);

   char* encodeAtrAddress      (char *ptr, quint16 type, const StunAtrAddress6& atr);
   char* encodeAtrChangeRequest(char *ptr, const StunAtrChangeRequest& atr);
   char* encodeAtrError        (char *ptr, const StunAtrError& atr);
   char* encodeAtrUnknown      (char *ptr, const StunAtrUnknown& atr);
   char* encodeXorOnly         (char *ptr);
   char* encodeAtrString       (char *ptr, quint16 type, const StunAtrString& atr);
   char* encodeAtrIntegrity    (char *ptr, const StunAtrIntegrity& atr);

public:
   StunMessage msg;

   MySTUNmessage();

   bool    DecodeMessage(char *buf, quint16 bufLen);
   quint32 EncodeMessage(char *buf, quint32 bufLen, const StunAtrString& password);

   void Clear();
   void SetID();
   void SetMappedAddress (quint8 family, MyIPv6Address addr);
   void SetSourceAddress (quint8 family, MyIPv6Address addr);
   void SetChangedAddress(quint8 family, MyIPv6Address addr);
   void SetChangeRequest (bool ChangeIP, bool ChangePort);
};

#endif // MYSTUNMESSAGE_H
