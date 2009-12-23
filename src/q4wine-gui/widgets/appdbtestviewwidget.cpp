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

#include "appdbtestviewwidget.h"

AppDBTestViewWidget::AppDBTestViewWidget(const WineAppDBInfo *appinfo, QWidget *parent) : QWidget(parent)
{
		setupUi(this);
		this->setAppName(QString("%1 - %2").arg(appinfo->name).arg(appinfo->appver));
		this->setAppDesc(appinfo->desc);
		this->_APPID=appinfo->id;
		this->_APPVERID=appinfo->ver_id;

		lblWineVer->setText(appinfo->winever);
		lblLicense->setText(appinfo->license);
		lblRating->setText(appinfo->rating);
		lblAddComments->setText(appinfo->comment);
		lblWhatWorks->setText(appinfo->works);
		lblWhatNotWorks->setText(appinfo->notworks);
		lblWhatWasNotTested->setText(appinfo->nottested);

		if (!appinfo->url.isEmpty()){
			AppDBLinkItemWidget *label = new AppDBLinkItemWidget(tr("Application web page"), 6);
			label->setSearchUrl(appinfo->url);
			label->setToolTip(appinfo->url);
			connect (label, SIGNAL(linkTrigged(short int, QString, int, int)), this, SIGNAL(linkTrigged(short int, QString, int, int)));
			AppDetailsLayout->addWidget(label);
		}

		addTestResults(appinfo->tests);
		addBugs(appinfo->bugs);
		addComments(appinfo->comments);
		return;
}

int AppDBTestViewWidget::selectParentCommentById(int id){
	QList<QObject*> list = this->children();
	QPalette p(palette());
	int y_pos = -1;
	// Start from 1 becouse of 0 -- is VBoxLayout
	for (int i=1; i<list.count(); i++){
		if (list.at(i)->objectName().toInt()==id){
			p.setColor(QPalette::Window, QColor(255,0,0));
			list.at(i)->setProperty("palette", p);
			y_pos = list.at(i)->property("y").toInt();
		} else {
			p.setColor(QPalette::Window, QPalette().color(QPalette::Window));
			list.at(i)->setProperty("palette", p);
		}
	}

	return y_pos;
}

void AppDBTestViewWidget::setAppName(QString name){
	lblAppName->setText(name);
	return;
}

void AppDBTestViewWidget::setAppDesc(QString desc){
	lblAppDesc->setText(desc);
	return;
}

void AppDBTestViewWidget::addBugs(QList<WineAppDBBug> bugs){
	AppDBAppVersionWidget *version;

	for (int i=0; i<bugs.count(); i++){
		version = new AppDBAppVersionWidget(7);
		version->setAppId(bugs.at(i).id);
		version->addLabel(QString("%1").arg(bugs.at(i).id), 70, 1);
		version->addLabel(bugs.at(i).desc, -1, 3, true);
		version->addLabel(bugs.at(i).status, 120, 1);
		version->addLabel(bugs.at(i).resolution, 120, 1);
		BugsLayout->addWidget(version);
		connect(version, SIGNAL(versionTrigged(short int, int, int, int)), this, SIGNAL(versionTrigged(short int, int, int, int)));
	}
	return;
}

void AppDBTestViewWidget::addComments(QList<WineAppDBComment> comments){
	AppDBCommentWidget *comment;

	for (int i=0; i<comments.count(); i++){
		comment = new AppDBCommentWidget(&comments.at(i));
		TestLayout->addWidget(comment);
		connect (comment, SIGNAL(linkTrigged(short int, QString, int, int)), this, SIGNAL(linkTrigged(short int, QString, int, int)));
	}

	return;
}

void AppDBTestViewWidget::addTestResults(QList<WineAppDBTestResult> tests){
	AppDBAppVersionWidget *version;

	for (int i=0; i<tests.count(); i++){
		version = new AppDBAppVersionWidget(4);
		version->setAppId(this->_APPID);
		version->setAppVerId(this->_APPVERID);
		version->setTestId(tests.at(i).id);

		if (tests.at(i).current){
			version->setEnabled(false);
		}

		version->addLabel(QString("%1").arg(tests.at(i).distrib), -1, 3, true);
		version->addLabel(tests.at(i).date, 120, 1);
		version->addLabel(QString("%1").arg(tests.at(i).winever), 120, 1);
		if (tests.at(i).install){
			version->addLabel(tr("Installs"), 60, 1);
		} else {
			version->addLabel(tr(""), 60, 1);
		}
		if (tests.at(i).run){
			version->addLabel(tr("Runs"), 60, 1);
		} else {
			version->addLabel(tr(""), 60, 1);
		}

		version->addLabel(tests.at(i).rating, 120, 1);

		Top5ResultsLayout->addWidget(version);
		connect(version, SIGNAL(versionTrigged(short int, int, int, int)), this, SIGNAL(versionTrigged(short int, int, int, int)));
	}
}
