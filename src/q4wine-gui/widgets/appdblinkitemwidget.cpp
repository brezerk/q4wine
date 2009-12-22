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

#include "appdblinkitemwidget.h"

AppDBLinkItemWidget::AppDBLinkItemWidget(QString text, short int action, QWidget * parent) : QLabel(parent)
{
	this->setText(text);
	//See setAction() for details =)
	this->Action = action;

	this->setEnabled(true);

	return;
}

AppDBLinkItemWidget::~AppDBLinkItemWidget(){
	//nothing but...
}


void AppDBLinkItemWidget::setEnabled(bool enable){
	// Sets item color to QT::Link color while item is enabled.
	QPalette p(palette());

	if (enable){
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Link));
		this->installEventFilter(this);
		this->setCursor(Qt::PointingHandCursor);
	} else {
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::WindowText));
		this->removeEventFilter(this);
		this->setCursor(Qt::ArrowCursor);
	}
	this->setPalette(p);
	return;
}

void AppDBLinkItemWidget::setBold(bool enable){
	// Sets item text bold state.
	QFont font;

	font.setBold(enable);
	this->setFont(font);
}

void AppDBLinkItemWidget::setAction(short int action){
	/*** Action codes ***
	1: Search
	2: Toggle search page
	3: Show app
	4: Show app version
	5: Show category
	6: Open url
	7: Show parent comment
	*****************/
	Action=action;
	return;
}

void AppDBLinkItemWidget::setSearchUrl(QString url){
	/* Search Url
		This might be used for saving user search text
	*/
	SearchUrl=url;
	return;
}

void AppDBLinkItemWidget::setAppId(int id){
	/* App Id
		Saveing App Id
	*/
	AppId=id;
	return;
}

void AppDBLinkItemWidget::setVerId(int id){
	/* Version Id
		Saveing Version Id
	*/
	VerId=id;
	return;
}

void AppDBLinkItemWidget::setCatId(int id){
	/* Category Id
		Saveing Category Id
	*/
	CatId=id;
	return;
}

void AppDBLinkItemWidget::setPage(short int id){
	/* Category Id
		Saveing Category Id
	*/
	Page=id;
	return;
}

void AppDBLinkItemWidget::setParentId(int id){
	/* Parent comment Id
		Saveing id of parent comment
	*/
	ParentId=id;
	return;
}

bool AppDBLinkItemWidget::eventFilter(QObject *obj, QEvent *event){
	if (event->type()==QEvent::MouseButtonRelease){
		switch (Action){
  case 1:
			emit(linkTrigged(1, SearchUrl));
			break;
  case 2:
			emit(linkTrigged(2, SearchUrl, Page));
			break;
  case 3:
			emit(linkTrigged(3, "", AppId));
			break;
  case 4:
			emit(linkTrigged(4, "", AppId, VerId));
			break;
  case 5:
			emit(linkTrigged(5, "", CatId));
			break;
  case 6:
			emit(linkTrigged(6, SearchUrl));
			break;
  case 7:
			emit(linkTrigged(7, "", ParentId));
			break;
		}
	} else if (event->type()==QEvent::Enter){
		QPalette p(palette());
		// Set colour
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Highlight));
		this->setPalette(p);
	} else if (event->type()==QEvent::Leave){
		QPalette p(palette());
		// Restore default color
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Link));
		this->setPalette(p);
	}
	return false;
}
