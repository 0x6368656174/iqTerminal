#include "myrc5.h"
#include <QTime>

quint32 MyRC5::ROL(quint32 a, int offset)
{
    return ((a << offset)|(a >> (32 - offset)));
}

quint32 MyRC5::ROR(quint32 a, int offset)
{
    return ((a >> offset)|(a << (32 - offset)));
}

void MyRC5::Initialize(const QByteArray &Key)
{
    memset(v_key,0,rc5KeySize);
    Extract(Key,v_key,0,(Key.size()>rc5KeySize)?rc5KeySize:Key.size());

    qint32 i=0;
    qint32 j=Key.size();
    while (j < rc5KeySize)
    {
        v_key[j]=v_key[i];
        i++;
        j++;
    }

    v_xor[0]=0;
    v_xor[1]=0;
}

void MyRC5::CalculateSubKeys()
{
    qint32 i, j, k;
    quint32 L[16];
    quint32 A, B;

    // Copy the key into L
    memcpy((void*)L,v_key, rc5KeySize);

    // Now initialize the S table
    v_S[0]= rc5P32;
    for(i= 1;i<rc5KeyLength;i++)
    {
        v_S[i]= v_S[i-1] + rc5Q32;
    }

    // Now scramble the S table with the key
    i = 0;
    j = 0;
    A = 0;
    B = 0;
    for(k=0;k<3*rc5KeyLength;k++)
    {
        A = ROL((v_S[i] + A + B), 3);
        v_S[i] = A;
        B = ROL((L[j] + A + B), (A + B));
        L[j] = B;
        i = (i + 1) % rc5KeyLength;
        j = (j + 1) % 16;
    }
}

void MyRC5::Extract(const QByteArray &Source, quint8 *Result, qint32 n, qint32 length)
{
    for(qint32 i=0;i<length;i++)
        Result[i]=Source.at(n+i);
}

void MyRC5::EncipherBlock(quint8 *data)
{
    quint32 *block=(quint32 *)data;
    block[0]=(block[0]^v_xor[0])+v_S[0];
    block[1]=(block[1]^v_xor[1])+v_S[1];

    for(int i=0;i<rc5Rounds;i++)
    {
        block[0]= ROL((block[0] ^ block[1]), block[1]) + v_S[2*i];
        block[1]= ROL((block[1] ^ block[0]), block[0]) + v_S[2*i+1];
    }

    v_xor[0]=block[0];
    v_xor[1]=block[1];
}

void MyRC5::DecipherBlock(quint8 *data)
{
    quint32 *block=(quint32 *)data;

    quint32 tmp[2];
    tmp[0]=block[0];
    tmp[1]=block[1];

    for(int i= rc5Rounds-1;i>=0;i--)
    {
        block[1] = ROR((block[1]-v_S[2*i+1]), block[0]) ^ block[0];
        block[0] = ROR((block[0]-v_S[2*i]),   block[1]) ^ block[1];
    }


    block[1]=(block[1]-v_S[1])^v_xor[1];
    block[0]=(block[0]-v_S[0])^v_xor[0];
    v_xor[0]=tmp[0];
    v_xor[1]=tmp[1];
}

MyRC5::MyRC5()
{
}

QByteArray MyRC5::generate()
{
    QTime time=QTime::currentTime();
    quint32 n=time.hour();
    n+=time.minute();
    n+=time.second();
    n+=time.msec();
    qsrand(n);

    QByteArray tmp;
    tmp.resize(64);
    for (qint32 i = 0; i < 64; i++)
       tmp[i]=qrand() & 0xff;

    return tmp;
}

void MyRC5::EncryptCopy(QByteArray &Dest, const QByteArray &Source, const QByteArray &Key)
{
    quint8 buffer[8];
    qint32 index;

    Initialize(Key);
    CalculateSubKeys();

    Dest.clear();
    Dest.reserve(Source.size());
    index=0;
    while ((index+8) <= Source.size())
    {
        Extract(Source,buffer,index,8);
        EncipherBlock(buffer);
        Dest.append((char *)buffer,8);
        index+=8;
    }

    if (index < Source.size())
    {
        Extract(Source,buffer,index,Source.size()-index);
        Dest.append((char *)buffer,Source.size()-index);
    }
}

void MyRC5::DecryptCopy(QByteArray &Dest, const QByteArray &Source, const QByteArray &Key)
{
    quint8 buffer[8];
    qint32 index;

    Initialize(Key);
    CalculateSubKeys();

    Dest.clear();
    Dest.reserve(Source.size());
    index=0;
    while ((index+8) <= Source.size())
    {
        Extract(Source,buffer,index,8);
        DecipherBlock(buffer);
        Dest.append((char *)buffer,8);
        index+=8;
    }

    if (index < Source.size())
    {
        Extract(Source,buffer,index,Source.size()-index);
        Dest.append((char *)buffer,Source.size()-index);
    }
}
