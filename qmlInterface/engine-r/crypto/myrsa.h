#ifndef MYRSA_H
#define MYRSA_H

#include <QBuffer>
#include "mybigint.h"

typedef struct {
   MyBigInt a, b;
} MyCryptoKey;

class MyRSA
{
   MyCryptoKey private_key, public_key;

public:
   MyRSA();

   void LoadKey(QByteArray *key, qint32 typeKey);

   void Encode(QBuffer *bufferIn, QBuffer *bufferOut);
   void Decode(QBuffer *bufferIn, QBuffer *bufferOut);

   qint32 getPrivateKeyLength();
   qint32 getPublicKeyLength();
};

#endif // MYRSA_H
