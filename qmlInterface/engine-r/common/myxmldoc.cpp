#include "myxmldoc.h"
#include <QBuffer>
#include <QTextStream>

QString MyXmlDoc::extractNode(QString name, qint32 level, qint32 &index)
{
   QString subnode="";
   QString subindex;
   index=0;

   qint32 ifrom=0;
   qint32 ito=0;
   while(level>=0)
   {
      ifrom=name.indexOf("/",ifrom);
      if(ifrom==-1) return subnode;
      ifrom++;
      level--;
   }

   ito=name.indexOf("/",ifrom);
   if(ito<0) ito=name.size()+1;

   subnode=name.mid(ifrom,ito-ifrom);
   ifrom=subnode.indexOf("[");
   ito=subnode.indexOf("]");
   if((ifrom>0) && (ito>ifrom))
   {
      subindex=subnode.mid(ifrom+1,ito-ifrom-1);
      index=subindex.toInt();
      subnode=subnode.left(ifrom);
   }
   return subnode;
}

MyXmlTag *MyXmlDoc::getTagByName(QString name)
{
   MyXmlTag *tag=NULL;
   qint32 level, index;
   QString subnode;

   if(!m_root) return tag;

   level=0;
   while(1)
   {
      subnode=extractNode(name,level,index);
      if(subnode=="") break;
      if(tag) tag=tag->getChild(); else tag=m_root;

      while(1)
      {
         if(tag->getName()==subnode) index--;
         if(index<0) break;

         if(tag->getNext())
            tag=tag->getNext();
         else
            return NULL;
      }
      level++;
   }
   return tag;
}

QByteArray MyXmlDoc::saveTag(MyXmlTag *tag)
{
   QByteArray baDoc;
   baDoc.append("<");
   baDoc.append(tag->getName());

   MyXmlTag *child_tag=tag->getChild();

   bool haveChild=false;
   while(child_tag)
   {
      if(!child_tag->isNode())
      {
         baDoc.append(" ");
         baDoc.append(child_tag->getName());
         baDoc.append("=\"");
         baDoc.append(child_tag->getValue());
         baDoc.append("\"");

      }
      else
         haveChild=true;

      child_tag=child_tag->getNext();
   }

   if(haveChild || tag->getValue().size())
   {
      baDoc.append(">\r");
      if(tag->getValue().size())
      {
         baDoc.append(tag->getValue());
         baDoc.append("\r");
      }
      child_tag=tag->getChild();
      while(child_tag)
      {
         if(child_tag->isNode())
            baDoc.append(saveTag(child_tag));

         child_tag=child_tag->getNext();
      }
      baDoc.append("</");
      baDoc.append(tag->getName());
      baDoc.append(">\r");
   }
   else
   {
      baDoc.append(" />\r");
   }

   return baDoc;
}

QString MyXmlDoc::createString(char *buf, qint32 length)
{
   QByteArray ba;
   ba.reserve(length);
   for(qint32 i=0;i<length;i++)
      ba[i]=buf[i];

   return QString(ba);
}

MyXmlTag *MyXmlDoc::addTag2(MyXmlTag *Parent, QString Node, QString Value, bool type)
{
   MyXmlTag *tag=new MyXmlTag;
   tag->setTag(Parent,NULL,NULL,type,Node,Value);

   if(!Parent)
   {
      m_root=tag;
   }
   else
   {
      MyXmlTag *next=Parent->getChild();
      if(next)
      {
         while(next->getNext()) next=next->getNext();
         next->setNext(tag);
      }
      else
      {
         Parent->setChild(tag);
      }
   }
   return tag;
}

MyXmlDoc::MyXmlDoc()
{
   m_root=NULL;
}

MyXmlDoc::~MyXmlDoc()
{
   if(m_root) delete m_root;
}

MyXmlTag *MyXmlDoc::getRoot()
{
   return m_root;
}

QString MyXmlDoc::getTagValue(QString Node)
{
   MyXmlTag *tag=getTagByName(Node);
   if(!tag) return QString("");

   return tag->getValue();
}

