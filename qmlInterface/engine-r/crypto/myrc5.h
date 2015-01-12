#ifndef MYRC5_H
#define MYRC5_H

#include <QByteArray>

#define rc5Rounds       12
#define rc5KeySize      64
#define rc5KeyLength  (2 * (rc5Rounds + 1))
#define rc5P32 0xb7e15163
#define rc5Q32 0x9e3779b9

class MyRC5
{
    quint32 v_xor[2];

    quint8 v_key[rc5KeySize];
    quint8 v_keyPtr[rc5KeySize];

    quint32 v_S[rc5KeyLength];

    quint32 ROL(quint32 a, int offset);
    quint32 ROR(quint32 a, int offset);

    void Initialize(const QByteArray &Key);
    void CalculateSubKeys();
    void Extract(const QByteArray &Source, quint8 *Result, qint32 n, qint32 length);

    void EncipherBlock(quint8 *data);
    void DecipherBlock(quint8 *data);

public:
    MyRC5();

    QByteArray generate();
    void EncryptCopy(QByteArray &Dest, const QByteArray &Source, const QByteArray &Key);
    void DecryptCopy(QByteArray &Dest, const QByteArray &Source, const QByteArray &Key);
};

#endif // MYRC5_H
