#include "mybigint.h"
#include <QtCore>
#include <QTime>


MyBigInt::MyBigInt()
{
   memset(digits,0,sizeof(digits));
}

MyBigInt::MyBigInt(const MyBigInt &obj)
{
   memcpy(digits,obj.digits,sizeof(digits));
}

MyBigInt::MyBigInt(const qint32 b)
{
   if(b<0)
      memset(digits,0xff,sizeof(digits));
   else
      memset(digits,0,sizeof(digits));

   digits[0]=b & 0xffff;
   digits[1]=(b >> 16) & 0xffff;
}

void MyBigInt::Load(char *buffer, qint32 length)
{
   if(length>2*myBigIntLength) length=2*myBigIntLength;
   memset(digits,0,sizeof(digits));
   memcpy(digits,buffer,length);
}

void MyBigInt::UpLoad(char *buffer, qint32 length)
{
   if(length>2*myBigIntLength) length=2*myBigIntLength;
   memcpy(buffer,digits,length);
}

void MyBigInt::Inv()
{
   quint32 registr = 1;
   for(int i=0;i<myBigIntLength;i++)
   {
      registr=registr+(digits[i] ^ 0xffff);
      digits[i]=registr & 0xffff;
      registr=registr >> 16;
   }

}

bool MyBigInt::isEven()
{
   return ((digits[0]&0x1)==0);
}

bool MyBigInt::isPositive()
{
   return ((digits[myBigIntLength-1] & 0x8000)==0);
}

bool MyBigInt::isZero()
{
   for(int i=0;i<myBigIntLength;i++)
      if(digits[i]!=0) return false;
   return true;
}

bool MyBigInt::isAbsBigger(const MyBigInt &obj)
{
   MyBigInt tmp1(*this), tmp2(obj);
   if(!tmp1.isPositive()) tmp1.Inv();
   if(!tmp2.isPositive()) tmp2.Inv();

   return (tmp1>tmp2);
}

qint32 MyBigInt::getBit(qint32 index)
{
   qint32 result=0;
   qint32 n=index >> 4;
   qint32 bits=(index & 0xf);

   if((n>=0) && (n<myBigIntLength))
      result= (digits[n]>>bits) & 0x1;

   return result;
}

qint32 MyBigInt::getLength()
{
   qint32 i=myBigIntLength;
   while(i>0)
   {
      i--;
      if(digits[i]) break;
   }
   quint16 n=digits[i];
   i++;
   i<<=4;
   if(n)
   {
      while((n&0x8000)==0)
      {
         i--;
         n<<=1;
      }
   }
   else
      i=0;
   return i;
}

qint32 MyBigInt::getByteLength()
{
   qint32 n=myBigIntLength-1;
   while((n>=0) && (digits[n]==0)) n--;
   n++;n<<=1;
   return n;
}

void MyBigInt::random(qint32 length, qint32 type)
{
   QTime time=QTime::currentTime();
   quint32 n=time.hour();
   n+=time.minute();
   n+=time.second();
   n+=time.msec();
   qsrand(n);

   memset(digits,0,sizeof(digits));
   quint16 maska, grand_bit;

   qint32 words=(length>>4);
   qint32 bits =length & 0xf;
   if(bits)
   {
      words++;
      maska=(0xffff >> (16-bits));
      grand_bit=0x1<<(bits-1);
   }
   else
   {
      maska=0xffff;
      grand_bit=0x8000;
   }

   if (words<= myBigIntLength)
   {
      for (qint32 i = 0; i < words; i++)
         digits[i]=qrand() & 0xffff;

      if(type)
      {
         digits[words-1]&=maska;
         digits[words-1]|=grand_bit;
      }
   }
}

MyBigInt MyBigInt::abs()
{
   MyBigInt temp(*this);
   if(temp.isPositive()) temp.Inv();
   return temp;
}

MyBigInt MyBigInt::Pow(const qint32 power)
{
   MyBigInt res(1), t(power);

   bool nzero=false;
   qint32 bit;
   for(qint32 i=16*myBigIntLength-1;i>=0;i--)
   {
      if((bit=t.getBit(i)) || nzero)
      {
         nzero=true;
         res = res * res;
         if(bit) res = res * (*this);
      }
   }
   return res;
}

