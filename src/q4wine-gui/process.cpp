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

Process::Process (QStringList args, QString exec, QString dir, QString info, QString caption, bool showErr, QStringList env, QWidget * parent, Qt::WFlags f)
{
	setupUi(this);

	this->showErr=showErr;

	myProcess = new QProcess(parent);
	myProcess->setEnvironment(env);
	connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));
	connect(myProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotError(QProcess::ProcessError)));
	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_clicked()));

	lblInfo->setText(info);
	setWindowTitle(caption);

	myProcess->setWorkingDirectory (dir);
	myProcess->start(exec, args);

#ifdef DEBUG
	qDebug()<<"[ii] Process args: "<<exec<<args;
#endif

	return;
}

void Process::cmdCancel_clicked(void){
	myProcess->kill();
	reject();
	return;
}

void Process::slotError(QProcess::ProcessError err){
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
			switch (err){
			case 0:
				QMessageBox::warning(this, tr("Error"), tr("Process: The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program."));
				break;
			case 1:
				QMessageBox::warning(this, tr("Error"), tr("Process: The process crashed some time after starting successfully."));
				break;
			case 2:
				QMessageBox::warning(this, tr("Error"), tr("Process: The last waitFor...() function timed out."));
				break;
			case 3:
				QMessageBox::warning(this, tr("Error"), tr("Process: An error occurred when attempting to read from the process. For example, the process may not be running."));
				break;
			case 4:
				QMessageBox::warning(this, tr("Error"), tr("Process: An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."));
				break;
			case 5:
				QMessageBox::warning(this, tr("Error"), tr("Process: An unknown error occurred. This is the default return value of error()."));
				break;
			}
		}
	} else {
		switch (err){
			case 0:
			QMessageBox::warning(this, tr("Error"), tr("Process: The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program."));
			break;
			case 1:
			QMessageBox::warning(this, tr("Error"), tr("Process: The process crashed some time after starting successfully."));
			break;
			case 2:
			QMessageBox::warning(this, tr("Error"), tr("Process: The last waitFor...() function timed out."));
			break;
			case 3:
			QMessageBox::warning(this, tr("Error"), tr("Process: An error occurred when attempting to read from the process. For example, the process may not be running."));
			break;
			case 4:
			QMessageBox::warning(this, tr("Error"), tr("Process: An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."));
			break;
			case 5:
			QMessageBox::warning(this, tr("Error"), tr("Process: An unknown error occurred. This is the default return value of error()."));
			break;
		}
		reject ();
	}
	return;
}

void Process::slotFinished(int, QProcess::ExitStatus exitc){
	/*
	 * This piece of code try to catch error messages.
	 * In fact exitCode() not always can return error code
	 * (for example kdesu\gksu)
	 * So the beast way is to inform user about troubles is to show to him any STDERR messages.
	 */

	if (showErr){
		QString lang;
		lang = getenv("LANG");
		lang = lang.split(".").at(1);

		// If in is empty -- set UTF8 locale
		if (lang.isEmpty())
			lang = "UTF8";

		// Read STDERR with locale support
		QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
		QString string = codec->toUnicode(myProcess->readAllStandardError());

#ifdef DEBUG
		qDebug()<<"[ii] Process::slotFinished STDERR: "<<codec->toUnicode(myProcess->readAllStandardError());
		qDebug()<<"[ii] Process::slotFinished STDOUT: "<<codec->toUnicode(myProcess->readAllStandardOutput());
#endif

		if (!string.isEmpty()){
			if ((exitc == 0) && (myProcess->exitCode() == 0)){
				QMessageBox::warning(this, tr("Output"), tr("It seems the process exited normally.<br><br>STDERR log:<br>%1").arg(string));
				accept();
			} else {
				QMessageBox::warning(this, tr("Output"), tr("It seems the process crashed.<br><br>STDERR log:<br>%1").arg(string));
				reject();
			}
		}
	}
	accept();

	return;
}



