#ifndef MYRSAEXT_H
#define MYRSAEXT_H

#include <QObject>

#include "mybigint.h"
#include "myrsa.h"

class MyRSAext : public QObject
{
   Q_OBJECT

   qint32 keyLength;
   MyCryptoKey private_key, public_key;

   void     extEuclid(MyBigInt  a, MyBigInt  b, MyBigInt &y);
   bool     isPrime  (MyBigInt p);
   MyBigInt getPrime (qint32 length);
   void     SaveKey  (qint32 typeKey, QString HomeDir);
   QString  SaveKey  (qint32 typeKey);

public:
   explicit MyRSAext(QObject *parent = 0);

   void Create(qint32 length, QString HomeDir);
   void Create(qint32 length, QString &PrivateKey, QString &PublicKey);

signals:
   void message(QString msg);

public slots:

};
#endif // MYRSAEXT_H
