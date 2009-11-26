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

#include "appdbappversionwidget.h"

AppDBAppVersionWidget::AppDBAppVersionWidget(const int appid, QWidget *parent) : QWidget(parent)
{
	setCursor(Qt::PointingHandCursor);
	this->installEventFilter(this);
	this->setAutoFillBackground(true);

	contentLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	contentLayout->setMargin(0);
	contentLayout->setSpacing(0);

	this->_APPID=appid;
	//this->_VERID=versioninfo.id;
}

AppDBAppVersionWidget::~AppDBAppVersionWidget(){
#ifdef DEBUG
	qDebug()<<"[ii] AppDBAppVersionWidget deleted";
#endif
	//nothig but...
}

void AppDBAppVersionWidget::addLabel(const QString text, const short int width, const short int aligment, const bool worldwarp){
	QLabel *label = new QLabel(text, this);
	if (width!=-1){
		label->setMinimumWidth(width);
	}
	switch (aligment){
		case 0:
		label->setAlignment(Qt::AlignLeft);
		break;
		case 1:
		label->setAlignment(Qt::AlignHCenter);
		break;
		case 2:
		label->setAlignment(Qt::AlignRight);
		break;
	}
	if (worldwarp){
		label->setWordWrap(true);
	}
	contentLayout->addWidget(label);
	return;
}

void AppDBAppVersionWidget:: insertStretch(void){
	contentLayout->insertStretch(-1);
}

/*
void AppDBAppVersionWidget::setAppVersion(const QString version){
	lblAppVersion->setText(QString(" %1").arg(version));
	return;
}

void AppDBAppVersionWidget::setAppRating(const short int rating){
	switch (rating){
		case 1:
		lblAppRating->setText("Platinum");
		break;
		case 2:
		lblAppRating->setText("Gold");
		break;
		case 3:
		lblAppRating->setText("Silver");
		break;
		case 4:
		lblAppRating->setText("Bronze");
		break;
		case 5:
		lblAppRating->setText("Garbage");
		break;
		default:
		lblAppRating->setText("unexpected");
		break;
	}

	return;
}

void AppDBAppVersionWidget::setWineVersion(const QString version){
	lblWineVersion->setText(QString("Wine: %1").arg(version));
	return;
}
*/

bool AppDBAppVersionWidget::eventFilter(QObject *obj, QEvent *event){
	if (event->type()==QEvent::MouseButtonRelease){
		versionTrigged(4, this->_APPID, this->_VERID);
	}

	if (event->type()==QEvent::Enter){
		QPalette p(palette());
		// Set colour
		p.setColor(QPalette::Background, QPalette().color(QPalette::Highlight));
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::HighlightedText));
		this->setPalette(p);
	} else if (event->type()==QEvent::Leave){
		// Reset default color
		this->setPalette(QPalette());
	}
	return false;
}
