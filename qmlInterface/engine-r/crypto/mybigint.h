#ifndef MYBIGINT_H
#define MYBIGINT_H

#include <QtGlobal>
#include <QBuffer>

#define myBigIntLength 384

class MyBigInt
{
   quint16 digits[myBigIntLength];

   void Inv();

public:
   MyBigInt();
   MyBigInt(const MyBigInt &obj);
   MyBigInt(const qint32 b);

   void Load(char *buffer, qint32 length);
   void UpLoad(char *buffer, qint32 length);

   bool isEven();
   bool isPositive();
   bool isZero();
   bool isAbsBigger(const MyBigInt &obj);

   qint32 getBit(qint32 index);
   qint32 getLength();
   qint32 getByteLength();

   void random(qint32 length, qint32 type);
   MyBigInt abs();
   MyBigInt Pow(const qint32 power);
   MyBigInt Pow(const MyBigInt power);
   MyBigInt PowMod(const MyBigInt power, const MyBigInt obj);
   MyBigInt DivMod(const MyBigInt &obj, MyBigInt &reminder);
   MyBigInt MaxDiviser(MyBigInt b);
   quint32  Mod(const qint32 a);

   MyBigInt operator=(const MyBigInt &obj);
   MyBigInt operator=(const qint32 b);
   MyBigInt operator+(const MyBigInt &obj);
   MyBigInt operator+(const qint32 b);
   MyBigInt operator*(const MyBigInt &obj);
   MyBigInt operator*(const qint32 b);
   MyBigInt operator-(const MyBigInt &obj);
   MyBigInt operator-(const qint32 b);
   MyBigInt operator%(const MyBigInt &obj);
   MyBigInt operator%(const qint32 b);
   MyBigInt operator/(const MyBigInt &obj);
   MyBigInt operator/(const qint32 b);
   MyBigInt operator+=(const MyBigInt &obj);
   MyBigInt operator+=(const qint32 b);
   MyBigInt operator-=(const MyBigInt &obj);
   MyBigInt operator-=(const qint32 b);

   MyBigInt operator>>(const qint32 b);
   MyBigInt operator<<(const qint32 b);

   bool operator>(const MyBigInt &obj);
   bool operator>(const qint32 b);
   bool operator<(const MyBigInt &obj);
   bool operator<(const qint32 b);
   bool operator==(const MyBigInt &obj);
   bool operator==(const qint32 b);
   bool operator!=(const MyBigInt &obj);
   bool operator!=(const qint32 b);
   bool operator>=(const MyBigInt &obj);
   bool operator>=(const qint32 b);
   bool operator<=(const MyBigInt &obj);
   bool operator<=(const qint32 b);

   void FromString(QBuffer *buffer);
   void ToString  (QBuffer *buffer);
   void ToString10(QBuffer *buffer);
};

#endif // MYBIGINT_H
