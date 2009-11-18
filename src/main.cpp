/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

#include <QtGui>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>

#include <QTimer>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QChar>
#include <QSize>
#include <QToolBar>
#include <QIcon>
#include <QGroupBox>
#include <QLocale>

#include "mainwindow.h"

#include <stdlib.h>

#include "stdio.h"
#include <locale.h>

#include "core/database/db.h"
#include "core/database/initdb.h"

#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
	DataBase db;
	//QApplication app(argc, argv);
	QtSingleApplication app(argc, argv);
	if (app.sendMessage(QObject::tr("Only one instance of %1 can be runned at same time.").arg(APP_SHORT_NAME)))
		return 0;

	QTranslator*  qtt = new QTranslator ( 0 );
	if (qtt == NULL){
		qDebug()<<"[EE] Can't create QTranslator";
		return -1;
	}
	QSettings settings(APP_SHORT_NAME, "default");

	QString i18nPath;
	i18nPath.clear();
	i18nPath.append(APP_PREF);
	i18nPath.append("/share/");
	i18nPath.append(APP_SHORT_NAME);
	i18nPath.append("/i18n");
#ifdef DEBUG
	qDebug()<<"[ii] i18n path: "<<i18nPath;
#endif

	// Getting env LANG variable
	QString lang;
	settings.beginGroup("app");
	lang = settings.value("lang").toString();
	settings.endGroup();

	// This is hack for next QLocale bug:
	//  http://bugs.gentoo.org/150745

#ifdef DEBUG
	qDebug()<<"[ii] Config lang: "<<lang;
#endif

	if (lang.isEmpty()){
		lang = setlocale(LC_ALL, "");
#ifdef DEBUG
		qDebug()<<"[ii] LC_ALL: "<<lang;
#endif
		if (lang.isEmpty()){
			lang = setlocale(LC_MESSAGES, "");
#ifdef DEBUG
			qDebug()<<"[ii] LC_MESSAGES: "<<lang;
#endif
			if (lang.isEmpty()){
				lang = getenv("LANG");
#ifdef DEBUG
				qDebug()<<"[ii] Env LANG: "<<lang;
#endif
			}
		}

		lang = lang.split(".").at(0).toLower();
		if (lang.contains("=")){
			lang = lang.split("=").last();
		}
#ifdef DEBUG
		qDebug()<<"[ii] Lang split: "<<lang;
#endif

#ifdef DEBUG
		qDebug()<<"[ii] Lang to load: "<<lang;
#endif
	}

	if (!lang.isNull()){
		if (qtt->load(lang, i18nPath)){
			app.installTranslator( qtt );
		} else {
			qDebug()<<"[EE] Can't open user selected translation";
			if (qtt->load("en_us.qm", i18nPath)){
				app.installTranslator( qtt );
			} else {
				qDebug()<<"[EE] Can't open default translation, fall back to native translation ;[";
			}
		}
	} else {
		qDebug()<<"[EE] Can't get LANG variable, fall back to native translation ;[";
	}

	if (!settings.contains ("configure")){
		//If no key, we gona to start an First Run Wizard to setup q4wine
		Wizard *firstSetupWizard = new Wizard(1);
		if (firstSetupWizard == NULL){
			qDebug()<<"[EE] Can't create Wizard";
			return -1;
		}
		if (firstSetupWizard->exec()==QDialog::Accepted){
			QString rootConfPath;
			QDir dir;
			rootConfPath.clear();
			rootConfPath.append(QDir::homePath());
			rootConfPath.append("/.config/");
			rootConfPath.append(APP_SHORT_NAME);

			// Creating root folder
			if (!dir.exists(rootConfPath)){
				if (!dir.mkdir(rootConfPath)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create root directory %1.").arg(rootConfPath));
					return -1;
				}
			}

			// Creating sub folders
			QStringList subDirs;
			subDirs << "db" << "icons" << "prefixes" << "tmp" << "theme";

			QString subDir;

			for (int i=0; i<subDirs.size(); ++i){
				subDir=rootConfPath;
				subDir.append("/");
				subDir.append(subDirs.at(i).toLocal8Bit().constData());
				if (!dir.exists(subDir)){
					if (!dir.mkdir(subDir)){
						QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
						return -1;
					}
				}
			}

			settings.setValue("configure", "yes");
		} else {
			return -1;
		}
	}

	QTextStream Qcout(stdout);
	int result, startState=0;

	if (app.arguments().count()>1){
		if ((app.arguments().at(1)=="-version") or (app.arguments().at(1)=="-v")){
			Qcout<<QString("%1 %2").arg(APP_SHORT_NAME).arg(APP_VERS)<<endl;
			Qcout<<QString("(Copyright (C) 2008-2009, brezblock core team.")<<endl;
			Qcout<<QString("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
			Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<endl;
			Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
			Qcout<<endl;
			Qcout<<QObject::tr("Author: %1.").arg("Malakhov Alexey aka John Brezerk")<<endl;
			return 0;
		} else if ((app.arguments().at(1)=="-minimize") or (app.arguments().at(1)=="-m")) {
			startState = 1;
		} else {
			Qcout<<QObject::tr("Usage:")<<endl;
			Qcout<<QObject::tr("  %1 [KEY]...").arg(APP_SHORT_NAME)<<endl;
			Qcout<<QObject::tr("GUI utility for wine applications and prefixes management.")<<endl<<endl;
			Qcout<<QObject::tr("KEYs list:")<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -h,  --help"<<QObject::tr("display this help and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -v,  --version"<<QObject::tr("output version information and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -m,  --minimize"<<QObject::tr("minimize %1 main window on startup").arg(APP_SHORT_NAME)<<qSetFieldWidth(0)<<endl;
			Qcout<<endl;
			Qcout<<QObject::tr("Report %1 bugs to %2").arg(APP_SHORT_NAME).arg(APP_BUG_EMAIL)<<endl;
			Qcout<<QObject::tr("%1 homepage: <%2>").arg(APP_SHORT_NAME).arg(APP_WEBSITTE)<<endl;
			Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
			return 0;
		}
	}

	if (!initDb())
		return -1;

	QStringList tables;
	tables << "prefix" << "dir" << "icon" << "images" << "last_run_icon";
	if (!db.checkDb(tables))
		return -1;

	MainWindow mainWin(startState);
	mainWin.show();
	app.setActivationWindow(&mainWin);
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWin, SLOT(messageReceived(const QString&)));
	result = app.exec();
	db.close();
	return result;
}
