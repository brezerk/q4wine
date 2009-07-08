#ifndef WISITEM_H
#define WISITEM_H

#include <QObject>


class WisItem  {
    public:
    WisItem ();
    QString name; // name of script
    QString author; // author
    QString description;
    QString site; //site
    QString contact; //email, jabber, etc
    QString download; //where to download?
    QString install; //UNIX path, where to install
    bool warning;
    bool error;
};

#endif // WISITEM_H
