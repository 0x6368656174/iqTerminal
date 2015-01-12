#include "mystunmessage.h"

quint32 MySTUNmessage::_ntohl(const quint32 value)
{
   quint32 result;
   char *chr=(char *)&value;
   result=(chr[0]<<24)+(chr[1]<<16)+(chr[2]<<8)+chr[3];
   return result;
}

quint16 MySTUNmessage::_ntohs(const quint16 value)
{
   quint16 result;
   char *chr=(char *)&value;
   result=(chr[0]<<8)+chr[1];
   return result;
}

quint32 MySTUNmessage::_htonl(const quint32 value)
{
   quint32 result;
   char *chr=(char *)&result;
   chr[0]=(value>>24)&0xff;
   chr[1]=(value>>16)&0xff;
   chr[2]=(value>> 8)&0xff;
   chr[3]=(value    )&0xff;
   return result;
}

quint16 MySTUNmessage::_htons(const quint16 value)
{
   quint16 result;
   char *chr=(char *)&result;
   chr[0]=(value>>8)&0xff;
   chr[1]=(value   )&0xff;
   return result;
}

void MySTUNmessage::computeHmac(char *hmac/*, const char *input, qint32 length, const char *key, qint32 sizeKey*/)
{
   strncpy(hmac,"hmac-not-implemented",20);
}

bool MySTUNmessage::decodeAtrAddress(char *body, quint16 hdrLen, StunAtrAddress6 &result)
{
   if (hdrLen==(sizeof(MyIPv6Address)+2))
   {
      result.pad = *body++;
      result.family = *body++;
      if (result.family==IPv6Family)
      {
         memcpy(&result.ipv6,body,sizeof(MyIPv6Address));
         body+=sizeof(MyIPv6Address);
         return true;
      }
   }
   return false;
}

bool MySTUNmessage::decodeAtrChangeRequest(char *body, quint16 hdrLen, StunAtrChangeRequest &result)
{
   if (hdrLen==4)
   {
      memcpy(&result.value, body, 4);
      result.value = _ntohl(result.value);
      return true;
   }
   return false;
}

bool MySTUNmessage::decodeAtrError(char *body, quint16 hdrLen, StunAtrError &result)
{
   if (hdrLen<sizeof(result))
   {
      memcpy(&result.pad, body, 2); body+=2;
      result.pad = _ntohs(result.pad);
      result.errorClass = *body++;
      result.number = *body++;

      result.sizeReason = hdrLen - 4;
      memcpy(&result.reason, body, result.sizeReason);
      result.reason[result.sizeReason] = 0;
      return true;
   }
   return false;
}

bool MySTUNmessage::decodeAtrUnknown(char *body, quint16 hdrLen, StunAtrUnknown &result)
{
   if ((hdrLen<sizeof(result)) && ((hdrLen % 4)==0))
   {
      result.numAttributes = hdrLen / 4;
      for (int i=0; i<result.numAttributes; i++)
      {
         memcpy(&result.attrType[i], body, 2); body+=2;
         result.attrType[i] = _ntohs(result.attrType[i]);
      }
      return true;
   }
   return false;
}

bool MySTUNmessage::decodeAtrString(char *body, quint16 hdrLen, StunAtrString &result)
{
   if ((hdrLen<STUN_MAX_STRING) && ((hdrLen % 4)==0))
   {
      result.sizeValue = hdrLen;
      memcpy(&result.value, body, hdrLen);
      result.value[hdrLen] = 0;
      return true;
   }
   return false;
}

bool MySTUNmessage::decodeAtrIntegrity(char *body, quint16 hdrLen, StunAtrIntegrity &result)
{
   if (hdrLen == 20)
   {
      memcpy(&result.hash, body, hdrLen);
      return true;
   }
   return false;
}

char* MySTUNmessage::encode16(char *buf, quint16 data)
{
   quint16 ndata = _htons(data);
   memcpy(buf, reinterpret_cast<void*>(&ndata), sizeof(quint16));
   return buf + sizeof(quint16);
}

char* MySTUNmessage::encode32(char *buf, quint32 data)
{
   quint32 ndata = _htonl(data);
   memcpy(buf, reinterpret_cast<void*>(&ndata), sizeof(quint32));
   return buf + sizeof(quint32);
}

char* MySTUNmessage::encode(char *buf, const char *data, quint32 length)
{
   memcpy(buf, data, length);
   return buf + length;
}

