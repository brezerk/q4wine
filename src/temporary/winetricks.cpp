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
#include "ui_winetricks.h"

winetricks::winetricks(QString prefixName) : m_ui(new Ui::winetricks)
{
    core = new CoreMethods();

	this->winetricks_bin = core->getWhichOut("winetricks");
    this->prefix_path = core->getPrefixPath(prefixName);
	this->console_bin = core->getSettingValue("console", "bin");
    this->console_args = core->getSettingValue("console", "args");

	m_ui->setupUi(this);

	connect (m_ui->buttonBox, SIGNAL (accepted()), this, SLOT(onaccept()));
	connect (m_ui->buttonBox, SIGNAL (rejected()), this, SLOT(onreject()));
	connect (m_ui->btnInstWinetricks, SIGNAL (clicked()), this, SLOT (instwinetricks()));
}

winetricks::~winetricks()
{
	delete core;
	delete m_ui;
}

void winetricks::instwinetricks() {install_winetricks();}

void winetricks::install_winetricks() {
	QMessageBox msg;
	msg.setText(tr("Not implemented. Manually download winetricks from the http://kegel.com/wine/winetricks, copy it to /usr/bin, and set execution permissions"));
	msg.setWindowTitle(tr("Q4Wine"));
	msg.exec();
}

void winetricks::run_winetricks(){

	QStringList args;

	if (!console_args.isEmpty()){
		// If we have any conslope parametres, we gona preccess them one by one
		QStringList cons_args = console_args.split(" ");
		for (int i=0; i<cons_args.count(); i++){
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
	arg.append(m_ui->lstMain->currentItem()->text());

	args.append(arg);

	qDebug()<<args;

	Process *exportProcess = new Process(args, console_bin, QDir::homePath(), tr("Running winetricks..."), tr("Plz wait..."));
			
	if (exportProcess->exec()==QDialog::Accepted){
		accept();
	} else {
		reject();
	}

}

void winetricks::onaccept() {
	run_winetricks();
	this->close();
}

void winetricks::onreject() { 
	this->close();
	reject();
}

void winetricks::changeEvent(QEvent *e)
{
	switch (e->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
		break;
		default:
		break;
    }
}
