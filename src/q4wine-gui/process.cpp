/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "process.h"

Process::Process (QStringList args, QString exec, QString dir, QString info, QString caption, bool showErr, QStringList env, QWidget * parent, Qt::WindowFlags f)
{
	setupUi(this);



	this->showErr=showErr;

	myProcess.reset(new QProcess(parent));
	myProcess->setEnvironment(env);
	connect(myProcess.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished(int, QProcess::ExitStatus)));
	connect(myProcess.get(), SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotError(QProcess::ProcessError)));
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

		QTextStream stdErr(stderr);
		QString lang = this->getLocale();

		QTextCodec *codec = QTextCodec::codecForName(lang.toLatin1());
		if (!codec){
			stdErr<<"[ee] Cannot setup codec for \""<<lang<<"\""<<endl;
			stdErr<<"[ee] Aborting current operation!"<<endl;
			reject();
			return;
		}

		QString string = codec->toUnicode(myProcess->readAllStandardError());

		if (!string.isEmpty()){
			QMessageBox::warning(this, tr("Error"), tr("It seems that the process failed.<br><br>Error log:<br>%1").arg(string));
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

		QTextStream stdErr(stderr);
		QString lang = this->getLocale();

		// Read STDERR with locale support
		QTextCodec *codec = QTextCodec::codecForName(lang.toLatin1());
		if (!codec){
			stdErr<<"[ee] Cannot setup codec for \""<<lang<<"\""<<endl;
			stdErr<<"[ee] Aborting current operation!"<<endl;
			reject();
			return;
		}

		QString string = codec->toUnicode(myProcess->readAllStandardError());

#ifdef DEBUG
		qDebug()<<"[ii] Process::slotFinished exitstatus:"<<exitc<<"exitcode:"<<myProcess->exitCode();
		qDebug()<<"[ii] Process::slotFinished STDERR:"<<codec->toUnicode(myProcess->readAllStandardError());
		qDebug()<<"[ii] Process::slotFinished STDOUT:"<<codec->toUnicode(myProcess->readAllStandardOutput());
#endif

		if (!string.isEmpty()){
			if ((exitc == 0) && (myProcess->exitCode() == 0)){
				if (showErr){
					QMessageBox::warning(this, tr("Output"), tr("It seems that the process exited normally.<br><br>STDERR log:<br>%1").arg(string));
				}
				accept();
                return;
			} else {
				QMessageBox::warning(this, tr("Output"), tr("It seems that the process crashed.<br><br>STDERR log:<br>%1").arg(string));
				reject();
                return;
			}
		}

	accept();

	return;
}

QString  Process::getLocale(){
	QString lang;

	// This is hack for next QLocale bug:
	//  http://bugs.gentoo.org/150745

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

#ifdef DEBUG
		qDebug()<<"[ii] Lang before split: "<<lang;
#endif

        QStringList langSplit = lang.split(".");

	if (langSplit.size()>1){
		lang = langSplit.at(1);
	} else {
		lang = "UTF-8";
	}

	if (lang.isEmpty())
		lang = "UTF-8";

        if (lang.contains(";"))
            lang = lang.split(";").first();

#ifdef DEBUG
	qDebug()<<"[ii] Locale to use: "<<lang;
#endif
	return lang;
}
