/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                              *
 *   pzinin@gmail.com                                                     *
 *   Copyright (C) 2010 by Malakhov Alexey                           *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "winetricks.h"

winetricks::winetricks(QString prefixName, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName("libq4wine-core");
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
        libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->winetricks_bin.append(QDir::homePath());
    this->winetricks_bin.append("/.config/");
    this->winetricks_bin.append(APP_SHORT_NAME);
    this->winetricks_bin.append("/winetricks");

    this->prefix_name = prefixName;
    this->console_bin = CoreLib->getSetting("console", "bin").toString();
    this->console_args = CoreLib->getSetting("console", "args", FALSE).toString();

    setupUi(this);

    connect (cmdInstall, SIGNAL (clicked()), this, SLOT(run_winetricks()));
    connect (cmdExit, SIGNAL (clicked()), this, SLOT(accept()));
    connect (cmdInstWinetricks, SIGNAL (clicked()), this, SLOT (install_winetricks()));
    connect (cmdRefresh, SIGNAL (clicked()), this, SLOT(parse()));
    connect (cbCategories, SIGNAL (currentIndexChanged(QString)), this, SLOT(parse()));
    connect(lstMain, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(lstMain_itemClicked(QListWidgetItem*)));

    lstMain->installEventFilter(this);

   // if (parse()){
     //   if (lstMain->count()<=0){
            parse();
       // }
    //}
    //fill list
}

bool winetricks::eventFilter( QObject *object, QEvent *event )
{
   //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
    if (object == lstMain)
        if (event->type() == QEvent::KeyRelease)
        {
        // if yes, we need to cast the event
        std::auto_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));
        if ((keyEvent->key()==Qt::Key_Up) || (keyEvent->key()==Qt::Key_Down))
            this->lstMain_itemClicked(lstMain->currentItem());

        keyEvent.release();
        }

    return QWidget::eventFilter(object, event);
}

void winetricks::install_winetricks() {
    lstMain->clear();
    downloadwinetricks ();
}

void winetricks::run_winetricks(){

    if (!lstMain->currentItem()){
        this->parse();
        return;
    }

#ifdef DEBUG
    qDebug()<<"[plugin] run item";
#endif

   QStringList args;
        if (!console_args.isEmpty()){
                // If we have any conslope parametres, we gona preccess them one by one
                QStringList cons_args = console_args.split(" ");
                for (int i=0; i<cons_args.count(); i++){
                        if (!cons_args.at(i).isEmpty())
                                args.append(cons_args.at(i));
                }
        }

        QString proxy_host = CoreLib->getSetting("network", "host", false).toString();

        args.append("env");

        if (!proxy_host.isEmpty()){
            args.append(QString("HTTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
            args.append(QString("FTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
        }

        QHash<QString, QString> prefix_info = db_prefix.getByName(this->prefix_name);

        args << QString("WINEPREFIX=%1").arg(prefix_info.value("path"));
        args << QString("WINEDLLPATH=%1").arg(prefix_info.value("libs"));
        args << QString("WINELOADER=%1").arg(prefix_info.value("loader"));
        args << QString("WINESERVER=%1").arg(prefix_info.value("server"));
        if (!prefix_info.value("arch").isEmpty())
            args << QString("WINEARCH=%1").arg(prefix_info.value("arch"));

        args.append(CoreLib->getWhichOut("sh"));
        args.append("-c");
        args.append(QString("%1 %2").arg(this->winetricks_bin).arg(lstMain->currentItem()->text()));

    QProcess proc;
    proc.startDetached(console_bin, args, QDir::homePath());

    return;
}

void winetricks::downloadwinetricks () {
        /*
         * Downloading winetricks and installing it
         */

    QStringList args;
    if (!console_args.isEmpty()){
        // If we have any conslope parametres, we gona preccess them one by one
        QStringList cons_args = console_args.split(" ");
        for (int i=0; i<cons_args.count(); i++){
            if (!cons_args.at(i).isEmpty())
                args.append(cons_args.at(i));
        }
    }

        QString proxy_host = CoreLib->getSetting("network", "host", false).toString();
        if (!proxy_host.isEmpty()){
            args.append("env");
            args.append(QString("HTTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
            args.append(QString("FTP_PROXY=http://%1:%2").arg(proxy_host).arg(CoreLib->getSetting("network", "port", false).toString()));
        }

        args.append(CoreLib->getWhichOut("sh"));
        args.append("-c");

        QString arg;
            arg.append(CoreLib->getWhichOut("wget"));
            arg.append(" http://kegel.com/wine/winetricks -O ");
            arg.append(this->winetricks_bin);
            arg.append(" && ");
            arg.append(CoreLib->getWhichOut("chmod"));
            arg.append(" +x ");
            arg.append(this->winetricks_bin);
        args.append(arg);

    QProcess proc;
    proc.startDetached(console_bin, args, QDir::homePath());

#ifdef DEBUG
    qDebug()<<"[plugin] download done";
#endif
    return;


}



/*
Not needed right now...
void winetricks::changeEvent(QEvent *e)
{
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
        break;
        default:
        break;
    }
}
*/

void winetricks::addToList(QString arg) {
#ifdef DEBUG
    qDebug()<<"[plugin] add to list";
#endif
QStringList args = arg.split(" ");
QString name = args.at(0);
int pos = arg.count() - name.count();
QString desc = arg.right(pos);
names.append(name);
descs.append(desc);
#ifdef DEBUG
    qDebug()<<"[plugin] add to list end";
#endif
}


bool winetricks::parse() {

    names.clear();
    descs.clear();
    this->lstMain->clear();

    this->cmdInstall->setEnabled(false);

#ifdef DEBUG
    qDebug()<<"[plugin] parsing winetricks output";
#endif

    if (!QFile(this->winetricks_bin).exists()){
            //QMessageBox::warning(this, tr("Error"), tr("<p>q4wine can't locate winetricks at %1 path!</p><p>The script is maintained and hosted by DanKegel at http://www.kegel.com/wine/winetricks.  You can get it from the commandline with the command:</p><p>wget http://www.kegel.com/wine/winetricks</p><p>Or use \"Install winetricks\" button.</p>").arg(this->winetricks_bin));
            return false;
    }

    //create a Winetricks process
    QProcess p(this);
    QString pargs;
    pargs.append(winetricks_bin);

   // if (category.isEmpty()){
     //   pargs.append(" --kegelfix");
    //} else {
        pargs.append(" ");
        pargs.append(cbCategories->currentText());
        pargs.append(" list");
   // }
qDebug()<<pargs;
   p.start(pargs);

   p.waitForFinished();
     //get output
   QString lang = CoreLib->getLocale();
#ifdef DEBUG
   qDebug()<<lang;
#endif

   /* Getting list */
  QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
  QString wOut = codec->toUnicode(p.readAllStandardOutput());

    //parse now
QStringList strs = wOut.split("\n");
QString str;

foreach (str, strs){
    if (!str.isEmpty()){
        str = str.trimmed();

        addToList(str);
    }
}

#ifdef DEBUG
    qDebug()<<"[plugin] parsing winetricks output done";
#endif


str.clear();
foreach (str, names) {
        lstMain->addItem(str);
}

#ifdef DEBUG
    qDebug()<<"[plugin] add to list done";
#endif

    if (this->lstMain->count()>0)
        this->cmdInstall->setEnabled(true);

    return true;
}




void winetricks::lstMain_itemClicked(QListWidgetItem* item)
{

    int i;
  for (i=0; i < descs.count() -1; ++i) {
   if (names.at(i) ==  item->text()){
           label->setText (descs.at(i).trimmed());
   }
  }
}


