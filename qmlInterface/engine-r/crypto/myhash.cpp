#include "myhash.h"
#include <QFile>
#include <QDataStream>
#include <QCryptographicHash>

MyHash::MyHash(QObject *parent) :
    QThread(parent)
{
}

void MyHash::initCalc(QString FileName)
{
    m_filename=FileName;
}

void MyHash::run()
{
    QFile file;
    file.setFileName(m_filename);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    QCryptographicHash hash(QCryptographicHash::Sha1);

    hash.addData(stream.device());
    file.close();

    emit result(hash.result());
}
