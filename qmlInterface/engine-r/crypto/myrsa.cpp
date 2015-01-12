#include "myrsa.h"

MyRSA::MyRSA()
{
    private_key.a=0;
    private_key.b=0;

    public_key.a=0;
    public_key.b=0;
}

void MyRSA::LoadKey(QByteArray *key, qint32 typeKey)
{
    MyCryptoKey* v_key=NULL;

    switch(typeKey)
    {
        case 0: //  private key
            v_key=&private_key;
            break;
        case 1: //  public key;
            v_key=&public_key;
            break;
    }

    if(v_key)
    {
        QBuffer buffer;
        buffer.setBuffer(key);
        buffer.open(QBuffer::ReadWrite);
        v_key->a.FromString(&buffer);
        v_key->b.FromString(&buffer);
        buffer.close();
    }
}

void MyRSA::Encode(QBuffer *bufferIn, QBuffer *bufferOut)
{
    qint32 l;
    qint32 n=public_key.b.getByteLength()-4;
    if(n<=0) return;

    char bf[2*myBigIntLength];

    bufferIn->seek(0);
    bufferIn->read(bf,4);
    bufferOut->write(bf,4);

    qint32 size=bufferIn->size()-4;
    MyBigInt a,b;
    while(size>0)
    {
        memset(bf,0,sizeof(bf));
        l=(n<size)?n:size;
        bufferIn->read(bf,n);
        bf[n]=l&0xff;
        bf[n+1]=(l>>16)&0xff;
        bf[n+2]=0;
        bf[n+3]=0;

        a.Load(bf,n+4);
        b=a.PowMod(public_key.a,public_key.b);
        b.UpLoad(bf,n+4);
        bufferOut->write(bf,n+4);
        size-=l;
    }
}

void MyRSA::Decode(QBuffer *bufferIn, QBuffer *bufferOut)
{
    qint32 l, m;
    qint32 n=private_key.b.getByteLength();
    if(n<=0) return;

    char bf[2*myBigIntLength];

    bufferIn->seek(0);
    bufferIn->seek(0);
    bufferIn->read(bf,4);
    bufferOut->write(bf,4);

    qint32 size=bufferIn->size()-4;
    MyBigInt a,b;
    while(size>0)
    {
        memset(bf,0,sizeof(bf));
        l=(n<size)?n:size;
        bufferIn->read(bf,n);

        a.Load(bf,n);
        b=a.PowMod(private_key.a,private_key.b);
        b.UpLoad(bf,n);
        m=bf[n-4] | (bf[n-3]<<16);
        if((m<1) || (m>(n-4))) m=n-4;
        bufferOut->write(bf,m);
        size-=l;
    }
}

qint32 MyRSA::getPrivateKeyLength()
{
    return private_key.b.getLength();
}

qint32 MyRSA::getPublicKeyLength()
{
    return public_key.b.getLength();
}