MyBigInt MyBigInt::Pow(const MyBigInt power)
{
   MyBigInt res(1),t(power);

   bool nzero=false;
   qint32 bit;
   for(qint32 i=16*myBigIntLength-1;i>=0;i--)
   {
      if((bit=t.getBit(i)) || nzero)
      {
         nzero=true;
         res = res * res;
         if(bit) res = res * (*this);
      }
   }
   return res;
}

MyBigInt MyBigInt::PowMod(const MyBigInt power, const MyBigInt obj)
{
   MyBigInt res(1), t(power);

   bool no_zero=false;
   qint32 bit;
   for(qint32 i=16*myBigIntLength-1;i>=0;i--)
   {
      if((bit=t.getBit(i)) || no_zero)
      {
         no_zero=true;
         res=res * res;
         if(bit) res = res * (*this);
         res = res % obj;
      }
   }
   return res;
}

MyBigInt MyBigInt::DivMod(const MyBigInt &obj, MyBigInt &reminder)
{
   quint64 registr, mN, mNa, d1, d2;

   MyBigInt tmp1(*this), tmp2(obj), tmp3, result;

   bool sign=(tmp1.isPositive()!=tmp2.isPositive());

   if(!tmp1.isPositive()) tmp1.Inv();
   if(!tmp2.isPositive()) tmp2.Inv();

   int n=0, k=0;

   for(qint32 i=myBigIntLength-1;i>=0;i--)
   {
      if((n==0) && (tmp1.digits[i])) n=i;
      if((k==0) && (tmp2.digits[i])) k=i;
   }
   n=n-k;

   while(n>=0)
   {
      d1=((quint32)(tmp1.digits[n+k+1]<<16)+(quint32)tmp1.digits[n+k]);
      d2=(quint32)tmp2.digits[k];
      if(d2==0) break;
      mN=d1/d2;

      if(mN>0)
      {
         registr=0;
         memset(tmp3.digits,0,sizeof(digits));
         for(qint32 i=0;i<k+2;i++)
         {
            registr=registr+mN*tmp2.digits[i];
            tmp3.digits[i+n]=registr & 0xffff;
            registr=registr >> 16;
         }

         while(tmp3>tmp1)
         {
            mNa=mN;
            d1=((quint32)(tmp1.digits[n+k+1]<<16)+(quint32)tmp1.digits[n+k]);
            d2=((quint32)(tmp3.digits[n+k+1]<<16)+(quint32)tmp3.digits[n+k]);
            if(d2==0) break;
            mN=(mN*d1/d2)+1;
            if(mN>=mNa) mN=mNa-1;

            registr=0;
            memset(tmp3.digits,0,sizeof(digits));
            for(qint32 i=0;i<k+2;i++)
            {
               registr=registr+mN*tmp2.digits[i];
               tmp3.digits[i+n]=registr & 0xffff;
               registr=registr >> 16;
            }
         }
         if(d2==0) break;

         result.digits[n]=mN;
         tmp1-=tmp3;
      }
      n--;
   }

   if(sign) result.Inv();
   reminder=tmp1;
   return result;
}

MyBigInt MyBigInt::MaxDiviser(MyBigInt b)
{
   MyBigInt a(*this), c;

   while (b != 0)
   {
      c = a % b;
      a = b;
      b = c;
   }

   if(!a.isPositive()) a.Inv();
   return a;
}

quint32 MyBigInt::Mod(const qint32 a)
{
   quint32 registr=0;
   for(qint32 i=myBigIntLength-1;i>=0;i--)
   {
      registr=(registr << 16)+this->digits[i];
      registr=registr % a;
   }
   return registr;
}

MyBigInt MyBigInt::operator=(const MyBigInt &obj)
{
   if(this!=&obj) memcpy(digits,obj.digits,sizeof(digits));
   return *this;
}

MyBigInt MyBigInt::operator=(const qint32 b)
{
   this->digits[0]=(b & 0xffff);
   this->digits[1]=((b >> 16) & 0xffff);
   return *this;
}

MyBigInt MyBigInt::operator+(const MyBigInt &obj)
{
   MyBigInt temp(*this);
   quint32 registr=0;
   for(qint32 i=0;i<myBigIntLength;i++)
   {
      registr=registr+temp.digits[i]+obj.digits[i];
      temp.digits[i]=registr & 0xffff;
      registr=registr >> 16;
   }
   return temp;
}