qint32 MyXmlDoc::getCountChild(QString Parent, QString Node)
{
   MyXmlTag *tag=getTagByName(Parent);
   qint32 count=0;
   if(!tag) return count;

   tag=tag->getChild();
   while(tag)
   {
      if(tag->getName()==Node) count++;
      tag=tag->getNext();
   }
   return count;
}

qint32 MyXmlDoc::setTagValue(QString Node, QString Value)
{
   MyXmlTag *tag=getTagByName(Node);
   if(!tag) return 0;

   tag->setValue(Value);
   return 1;
}

MyXmlTag *MyXmlDoc::addTag(QString Parent, QString Node, QString Value, bool type)
{
   MyXmlTag *parent=getTagByName(Parent);
   if(!parent && (Parent!="")) return 0;

   MyXmlTag *tag=new MyXmlTag;
   tag->setTag(parent,NULL,NULL,type,Node,Value);

   if(!parent)
   {
      m_root=tag;
   }
   else
   {
      MyXmlTag *next=parent->getChild();
      if(next)
      {
         while(next->getNext()) next=next->getNext();
         next->setNext(tag);
      }
      else
      {
         parent->setChild(tag);
      }
   }
   return tag;
}

qint32 MyXmlDoc::delTag(QString Node)
{
   MyXmlTag *tag=getTagByName(Node);
   if(!tag)
   {
      if((Node=="") && (m_root))
      {
         delete m_root;
         return 1;
      }
      return 0;
   }

   MyXmlTag *prev_tag=tag->getParent()->getChild();

   if(tag==prev_tag)
   {
      prev_tag->getParent()->setChild(tag->getNext());
   }
   else
   {
      while(prev_tag->getNext()!=tag)
         prev_tag=prev_tag->getNext();

      prev_tag->setNext(tag->getNext());
   }

   delete tag;
   return 1;
}

#define EndOfBuf (buf_curr>=buf_end)
#define Is_Space(c) ((c==' ')||(c=='\r')||(c=='\t')||(c=='\n'))

