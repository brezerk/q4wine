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



#include "process.h"

Process::Process (QStringList args, QString exec, QString dir, QString info, QString caption, QStringList env , QWidget * parent, Qt::WFlags f)
{
	setupUi(this);
	
	myProcess = new QProcess(parent);
	myProcess->setEnvironment(env);
	connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));
	
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_clicked()));

	lblInfo->setText(info);
	setWindowTitle(caption);

	myProcess->setWorkingDirectory (dir);

	qDebug()<<args;
	myProcess->start(exec, args);
	
	return;	
}

void Process::cmdCancel_clicked(void){
	myProcess->kill();
	reject();
	return;
}

void Process::slotFinished(int, QProcess::ExitStatus){
			if (myProcess->exitCode()!=0){
	
				QString lang;
				// Getting env LANG variable
				lang = getenv("LANG");
				lang = lang.split(".").at(1);
		
				if (lang.isNull())
					lang = "UTF8";
				
				QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
				QString string = codec->toUnicode(myProcess->readAllStandardError());
				if (!string.isEmpty()){
					QMessageBox::warning(this, tr("Error"), tr("It seems procces fail.<br><br>Error log:<br>%1").arg(string));
				} else {
					QMessageBox::warning(this, tr("Error"), tr("It seems procces fail.<br><br>Cant read STDERR message.<br>%1").arg(string));
					
				}
				reject ();
				return;
			}
	accept();
	return;
}

