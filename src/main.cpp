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
#include <QSqlDatabase>
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

#include "gui/mainwindow.h"

#include <stdlib.h>

#include "stdio.h"

#include "initdb.h"

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	QTranslator*  qtt = new QTranslator ( 0 );

	//QSettings settings("Brezerk GNU Soft", APP_NAME);

	QSettings settings(APP_SHORT_NAME, "default");
	
	QString i18nPath;

	i18nPath.clear();
	i18nPath.append(APP_PREF);
	i18nPath.append("/share/");
	i18nPath.append(APP_SHORT_NAME);
	i18nPath.append("/i18n");
	
	QString lang;
	
	// Getting env LANG variable
	
	
	settings.beginGroup("app");
		lang = settings.value("lang").toString();
	settings.endGroup();

	if (lang.isEmpty()){
		lang = getenv("LANG");
		lang = lang.split(".").at(0).toLower();
		lang.append(".qm");
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
	}	else {
		qDebug()<<"[EE] Can't get LANG variable, fall back to native translation ;[";
	}
/* command line args */

	QDir dir;

	QString rootConfPath;
	rootConfPath.append(QDir::homePath());
	rootConfPath.append("/.q4wine/");

	if (dir.exists(rootConfPath)){
		QMessageBox::warning(0, QObject::tr("Version outdated"), QObject::tr("Sorry, new wersion of q4wine require new files location.<br><br>You must manually delete all old files at:<br>$HOME/.q4wine<br>$HOME/.config/Brezerk\\ GNU\\ Soft<br><br>Note: new files location is:<br>~/.config/q4wine"));
		return -1;
	}

	rootConfPath.clear();
	rootConfPath.append(QDir::homePath());
	rootConfPath.append("/.config/Brezerk GNU Soft");

	if (dir.exists(rootConfPath)){
		QMessageBox::warning(0, QObject::tr("Version outdated"), QObject::tr("Sorry, new wersion of q4wine require new files location.<br><br>You must manually delete all old files at:<br>$HOME/.q4wine<br>$HOME/.config/Brezerk\\ GNU\\ Soft<br><br>Note: new files location is:<br>~/.config/q4wine"));
		return -1;
	}


	if (!settings.contains ("configure")){
			//If no key, we gona to start an First Run Wizard to setup q4wine
		Wizard *firstSetupWizard = new Wizard(1);
		if (firstSetupWizard->exec()==QDialog::Accepted){
				
			rootConfPath.clear();
			rootConfPath.append(QDir::homePath());
			rootConfPath.append("/.config/");
			rootConfPath.append(APP_SHORT_NAME);
				
			if (!dir.exists(rootConfPath)){
				if (!dir.mkdir(rootConfPath)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create root directory %1.").arg(rootConfPath));
					return -1;
				}
			}
					
			QString subDir;
				
			subDir=rootConfPath;
			subDir.append("/db");
				
			if (!dir.exists(subDir)){
				if (!dir.mkdir(subDir)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
					return -1;
				}
			}
				
			subDir=rootConfPath;
			subDir.append("/icons");
				
			if (!dir.exists(subDir)){
				if (!dir.mkdir(subDir)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
					return -1;
				}
			}
				
			subDir=rootConfPath;
			subDir.append("/prefixes");
				
			if (!dir.exists(subDir)){
				if (!dir.mkdir(subDir)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
					return -1;
				}
			}
				
			subDir=rootConfPath;
			subDir.append("/tmp");
				
			if (!dir.exists(subDir)){
				if (!dir.mkdir(subDir)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
					return -1;
				}
			}
			
			subDir=rootConfPath;
			subDir.append("/theme");
				
			if (!dir.exists(subDir)){
				if (!dir.mkdir(subDir)){
					QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create directory %1.").arg(subDir));
					return -1;
				}
			}
				
			if (!initDb()){
				QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to load database."));
				return -1;
			}
				
			if (!createDb()){
				QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to create database."));;
				return -1;
			}
			
			settings.setValue("configure", "yes");
			
			
			//Creating default prefix reccord
			QSqlQuery q;
			q.prepare("INSERT INTO prefix(id, name) VALUES(NULL, :name);");
			q.bindValue(":name", "Default");
					
			if (!q.exec()){
				#ifdef DEBUG
					qDebug()<<"WARNING: SQL error at Wizard::Scene prefix create\nINFO:\n"<<q.executedQuery()<<"\n"<<q.lastError();
				#endif
				return -1;
			}
			
		} else {
			return -1;
		}
	} else {
		if (!initDb()){
			QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("[EE] Unable to load database."));
			return -1;
		}	
	}	
	

	MainWindow mainWin;
	mainWin.show();
	
	return app.exec();
}