char* MySTUNmessage::encodeAtrAddress(char *ptr, quint16 type, const StunAtrAddress6 &atr)
{
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, sizeof(MyIPv6Address)+2);
   *ptr++ = atr.pad;
   *ptr++ = IPv6Family;
   memcpy(ptr,&atr.ipv6,sizeof(MyIPv6Address));
   ptr+=sizeof(MyIPv6Address);
   return ptr;
}

char* MySTUNmessage::encodeAtrChangeRequest(char *ptr, const StunAtrChangeRequest &atr)
{
   ptr = encode16(ptr, ChangeRequest);
   ptr = encode16(ptr, 4);
   ptr = encode32(ptr, atr.value);
   return ptr;
}

char* MySTUNmessage::encodeAtrError(char *ptr, const StunAtrError &atr)
{
   ptr = encode16(ptr, ErrorCode);
   ptr = encode16(ptr, 6 + atr.sizeReason);
   ptr = encode16(ptr, atr.pad);
   *ptr++ = atr.errorClass;
   *ptr++ = atr.number;
   ptr = encode(ptr, atr.reason, atr.sizeReason);
   return ptr;
}

char* MySTUNmessage::encodeAtrUnknown(char *ptr, const StunAtrUnknown &atr)
{
   ptr = encode16(ptr, UnknownAttribute);
   ptr = encode16(ptr, 2+2*atr.numAttributes);
   for (qint32 i=0; i<atr.numAttributes; i++)
   {
      ptr = encode16(ptr, atr.attrType[i]);
   }
   return ptr;
}

char* MySTUNmessage::encodeXorOnly(char *ptr)
{
   ptr = encode16(ptr, XorOnly );
   return ptr;
}

char* MySTUNmessage::encodeAtrString(char *ptr, quint16 type, const StunAtrString &atr)
{
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, atr.sizeValue);
   ptr = encode(ptr, atr.value, atr.sizeValue);
   return ptr;
}

char* MySTUNmessage::encodeAtrIntegrity(char *ptr, const StunAtrIntegrity &atr)
{
   ptr = encode16(ptr, MessageIntegrity);
   ptr = encode16(ptr, 20);
   ptr = encode(ptr, atr.hash, sizeof(atr.hash));
   return ptr;
}

MySTUNmessage::MySTUNmessage()
{
}

bool MySTUNmessage::DecodeMessage(char *buf, quint16 bufLen)
{
   memset(&msg, 0, sizeof(msg));

   if (sizeof(StunMsgHdr) > bufLen) return false;

   memcpy(&msg.msgHdr, buf, sizeof(StunMsgHdr));
   msg.msgHdr.msgType   = _ntohs(msg.msgHdr.msgType);
   msg.msgHdr.msgLength = _ntohs(msg.msgHdr.msgLength);

   if ((msg.msgHdr.msgLength+quint16(sizeof(StunMsgHdr)))!=bufLen)
      return false;

   char* body = buf + sizeof(StunMsgHdr);
   quint16 size = msg.msgHdr.msgLength;

   while (size>0)
   {
      StunAtrHdr* attr = reinterpret_cast<StunAtrHdr*>(body);

      quint16 attrLen = _ntohs(attr->length);
      quint32  atrType = _ntohs(attr->type);

      if (attrLen+4>size) return false;

      body += 4;
      size -= 4;

      switch (atrType)
      {
         case MappedAddress:
            msg.hasMappedAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.mappedAddress))
               return false;
            break;

         case ResponseAddress:
            msg.hasResponseAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.responseAddress))
               return false;
            break;

         case ChangeRequest:
            msg.hasChangeRequest = true;
            if (!decodeAtrChangeRequest(body,attrLen,msg.changeRequest))
               return false;
            break;

         case SourceAddress:
            msg.hasSourceAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.sourceAddress))
               return false;
            break;

         case ChangedAddress:
            msg.hasChangedAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.changedAddress))
               return false;
            break;

         case Username:
            msg.hasUsername = true;
            if (!decodeAtrString(body,attrLen,msg.username))
               return false;
            break;

         case Password:
            msg.hasPassword = true;
            if (!decodeAtrString(body,attrLen,msg.password))
               return false;
            break;

         case MessageIntegrity:
            msg.hasMessageIntegrity = true;
            if (!decodeAtrIntegrity(body,attrLen,msg.messageIntegrity))
               return false;
            break;

         case ErrorCode:
            msg.hasErrorCode = true;
            if (!decodeAtrError(body,attrLen,msg.errorCode))
               return false;
            break;

         case UnknownAttribute:
            msg.hasUnknownAttributes = true;
            if (!decodeAtrUnknown(body,attrLen,msg.unknownAttributes))
               return false;
            break;

         case ReflectedFrom:
            msg.hasReflectedFrom = true;
            if (!decodeAtrAddress(body,attrLen,msg.reflectedFrom))
               return false;
            break;

         case XorMappedAddress:
            msg.hasXorMappedAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.xorMappedAddress))
               return false;
            break;

         case XorOnly:
            msg.xorOnly = true;
            break;

         case ServerName:
            msg.hasServerName = true;
            if (!decodeAtrString(body,attrLen,msg.serverName))
               return false;
            break;

         case SecondaryAddress:
            msg.hasSecondaryAddress = true;
            if (!decodeAtrAddress(body,attrLen,msg.secondaryAddress))
               return false;
            break;

         default:
            if (atrType <= 0x7FFF)
               return false;
      }
      body += attrLen;
      size -= attrLen;
   }
   return true;
}

