#ifndef MYXMLDOC_H
#define MYXMLDOC_H
#include <QString>
#include <QByteArray>
#include "myxmltag.h"

class MyXmlDoc
{
   MyXmlTag *m_root;

   QString extractNode(QString name, qint32 level, qint32 &index);
   MyXmlTag *getTagByName(QString name);

   QByteArray saveTag(MyXmlTag *tag);
   QString createString(char* buf, qint32 length);
   MyXmlTag *addTag2(MyXmlTag *Parent, QString Node, QString Value, bool type);

public:
   MyXmlDoc();
   ~MyXmlDoc();

   MyXmlTag *getRoot();

   QString getTagValue(QString Node);
   qint32  getCountChild(QString Parent, QString Node);

   qint32 setTagValue(QString Node, QString Value);
   MyXmlTag *addTag(QString Parent, QString Node, QString Value, bool type);
   qint32 delTag(QString Node);

   qint32 load(QByteArray file);
   QByteArray save();
};

#endif // MYXMLDOC_H
