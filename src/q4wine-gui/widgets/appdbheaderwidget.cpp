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

void AppDBHeaderWidget::addLink(QString info, QString url, bool enabled){
	if (contentLayout){
		AppDBLinkItemWidget *label = new AppDBLinkItemWidget(info, url, enabled);
		contentLayout->addWidget(label);
		//label->installEventFilter(this);
	}
	return;
}

void AppDBHeaderWidget::setLayout(short int direction){
	return;
}

void AppDBHeaderWidget::insertStretch(short int place){
	contentLayout->insertStretch(place);
	return;
}

void AppDBHeaderWidget::createPagesList(short int count, short int current){

	this->insertStretch(1);

	if (current>pages_len){
		addLink("<", "url");
		for (int i=current-pages_len; i<current; i++){
			addLink(QString("%1").arg(i), "url");
		}
	} else {
		for (int i=1; i<current; i++){
			addLink(QString("%1").arg(i), "url");
		}
	}

	addLink(QString("%1").arg(current), "", false);

	if (current+pages_len>count){
		for (int i=current+1; i<=count; i++){
			addLink(QString("%1").arg(i), "url");
		}
	} else {
		for (int i=current+1; i<=current+pages_len; i++){
			addLink(QString("%1").arg(i), "url");
		}
		addLink(">", "");
	}

	this->insertStretch(-1);

	this->addLabel(tr("Page %1 of %2").arg(current).arg(count));
	return;
}

void AppDBHeaderWidget::clear(){
	if (this){
		QList<QObject*> list = this->children();
		for (int i=0; i<list.count(); i++){
			list.at(i)->removeEventFilter(this);
			delete(list.at(i));
		}

		contentLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
		contentLayout->setMargin(3);
	}
	return;
}
