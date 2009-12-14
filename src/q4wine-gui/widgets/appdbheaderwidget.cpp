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

#include "appdbheaderwidget.h"

AppDBHeaderWidget::AppDBHeaderWidget(QWidget * parent) : QFrame(parent)
{
	this->setFrameShape(QFrame::StyledPanel);
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

	pages_len=5;

	contentLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	contentLayout->setMargin(3);

	return;
}

void AppDBHeaderWidget::addLabel(QString info){
	if (contentLayout){
		QLabel *label = new QLabel(info);
		contentLayout->addWidget(label);
	}
	return;
}

void AppDBHeaderWidget::addLink(QString text, bool enabled, short int action, QString search, int value){
	if (contentLayout){
		//
		AppDBLinkItemWidget *label = new AppDBLinkItemWidget(text, action);
		if (!enabled)
			label->setEnabled(enabled);
		label->setSearchUrl(search);
		label->setPage(value);

		contentLayout->addWidget(label);
		connect (label, SIGNAL(linkTrigged(short int, QString, int)), this, SIGNAL(linkTrigged(short int, QString, int)));
		//label->installEventFilter(this);
	}
	return;
}

void AppDBHeaderWidget::setLayout(short int direction){
	return;
}

void AppDBHeaderWidget::insertStretch(void){
	//contentLayout->insertStretch(place);
	QWidget *visibleStrech = new QWidget();
	visibleStrech->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	contentLayout->addWidget(visibleStrech);
	return;
}

void AppDBHeaderWidget::createPagesList(short int count, short int current, QString search){
	this->insertStretch();

	short int start_page = current - 4;
	if (start_page<=0)
		start_page=1;

	short int end_page = current + 4;
	if (end_page>count)
		end_page=count;

	for (int i=start_page; i<=end_page; i++) {
		if (i==current){
			addLink(QString("%1").arg(i), false);
		} else {
			addLink(QString("%1").arg(i), true, 2, search, i);
		}
	}
	this->insertStretch();

	this->addLabel(tr("Page %1 of %2").arg(current).arg(count));
	return;
}

void AppDBHeaderWidget::createCategoryList(const QList<WineAppDBCategory> *category){
	for (int i=0; i<category->count(); i++){
		this->addLink(category->at(i).name, true, 3);
		this->addLabel(">");
	}
	return;
}

void AppDBHeaderWidget::clear(){
	QList<QObject*> list = this->children();
	// Start from 1 becouse of 0 -- is VBoxLayout
	for (int i=1; i<list.count(); i++){
#ifdef DEBUG
		qDebug()<<"[ii] Shedule QObject for deletetion. object type is:"<<list.at(i)->metaObject()->className();
#endif
			list.at(i)->setProperty("visible", false);
			list.at(i)->disconnect();
			list.at(i)->removeEventFilter(this);
			list.at(i)->deleteLater();
	}

	return;
}
