#ifndef USER_H
#define USER_H

#include "abstractxmlitemobject.h"

class User : public AbstractXmlItemObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

public:
    virtual QString tagName() const;
};

#endif // USER_H
