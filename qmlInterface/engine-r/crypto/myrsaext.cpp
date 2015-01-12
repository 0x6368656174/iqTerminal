#include "myRSAext.h"
#include <QFile>

void MyRSAext::extEuclid(MyBigInt a, MyBigInt b, MyBigInt &y)
{
   MyBigInt r, q, a11(1), a12, a21, a22(1);
   MyBigInt A11, A12, A21 ,A22;

   while ( b > 0)
   {
      r = a%b;
      q = a/b;
      if (r == 0) break;

      A11 = a12;
      A12 = a11-q*a12;
      A21 = a22;
      A22 = a21-q*a22;

      a11 = A11, a12 = A12, a21 = A21, a22 = A22;

      a = b;
      b = r;
   }
   y = a22;
}

bool MyRSAext::isPrime(MyBigInt p)
{
   if (p == 1 || p.isZero() || p.isEven()) return false;
   if (p == 2 || p == 3 || p == 5 || p == 7 || p == 9) return true;

   bool result=true;
   MyBigInt temp(p), t, a, c, x;
   temp-=1;

   //найти s и t, где N-1=2^s*t
   t=temp;
   qint32 s = 0;
   for(qint32 i=1;i<16*myBigIntLength;i++)
   {
      if(t.getBit(i))
      {
         s=i;
         break;
      }
   }
   t=temp >> s;

   // определить длину числа
   qint32 size=0;
   for(qint32 i=16*myBigIntLength-1;i>=0;i--)

      if(p.getBit(i))
      {
         size=i; break;
      }

   qint32 count=size >> 2;
   for(int i=0;i<count;i++)
   {
      // Выбрать случайное число 1 < a < N
      do { a.random(size-8,0); } while((a>=p) || (a<3));

      // исходное делиться без остатка?
      if((p % a)==0) { result=false; break; }

      // c=a^t mod N  ==1 ? OK
      c=a.PowMod(t,p);
      if((c==1) || (c==temp)) continue;

      // для k от 0 до s-1 проверяем a^(2^k*t) mod N
      result=false;
      for(int k=0;k<s;k++)
      {
         x=t<<k;
         c=a.PowMod(x,p);

         if((c==1) || (c==temp)) { result=true; break; }
      }

      if(!result) break;
   }
   return result;
}

MyBigInt MyRSAext::getPrime(qint32 length)
{
   qint32 size=0xfffff;
   qint32 c=size >> 1;

   // Формирование простых чисел до size;
   char* erato=new char[size];
   memset(erato,1,size);
   erato[0]=0;
   erato[1]=0;
   qint32 step;
   for(qint32 i=2;i<c;i++)
   {
      if(erato[i])
      {
         step=i<<1; while(step<size) { erato[step]=0; step+=i; }
      }
   }

   // подсчет числа простых чисел
   qint32 count_prime=0;
   for(qint32 i=0;i<size;i++)
      if(erato[i]) count_prime++;

   //  выборка простых чисел
   qint32 *stek=new qint32[count_prime];
   c=0;
   for(qint32 i=0;i<size;i++)
      if(erato[i])
      {
         stek[c]=i; c++;
      }

   // формирование базового числа
   MyBigInt base;
   base.random(length,1);

   // предварительное просеивание
   memset(erato,1,size);
   for(qint32 i=0;i<count_prime;i++)
   {
      c=base.Mod(stek[i]);
      if(c>0) c=stek[i]-c;
      step=stek[i];
      while(c<size)
      {
         erato[c]=0;
         c+=step;
      }
   }

   // Тест Миллера-Рабина
   MyBigInt temp;
   for(qint32 i=0;i<size;i++)
      if(erato[i])
      {
         temp=base+i;
         if(isPrime(temp)) break;
      }

   delete [] erato;
   delete [] stek;
   return temp;
}

void MyRSAext::SaveKey(qint32 typeKey, QString HomeDir)
{
   QFile out;
   QBuffer buffer;
   MyCryptoKey* key=NULL;

   switch(typeKey)
   {
      case 0: //  private key
         key=&private_key;
         out.setFileName(HomeDir+QString("/clientRSA.pri"));
         break;
      case 1: //  public key;
         key=&public_key;
         out.setFileName(HomeDir+QString("/clientRSA.pub"));
         break;
   }

   if(key)
   {
      buffer.open(QBuffer::ReadWrite);
      key->a.ToString(&buffer);
      buffer.putChar(':');
      key->b.ToString(&buffer);
      out.open(QIODevice::WriteOnly);
      out.write(buffer.data());
      out.close();
      buffer.close();
   }
}

QString MyRSAext::SaveKey(qint32 typeKey)
{
   QString strKey="";
   QBuffer buffer;
   MyCryptoKey* key=NULL;

   switch(typeKey)
   {
      case 0: //  private key
         key=&private_key;
         break;
      case 1: //  public key;
         key=&public_key;
         break;
   }

   if(key)
   {
      buffer.open(QBuffer::ReadWrite);
      key->a.ToString(&buffer);
      buffer.putChar(':');
      key->b.ToString(&buffer);

      QByteArray ba=buffer.data();
      strKey=QString::fromUtf8(ba);
      buffer.close();
   }
   return strKey;
}

MyRSAext::MyRSAext(QObject *parent) :
   QObject(parent)
{
}

void MyRSAext::Create(qint32 length, QString HomeDir)
{
   MyBigInt p, q, n, m, e, d;
   QString str;

   keyLength=length;

   str.setNum(keyLength);
   emit message("Генерация ключа шифрования "+str+" бит");

   // поиск простого числа p
   p=getPrime(keyLength>>1);
   q=getPrime(keyLength>>1);

   n=p*q;
   m=(p-1)*(q-1);
   do
   {
      do
      {
         d.random(keyLength-8,0);
         e = d. MaxDiviser(m);
      }
      while ( e != 1);
      extEuclid(m, d, e);
   }
   while (e < 0);

   private_key.a=d;
   private_key.b=n;
   public_key.a=e;
   public_key.b=n;
   emit message("Ключ шифрования создан");

   SaveKey(0,HomeDir);
   SaveKey(1,HomeDir);
}

void MyRSAext::Create(qint32 length, QString &PrivateKey, QString &PublicKey)
{
   MyBigInt p, q, n, m, e, d;
   QString str;

   keyLength=length;

   str.setNum(keyLength);
   // поиск простого числа p
   p=getPrime(keyLength>>1);
   q=getPrime(keyLength>>1);

   n=p*q;
   m=(p-1)*(q-1);
   do
   {
      do
      {
         d.random(keyLength-8,0);
         e = d. MaxDiviser(m);
      }
      while ( e != 1);
      extEuclid(m, d, e);
   }
   while (e < 0);

   private_key.a=d;
   private_key.b=n;
   public_key.a=e;
   public_key.b=n;

   PrivateKey=SaveKey(0);
   PublicKey=SaveKey(1);
}