MyBigInt MyBigInt::operator+(const qint32 b)
{
   MyBigInt temp(b);
   return (temp + *this);
}

MyBigInt MyBigInt::operator*(const MyBigInt &obj)
{
   quint32 registr;
   MyBigInt tmp1(*this), tmp2(obj);

   quint32 result[2*myBigIntLength];
   memset(result,0,sizeof(result));

   bool sign=(tmp1.isPositive()!=tmp2.isPositive());

   if(!tmp1.isPositive()) tmp1.Inv();
   if(!tmp2.isPositive()) tmp2.Inv();

   for(qint32 i=0;i<myBigIntLength;i++)
   {
      registr=0;
      if(tmp2.digits[i]!=0)
      {
         for(qint32 j=0;j<myBigIntLength;j++)
         {
            registr=registr+result[i+j]+tmp1.digits[j]*tmp2.digits[i];
            result[i+j]=registr & 0xffff;
            registr=registr >> 16;
         }
      }
   }

   for(qint32 i=0;i<myBigIntLength;i++) tmp1.digits[i]=result[i];
   tmp1.digits[myBigIntLength-1]=tmp1.digits[myBigIntLength-1]&0x7fff;
   if(sign) tmp1.Inv();
   return tmp1;
}

MyBigInt MyBigInt::operator*(const qint32 b)
{
   MyBigInt temp(b);
   return (*this * temp);
}

MyBigInt MyBigInt::operator-(const MyBigInt &obj)
{
   MyBigInt temp(obj);
   temp.Inv();
   return (temp + *this);
}

MyBigInt MyBigInt::operator-(const qint32 b)
{
   MyBigInt temp(-b);
   return (temp + *this);
}

MyBigInt MyBigInt::operator%(const MyBigInt &obj)
{
   MyBigInt res(obj), bf;
   if(!res.isZero())
   {
      res = DivMod(obj, bf);
   }
   return bf;
}

MyBigInt MyBigInt::operator%(const qint32 b)
{
   MyBigInt res, temp(b), bf;
   if(b != 0)
   {
      res = DivMod(temp, bf);
   }
   return bf;
}

MyBigInt MyBigInt::operator/(const MyBigInt &obj)
{
   MyBigInt res, bf(obj);
   if(!bf.isZero())
   {
      res = DivMod(obj, bf);
   }
   return res;
}

MyBigInt MyBigInt::operator/(const qint32 b)
{
   MyBigInt res, temp(b);
   if(b != 0)
   {
      res =*this/temp;
   }
   return res;
}

MyBigInt MyBigInt::operator+=(const MyBigInt &obj)
{
   *this = *this + obj;
   return *this;
}

MyBigInt MyBigInt::operator+=(const qint32 b)
{
   *this = *this + b;
   return *this;
}

MyBigInt MyBigInt::operator-=(const MyBigInt &obj)
{
   *this = *this - obj;
   return *this;
}

MyBigInt MyBigInt::operator-=(const qint32 b)
{
   *this = *this - b;
   return *this;
}

MyBigInt MyBigInt::operator>>(const qint32 b)
{
   MyBigInt res;
   quint32 registr;

   qint32 s=(b >> 4);
   qint32 bits=(b & 0xf);

   for(qint32 i=0;i<myBigIntLength;i++)
   {
      registr=0;
      if((i+s)<myBigIntLength) registr=digits[i+s];
      if((i+s+1)<myBigIntLength) registr=registr+(digits[i+s+1] << 16);
      res.digits[i]=(registr>>bits) & 0xffff;
   }
   return res;
}

MyBigInt MyBigInt::operator<<(const qint32 b)
{
   MyBigInt res;
   quint32 registr;

   qint32 s=(b >> 4);
   qint32 bits=16-(b & 0xf);

   for(qint32 i=0;i<myBigIntLength;i++)
   {
      registr=digits[i] << 16;
      if(i>0) registr=registr+digits[i-1];
      if((i+s)<myBigIntLength) res.digits[i+s]=(registr>>bits) & 0xffff;
   }
   return res;
}

