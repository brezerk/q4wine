/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "commentwidget.h"

CommentWidget::CommentWidget(const WineAppDBComment comment, QWidget * parent) : QFrame(parent)
{
	setupUi(this);
	if (comment.id>0)
		this->setObjectName(QString("%1").arg(comment.id));
	this->setId(comment.id);
	this->setParentId(comment.parent_id);
	setTopic(comment.topic, comment.id);
	setDate(comment.autor, comment.date);
	setMessage(comment.message);
	return;
}

void CommentWidget::setTopic(QString topic, int type){
	std::unique_ptr<LinkItemWidget> label(new LinkItemWidget(topic, 8));
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	label->setBold(true);
	label->setWordWrap(true);
	if (parentid>0){
		label->setEnabled(true);
		label->setParentId(parentid);
		connect (label.get(), SIGNAL(requestParentComment(int)), this, SIGNAL(requestParentComment(int)));
	} else {
		label->setEnabled(false);
	}

	widgetLabelLayout->addWidget(label.release());
	QPalette p(palette());

	if (type<=0){
		//FIXME: check for WARNING and HOWTO colors
		if (topic.contains("warning",  Qt::CaseInsensitive)){
			p.setColor(QPalette::Background, QColor(190, 138, 138));
		} else {
			p.setColor(QPalette::Background, QColor(144, 160, 177));
		}
	} else {
		p.setColor(QPalette::Background, QPalette().color(QPalette::Dark));
	}
	widgetLabel->setPalette(p);
	widgetLabel->setAutoFillBackground(true);
	return;
}

void CommentWidget::setDate(QString autor, QString date){
	std::unique_ptr<QLabel> label(new QLabel());
	label->setText(QString("by %1 on %2").arg(autor).arg(date));
	widgetLabelLayout->addWidget(label.release());
	return;
}

void CommentWidget::setMessage(QString message){
	lblContent->setText(message);
	return;
}

void CommentWidget::setId(int id){
	this->id=id;
	return;
}

void CommentWidget::setParentId(int id){
	this->parentid=id;
	return;
}

bool CommentWidget::isId(int id){
	if (this->id==id){
		return true;
	} else {
		return false;
	}
}

