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

AppDBTestViewWidget::AppDBTestViewWidget(const WineAppDBTestInfo *appinfo, QWidget *parent) : QWidget(parent)
{
		setupUi(this);
		this->setAppName(QString("%1 - %2").arg(appinfo->name).arg(appinfo->appver));
		this->setAppDesc(appinfo->desc);
		this->_APPID=appinfo->id;

		lblWineVer->setText(appinfo->winever);
		lblLicense->setText(appinfo->license);
		lblRating->setText(appinfo->rating);
		lblAddComments->setText(appinfo->comment);
		lblWhatWorks->setText(appinfo->works);
		lblWhatNotWorks->setText(appinfo->notworks);
		lblWhatWasNotTested->setText(appinfo->nottested);

		addTestResults(appinfo->tests);
		addBugs(appinfo->bugs);
		return;
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
		version = new AppDBAppVersionWidget(4, 0, bugs.at(i).id, 0);

		version->addLabel(QString("%1").arg(bugs.at(i).id), 70, 1);
		version->addLabel(bugs.at(i).desc);
		version->insertStretch();
		version->addLabel(QString("%1").arg(bugs.at(i).status), 120, 1);
		version->addLabel(QString("%1").arg(bugs.at(i).resolution), 120, 1);

		BugsLayout->addWidget(version);
	}
	return;
}

void AppDBTestViewWidget::addTestResults(QList<WineAppDBTestResult> tests){
	AppDBAppVersionWidget *version;
	QString rating_desc;

	for (int i=0; i<tests.count(); i++){
		if (tests.at(i).current){
			version = new AppDBAppVersionWidget(4, this->_APPID, tests.at(i).id, 0, false);
		} else {
			version = new AppDBAppVersionWidget(4, this->_APPID, tests.at(i).id, 0);
		}

		version->addLabel(QString(" %1").arg(tests.at(i).distrib), -1, 0, true);
		version->insertStretch();
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
		switch (tests.at(i).rating){
		case 1:
			rating_desc="Platinum";
			break;
		case 2:
			rating_desc="Gold";
			break;
		case 3:
			rating_desc="Silver";
			break;
		case 4:
			rating_desc="Bronze";
			break;
		case 5:
			rating_desc="Garbage";
			break;
		default:
			rating_desc="unexpected";
			break;
		}
		version->addLabel(rating_desc, 120, 1);

		Top5ResultsLayout->addWidget(version);
	}
}
