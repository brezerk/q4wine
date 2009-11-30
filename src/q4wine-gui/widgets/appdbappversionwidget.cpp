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

AppDBAppVersionWidget::AppDBAppVersionWidget(const short int action, const int appid, const int verid, const int testid, const bool active, QWidget *parent) : QWidget(parent)
{
	if (active){
		setCursor(Qt::PointingHandCursor);
		installEventFilter(this);
	}
	setAutoFillBackground(true);

	contentLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	contentLayout->setMargin(0);
	contentLayout->setSpacing(0);

	_ACTION=action;
	_APPID=appid;
	_VERID=verid;
	_TESTID=testid;
	_BOLD=active;
}

AppDBAppVersionWidget::~AppDBAppVersionWidget(){
	//nothig but...
}

void AppDBAppVersionWidget::addLabel(const QString text, const short int width, const short int aligment, const bool worldwarp){
	QLabel *label = new QLabel(text, this);

	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

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

	if (!_BOLD){
		QPalette p(palette());
		// Set colour
		p.setColor(QPalette::Background, QPalette().color(QPalette::Base));
		this->setPalette(p);
	}

	contentLayout->addWidget(label);
	return;
}

void AppDBAppVersionWidget:: insertStretch(void){
	contentLayout->insertStretch(-1);
}

bool AppDBAppVersionWidget::eventFilter(QObject *obj, QEvent *event){
	if (event->type()==QEvent::MouseButtonRelease){
		versionTrigged(this->_ACTION, this->_APPID, this->_VERID, this->_TESTID);
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
