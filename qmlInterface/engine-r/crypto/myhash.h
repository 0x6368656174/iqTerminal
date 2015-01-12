#ifndef MYHASH_H
#define MYHASH_H

#include <QThread>
#include <QString>
#include <QByteArray>

class MyHash : public QThread
{
   Q_OBJECT

   QString m_filename;

public:
   explicit MyHash(QObject *parent = 0);

   void initCalc(QString FileName);

   void run() Q_DECL_OVERRIDE;

signals:
   void result(QByteArray);

};

#endif // MYHASH_H
