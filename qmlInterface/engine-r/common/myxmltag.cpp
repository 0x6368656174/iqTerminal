#include "myxmltag.h"

MyXmlTag::MyXmlTag()
{
   m_parent=NULL;
   m_next=NULL;
   m_child=NULL;
   m_node=false;
   m_name="";
   m_value="";
}

MyXmlTag::~MyXmlTag()
{
   MyXmlTag *curr=m_child;
   MyXmlTag *child=NULL;
   while(curr)
   {
      child=curr->getNext();
      delete curr;
      curr=child;
   }
}

MyXmlTag *MyXmlTag::getParent()
{
   return m_parent;
}

MyXmlTag *MyXmlTag::getNext()
{
   return m_next;
}

MyXmlTag *MyXmlTag::getChild()
{
   return m_child;
}

bool MyXmlTag::isNode()
{
   return m_node;
}

QString MyXmlTag::getName()
{
   return m_name;
}

QString MyXmlTag::getValue()
{
   return m_value;
}

void MyXmlTag::setParent(MyXmlTag *parent)
{
   m_parent=parent;
}

void MyXmlTag::setNext(MyXmlTag *next)
{
   m_next=next;
}

void MyXmlTag::setChild(MyXmlTag *child)
{
   m_child=child;
}

void MyXmlTag::setType(bool node)
{
   m_node=node;
}

void MyXmlTag::setName(QString name)
{
   m_name=name;
}

void MyXmlTag::setValue(QString value)
{
   m_value=value;
}

void MyXmlTag::setTag(MyXmlTag *parent,
                      MyXmlTag *next,
                      MyXmlTag *child,
                      bool node,
                      QString name,
                      QString value)
{
   m_parent=parent;
   m_next=next;
   m_child=child;
   m_node=node;
   m_name=name;
   m_value=value;
}
