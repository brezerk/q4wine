#include "commandline.h"

CommandLine::CommandLine(QStringList  args){
/* args is command-line argumens */
    qout = new QTextStream (stdout);
    qout << "Checking arguments...";
        args.removeFirst(); //remove program name
        if (args.isEmpty()) {
            qDebug << tr ("no command-line arguments");
            return;
        }
foreach (QString str, args)
        {
    switch (str)
     case "-v":
            version();
break;
case "-m":
	
    break;
    //other arg-s
    return;


}
}


CommandLine::version() {
    QString message;
    message = APP_NAME + " version " + APP_VERS + "\n" + "Installed in " + APP_PREF + "\n Platform: " + APP_REALM + "\n Copyright (c) Malakhov Alexey, Pavel Zinin, 2009, GPLv2";
    qout << message;
}