bool MyBigInt::operator>(const MyBigInt &obj)
{
   bool result=false;
   MyBigInt tmp1(*this), tmp2(obj);

   bool sign=(tmp1.isPositive()==tmp2.isPositive());

   if(sign)
   {
      if(!tmp1.isPositive()) tmp1.Inv();
      if(!tmp2.isPositive()) tmp2.Inv();

      for(int i=myBigIntLength-1;i>=0;i--)
      {
         if(tmp1.digits[i]>tmp2.digits[i]) { result=true; break; }
         if(tmp1.digits[i]<tmp2.digits[i]) break;
      }
      if(!isPositive()) result=!result;
   }
   else
   {
      if(isPositive()) result=true;
   }
   return result;
}

bool MyBigInt::operator>(const qint32 b)
{
   MyBigInt temp(b);
   return (*this > temp);
}

bool MyBigInt::operator<(const MyBigInt &obj)
{
   MyBigInt temp(obj);
   return (temp > *this);
}

bool MyBigInt::operator<(const qint32 b)
{
   MyBigInt temp(b);
   return (temp > *this);
}

bool MyBigInt::operator==(const MyBigInt &obj)
{
   bool result=true;
   for(qint32 i=0;i<myBigIntLength;i++)
      if(digits[i]!=obj.digits[i]) { result=false; break; }
   return result;
}

bool MyBigInt::operator==(const qint32 b)
{
   MyBigInt temp(b);
   return (temp == *this);
}

bool MyBigInt::operator!=(const MyBigInt &obj)
{
   bool result=false;
   for(qint32 i=0;i<myBigIntLength;i++)
      if(digits[i]!=obj.digits[i]) { result=true; break; }
   return result;
}

bool MyBigInt::operator!=(const qint32 b)
{
   MyBigInt temp(b);
   return (temp != *this);
}

bool MyBigInt::operator>=(const MyBigInt &obj)
{
   return ((*this > obj) || (*this == obj));
}

bool MyBigInt::operator>=(const qint32 b)
{
   MyBigInt temp(b);
   return ((*this > temp) || (*this == temp));

}

bool MyBigInt::operator<=(const MyBigInt &obj)
{
   return ((*this < obj) || (*this == obj));
}

bool MyBigInt::operator<=(const qint32 b)
{
   MyBigInt temp(b);
   return ((*this < temp) || (*this == temp));
}

const char hex_digit[]={ "0123456789ABCDEF" };

void MyBigInt::FromString(QBuffer *buffer)
{
   memset(digits,0,sizeof(digits));

   char c;
   qint32 i=0, n;
   while ((i < 4*myBigIntLength) && (buffer->pos()<buffer->size()))
   {
      buffer->getChar(&c);

      n=-1;
      for(qint32 j=0;j<16;j++)
         if (c==hex_digit[j])
         {
            n=j; break;
         }

      if(n==-1) break;
      digits[i>>2]=digits[i>>2]+(n<<((i&3)<<2));
      i++;
   }
}

void MyBigInt::ToString(QBuffer *buffer)
{
   qint32 n=myBigIntLength-1;
   while(n>0)
      if(digits[n]==0) n--; else break;
   n++;

   char c;
   for(qint32 i=0;i<n;i++)
   {
      c=hex_digit[(digits[i]    )&0xf]; buffer->putChar(c);
      c=hex_digit[(digits[i]>> 4)&0xf]; buffer->putChar(c);
      c=hex_digit[(digits[i]>> 8)&0xf]; buffer->putChar(c);
      c=hex_digit[(digits[i]>>12)&0xf]; buffer->putChar(c);
   }
}

void MyBigInt::ToString10(QBuffer *buffer)
{
   MyBigInt a(*this), b(10), c;

   char c1, c2;
   while(!a.isZero())
   {
      a=a.DivMod(b,c);
      c1=c.digits[0]+'0';
      buffer->putChar(c1);
   }

   qint32 n=buffer->size();
   for(qint32 i=0;i<n/2;i++)
   {
      buffer->seek(i);
      buffer->getChar(&c1);
      buffer->seek(n-1-i);
      buffer->getChar(&c2);
      buffer->seek(i);
      buffer->putChar(c2);
      buffer->seek(n-1-i);
      buffer->putChar(c1);
   }
}
