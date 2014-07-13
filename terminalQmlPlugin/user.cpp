#include "user.h"

User::User(QObject *parent) :
    AbstractXmlItemObject(parent)
{
}

QString User::tagName() const
{
    return "user";
}
