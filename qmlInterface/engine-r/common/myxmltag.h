#ifndef MYXMLTAG_H
#define MYXMLTAG_H
#include <QString>

class MyXmlTag
{
   MyXmlTag *m_parent;
   MyXmlTag *m_next;
   MyXmlTag *m_child;

   bool m_node;
   QString m_name;
   QString m_value;

public:
   MyXmlTag();
   ~MyXmlTag();

   MyXmlTag *getParent();
   MyXmlTag *getNext();
   MyXmlTag *getChild();
   bool isNode();
   QString getName();
   QString getValue();

   void setParent(MyXmlTag *parent);
   void setNext(MyXmlTag *next);
   void setChild(MyXmlTag *child);
   void setType(bool node);
   void setName(QString name);
   void setValue(QString value);
   void setTag(MyXmlTag *parent,
               MyXmlTag * next,
               MyXmlTag *child,
               bool node,
               QString name,
               QString value);
};

#endif // MYXMLTAG_H
