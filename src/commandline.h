#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <QStringList>
#include "config,h"
class CommandLine : public QObject
{
public:
    CommandLine(QStringList  args);
    private:
    void usage();
    void ms(); //it is Easter Egg!
    void version ();
   private:
    QTextStream *qout ; //to out
};

#endif // COMMANDLINE_H