qint32 MyXmlDoc::load(QByteArray file)
{
   QString tag_name;
   QString tag_value;

   char *buf_curr=file.data();
   char *buf_end=buf_curr+file.size();

   m_root=NULL;
   MyXmlTag *curr_tag=NULL;

   qint32 i;
   char *elementname;

   // пропуск пробелов
   while(*buf_curr!='<')
   {
      buf_curr++;
      if(EndOfBuf) return 0;
   }

   bool tag_is_open=false;
   /*************************************************************************
 *  Возможные ситуации:                                                   *
 *        1. Новый тэг <имя_тэга> (вариант без параметров в тэге)         *
 *        2. Значение тэга (если тэг открыт)                              *
 *        3. Завершение тэга </имя_тэга>                                  *
 **************************************************************************/
   while(buf_curr<buf_end)
   {
      // пропуск пробелов
      while(Is_Space(*buf_curr))
      {
         buf_curr++;
         if(EndOfBuf) break;
      }

      // пропуск версии <? ?>
      if((*buf_curr=='<') && (*(buf_curr+1)=='?'))
      {
         buf_curr++; buf_curr++;
         while((*buf_curr!='?') && (*(buf_curr+1)!='>'))
         {
            buf_curr++;
            if(EndOfBuf) return 0;
         }
         buf_curr++; buf_curr++;
         continue;
      }

      // пропуск комментариев <!--- --->
      if((*buf_curr=='<') && (*(buf_curr+1)=='!'))
      {
         buf_curr++;
         buf_curr++;
         while(*(buf_curr+1)!='>')
         {
            buf_curr++;
            if(EndOfBuf) return 0;
         }
         buf_curr++;
         continue;
      }

      // тэг открыт и не новый тэг => значение тэга
      if((tag_is_open) && (*buf_curr!='<'))
      {
         i=0;
         elementname=buf_curr;
         while(*buf_curr!='<')
         {
            i++;
            buf_curr++;
            if(EndOfBuf) return 0;
         }
         while((i>0) && Is_Space(elementname[i-1])) i--;

         tag_value=createString(elementname,i);
         curr_tag->setValue(tag_value);
         continue;
      }

      // тэг открыт и найден закрывающий тэг => закрыть тэг
      if((curr_tag) && (*buf_curr=='<') && (*(buf_curr+1)=='/'))
      {
         buf_curr++;
         buf_curr++;
         if(EndOfBuf) return 0;

         i=0;
         elementname=buf_curr;
         while((!Is_Space(*buf_curr)) && (*buf_curr!='>'))
         {
            i++;
            buf_curr++;
            if(EndOfBuf) return 0;
         }

         // ищем закрывающую скобку
         while(*buf_curr!='>')
         {
            buf_curr++;
            if(EndOfBuf) return 0;
         }
         buf_curr++;

         tag_name=createString(elementname,i);
         if(tag_name!=curr_tag->getName()) return 0;
         curr_tag=curr_tag->getParent();

         tag_is_open=false;
         continue;
      }

      // найден открывающий тэг => добавить тэг
      if((*buf_curr=='<') && (*(buf_curr+1)!='/'))
      {
         buf_curr++;
         i=0;
         elementname=buf_curr;
         while((!Is_Space(*buf_curr)) && (*buf_curr!='/') && (*buf_curr!='>'))
         {
            i++;
            buf_curr++;
            if(EndOfBuf) return 0;
         }

         // добавляем тэг
         tag_name=createString(elementname,i);
         curr_tag=addTag2(curr_tag,tag_name,"",true);
         tag_is_open=true;

         // пропуск пробелов
         while(Is_Space(*buf_curr))
         {
            buf_curr++;
            if(EndOfBuf) break;
         }

         // тэг может содержать параметры вида  имя="значение" ...
         if(*buf_curr!='>')
         {
            // циклимся по параметрам тега
            while((*buf_curr!='>') &&(*buf_curr!='/'))
            {
               // выделяем имя тэга
               i=0;
               elementname=buf_curr;
               while((!Is_Space(*buf_curr)) && (*buf_curr!='='))
               {
                  i++;
                  buf_curr++;
                  if(EndOfBuf) return 0;
               }

               // пропуск пробелов
               while(Is_Space(*buf_curr))
               {
                  buf_curr++;
                  if(EndOfBuf) break;
               }

               if(*buf_curr=='=')
               {
                  // добавляем тэг
                  tag_name=createString(elementname,i);
                  curr_tag=addTag2(curr_tag,tag_name,"",false);
                  tag_is_open=true;

                  buf_curr++;
                  // пропуск пробелов
                  while(Is_Space(*buf_curr))
                  {
                     buf_curr++;
                     if(EndOfBuf) break;
                  }

                  // значение начинается с "
                  if(*buf_curr!='\"') return 0;
                  buf_curr++;

                  // выделяем значение параметра тэга
                  i=0;
                  elementname=buf_curr;
                  while(*buf_curr!='\"')
                  {
                     i++;
                     buf_curr++;
                     if(EndOfBuf) return 0;
                  }
                  buf_curr++;
                  tag_value=createString(elementname,i);
                  curr_tag->setValue(tag_value);
                  curr_tag=curr_tag->getParent();
               } else
                  return 0;

               // пропуск пробелов
               while(Is_Space(*buf_curr))
               {
                  buf_curr++;
                  if(EndOfBuf) break;
               }
            }
         }

         // если самозакрывающийся тэг, то закрыть его
         if(*buf_curr=='/')
         {
            curr_tag=curr_tag->getParent();
            tag_is_open=false;

            // пропуск пробелов
            buf_curr++;
            while(Is_Space(*buf_curr))
            {
               buf_curr++;
               if(EndOfBuf) break;
            }
            if(*buf_curr!='>') return 0;
         }
         buf_curr++;
      }
      else
      {
         buf_curr++;
      }
   }
   return 1;
}

QByteArray MyXmlDoc::save()
{
   QByteArray baDoc;
   baDoc.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r");
   baDoc.append(saveTag(m_root));
   return baDoc;
}
