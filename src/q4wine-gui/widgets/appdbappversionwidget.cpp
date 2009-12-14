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

AppDBAppVersionWidget::AppDBAppVersionWidget(const short int action, QWidget *parent) : QWidget(parent)
{

	setCursor(Qt::PointingHandCursor);
	installEventFilter(this);
	setAutoFillBackground(true);

	contentLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	contentLayout->setMargin(0);
	contentLayout->setSpacing(0);

	this->Action=action;
	this->AppID=0;
	this->AppVerID=0;
	this->TestId=0;
}

AppDBAppVersionWidget::~AppDBAppVersionWidget(){
	//nothig but...
}

void AppDBAppVersionWidget::addLabel(const QString text, const short int width, const short int aligment, const bool worldwarp){
	QLabel *label = new QLabel(this);

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
		case 3:
		label->setAlignment(Qt::AlignJustify);
		break;
	}
//label->setAlignment(Qt::AlignLeft);

	if (worldwarp){
		label->setWordWrap(true);
	}

	if (width!=-1){
		label->setMinimumWidth(width);
		label->setMaximumWidth(width);
	}


	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	label->setText(text);
contentLayout->addWidget(label);


	return;
}

void AppDBAppVersionWidget:: insertStretch(void){
	contentLayout->insertStretch(-1);
}

bool AppDBAppVersionWidget::eventFilter(QObject *obj, QEvent *event){
	if (event->type()==QEvent::MouseButtonRelease){
		versionTrigged(this->Action, this->AppID, this->AppVerID, this->TestId);
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

void AppDBAppVersionWidget::setAction(short int action){
	this->Action=action;
	return;
}

void AppDBAppVersionWidget::setAppId(int id){
	this->AppID=id;
	return;
}

void AppDBAppVersionWidget::setAppVerId(int id){
	this->AppVerID=id;
	return;
}

void AppDBAppVersionWidget::setTestId(int id){
	this->TestId=id;
	return;
}

void AppDBAppVersionWidget::setEnabled(bool enabled){
	QPalette p(palette());
	if (!enabled){
		setCursor(Qt::ArrowCursor);
		removeEventFilter(this);
		p.setColor(QPalette::Background, QPalette().color(QPalette::Base));
	} else {
		setCursor(Qt::PointingHandCursor);
		installEventFilter(this);
		p.setColor(QPalette::Background, QPalette().color(QPalette::Button));
	}
	this->setPalette(p);

	return;
}
