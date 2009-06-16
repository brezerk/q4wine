/***************************************************************************
 *   Copyright (C) 2009 by Pavel Zinin (pashazz)                              *
 *   pzinin@gmail.com                                                     *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#include "winetricks.h"

winetricks::winetricks(QString prefixName, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	core = new CoreMethods();

	this->winetricks_bin.append(QDir::homePath());
        this->winetricks_bin.append("/.config/");
        this->winetricks_bin.append(APP_SHORT_NAME);
        this->winetricks_bin.append("/winetricks");
        
	this->prefix_path = core->getPrefixPath(prefixName);
	this->console_bin = core->getSettingValue("console", "bin");
	this->console_args = core->getSettingValue("console", "args");

	setupUi(this);

	connect (cmdInstall, SIGNAL (clicked()), this, SLOT(run_winetricks()));
	connect (cmdExit, SIGNAL (clicked()), this, SLOT(accept()));
	connect (cmdInstWinetricks, SIGNAL (clicked()), this, SLOT (install_winetricks()));

        parse();
        //fill list
        QString str;
        foreach (str, names) {
            lstMain->addItem(str);
        }
}

winetricks::~winetricks()
{
	delete core;
}

void winetricks::install_winetricks() {
	downloadwinetricks ();
}

void winetricks::run_winetricks(){
        if (!QFile(this->winetricks_bin).exists()){
                QMessageBox::warning(this, tr("Error"), tr("<p>q4wine can't locate winetricks at %1 path!</p><p>The script is maintained and hosted by DanKegel at http://www.kegel.com/wine/winetricks.  You can get it from the commandline with the command:</p><p>wget http://www.kegel.com/wine/winetricks</p><p>Or use \"Install winetricks\" button.</p>").arg(this->winetricks_bin));
		return;
	}

	QStringList args;
	if (!console_args.isEmpty()){
		// If we have any conslope parametres, we gona preccess them one by one
		QStringList cons_args = console_args.split(" ");
		for (int i=0; i<cons_args.count(); i++){
			if (!cons_args.at(i).isEmpty())
				args.append(cons_args.at(i));
		}
	}

	args.append(core->getSettingValue("system", "sh"));
	args.append("-c");

	QString arg;
		arg.append("WINEPREFIX=");
		arg.append(this->prefix_path);
		arg.append(" ");
		arg.append(this->winetricks_bin);
		arg.append(" ");
		arg.append(lstMain->currentItem()->text());

	args.append(arg);

	Process *exportProcess = new Process( args, console_bin, QDir::homePath(), tr("Running winetricks..."), tr("Plz wait..."));

	if (exportProcess->exec()==QDialog::Accepted){
		return;
	}

	return;
}

void winetricks::downloadwinetricks () {
        /*
         * Downloading winetriks and installing it
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

        args.append(core->getSettingValue("system", "sh"));
	args.append("-c");
	QString arg;
		arg.append(core->getWhichOut("wget"));
                arg.append(" http://kegel.com/wine/winetricks -O ");
                arg.append(this->winetricks_bin);
                arg.append(" && ");
                arg.append(core->getWhichOut("chmod"));
                arg.append(" +x ");
                arg.append(this->winetricks_bin);

	args.append(arg);

	Process *exportProcess = new Process( args, console_bin, QDir::homePath(), tr("Downloading and installing winetricks..."), tr("Plz wait..."));

        exportProcess->exec();
        // setting help



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
QStringList args = arg.split(" ");
QString name = args.at(0);
int pos = arg.count() - name.count();
QString desc = arg.right(pos);
names.append(name);
descs.append(desc);

}
void winetricks::parse() {
    //create a Winetricks process
      QProcess *p = new QProcess (this);
    QString pargs;
    pargs.append(winetricks_bin);
    pargs.append(" --kegelfuck");

   p->start(pargs);

   p->waitForFinished();
     //get output
   QString lang;
   // Getting env LANG variable
   lang = getenv("LANG");
  lang = lang.split(".").at(1);
 if (lang.isNull())
  lang = "UTF8";
 /* Fucking code because Kegel is MORON! */
  QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
  QString wOut = codec->toUnicode(p->readAllStandardOutput());
    delete p;

    //parse now
bool isNowParse = false;
QStringList strs = wOut.split("\n");
QString str;

foreach (str, strs){
    str = str.trimmed();

  if (str == "Apps:" || str == "Pseudopackages:"){continue;}
  if (str == "Packages:") {isNowParse = true;
  continue;
  }
  if (isNowParse) {addToList(str);}


}


}




void winetricks::on_lstMain_itemClicked(QListWidgetItem* item)
{
    int i;
  for (i=0; i < descs.count() -1; ++i) {
   if (names.at(i) ==  item->text()){
       label->setText (descs.at(i));
   }
  }
}