quint32 MySTUNmessage::EncodeMessage(char *buf, quint32 bufLen, const StunAtrString &password)
{
   if(bufLen<1024) bufLen=1024;
   char* ptr = buf;

   ptr = encode16(ptr, msg.msgHdr.msgType);
   char* lengthp = ptr;
   ptr = encode16(ptr, 0);
   ptr = encode(ptr, reinterpret_cast<const char*>(msg.msgHdr.id), sizeof(msg.msgHdr.id));

   if (msg.hasMappedAddress)     ptr = encodeAtrAddress(ptr, MappedAddress, msg.mappedAddress);
   if (msg.hasResponseAddress)   ptr = encodeAtrAddress(ptr, ResponseAddress, msg.responseAddress);
   if (msg.hasChangeRequest)     ptr = encodeAtrChangeRequest(ptr, msg.changeRequest);
   if (msg.hasSourceAddress)     ptr = encodeAtrAddress(ptr, SourceAddress, msg.sourceAddress);
   if (msg.hasChangedAddress)    ptr = encodeAtrAddress(ptr, ChangedAddress, msg.changedAddress);
   if (msg.hasUsername)          ptr = encodeAtrString (ptr, Username, msg.username);
   if (msg.hasPassword)          ptr = encodeAtrString (ptr, Password, msg.password);
   if (msg.hasErrorCode)         ptr = encodeAtrError  (ptr, msg.errorCode);
   if (msg.hasUnknownAttributes) ptr = encodeAtrUnknown(ptr, msg.unknownAttributes);
   if (msg.hasReflectedFrom)     ptr = encodeAtrAddress(ptr, ReflectedFrom, msg.reflectedFrom);
   if (msg.hasXorMappedAddress)  ptr = encodeAtrAddress(ptr, XorMappedAddress, msg.xorMappedAddress);
   if (msg.xorOnly)              ptr = encodeXorOnly   ( ptr );
   if (msg.hasServerName)        ptr = encodeAtrString (ptr, ServerName, msg.serverName);
   if (msg.hasSecondaryAddress)  ptr = encodeAtrAddress(ptr, SecondaryAddress, msg.secondaryAddress);

   if (password.sizeValue > 0)
   {
      StunAtrIntegrity integrity;
      computeHmac(integrity.hash/*, buf, qint32(ptr-buf) , password.value, password.sizeValue*/);
      ptr = encodeAtrIntegrity(ptr, integrity);
   }

   encode16(lengthp, quint16(ptr - buf - sizeof(StunMsgHdr)));
   return qint32(ptr - buf);
}

void MySTUNmessage::Clear()
{
   memset(&msg, 0, sizeof(StunMessage));
}

void MySTUNmessage::SetID()
{
   for(qint32 i=0;i<16;i++)
      msg.msgHdr.id[i]=qrand()&0x7f;
}

void MySTUNmessage::SetMappedAddress(quint8 family, MyIPv6Address addr)
{
   msg.hasMappedAddress=true;
   msg.mappedAddress.family=family;
   msg.mappedAddress.ipv6=addr;
}

void MySTUNmessage::SetSourceAddress(quint8 family, MyIPv6Address addr)
{
   msg.hasSourceAddress=true;
   msg.sourceAddress.family=family;
   msg.sourceAddress.ipv6=addr;
}

void MySTUNmessage::SetChangedAddress(quint8 family, MyIPv6Address addr)
{
   msg.hasChangedAddress=true;
   msg.changedAddress.family=family;
   msg.changedAddress.ipv6=addr;
}

void MySTUNmessage::SetChangeRequest(bool ChangeIP, bool ChangePort)
{
   msg.hasChangeRequest=true;
   if(ChangeIP)   msg.changeRequest.value|=0x4;
   if(ChangePort) msg.changeRequest.value|=0x2;
}
