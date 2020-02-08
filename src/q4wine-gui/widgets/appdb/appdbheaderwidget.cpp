/***************************************************************************
 *   Copyright (C) 2008-2020 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "appdbheaderwidget.h"

AppDBHeaderWidget::AppDBHeaderWidget(QWidget * parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::StyledPanel);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    pages_len=5;

    contentLayout.reset(new QBoxLayout(QBoxLayout::LeftToRight, this));
    contentLayout->setMargin(3);
    return;
}

void AppDBHeaderWidget::addLabel(QString info){
    if (contentLayout.get()){
        std::unique_ptr<QLabel> label(new QLabel(info));
        contentLayout->addWidget(label.release());
    }
    return;
}

void AppDBHeaderWidget::addLink(QString text, bool enabled, short int action, QString search, int value){
    if (contentLayout.get()){

        std::unique_ptr<LinkItemWidget> label(new LinkItemWidget(text, action));
        if (!enabled)
            label->setEnabled(enabled);
        label->setSearchUrl(search);
        switch (action){
  case 3:
            label->setAppId(value);
  case 5:
            label->setCatId(value);
            break;
  default:
            label->setPage(value);
            break;
        }
        connect (label.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        contentLayout->addWidget(label.release());
    }
    return;
}

void AppDBHeaderWidget::setLayout(short int direction){
    return;
}

void AppDBHeaderWidget::insertStretch(void){
    std::unique_ptr<QWidget> visibleStrech(new QWidget());
    visibleStrech->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    contentLayout->addWidget(visibleStrech.release());
    return;
}

void AppDBHeaderWidget::createPagesList(short int count, short int current, QString search){
    this->insertStretch();

    if (current<=0)
        current=1;

    short int start_page = current - 4;
    if (start_page<=0)
        start_page=1;

    if (start_page>1)
        addLink("<", true, 2, search, start_page-1);

    short int end_page = current + 4;
    if (end_page>count){
        end_page=count;
    }

    for (int i=start_page; i<=end_page; i++) {
        if (i==current){
            addLink(QString("%1").arg(i), false);
        } else {
            addLink(QString("%1").arg(i), true, 2, search, i);
        }
    }

    if (end_page<count)
        addLink(">", true, 2, search, end_page+1);

    this->insertStretch();

    this->addLabel(tr("Page %1 of %2").arg(current).arg(count));
    return;
}

void AppDBHeaderWidget::createCategoryList(const QList<WineAppDBCategory> category){
    for (int i=0; i<category.count(); i++){
        if (category.at(i).enabled){
            this->addLink(category.at(i).name, true, 5, "", category.at(i).id);
        } else {
            this->addLabel(category.at(i).name);
        }
        if (i<category.count()-1)
            this->addLabel(">");
    }
    return;
}

void AppDBHeaderWidget::clear(){
    QList<QObject*> list = this->children();
    // Start from 1 because of 0 -- is VBoxLayout
    for (int i=1; i<list.count(); i++){
#ifdef DEBUG
        qDebug()<<"[ii] Schedule QObject for deletetion. object type is:"<<list.at(i)->metaObject()->className();
#endif
            list.at(i)->setProperty("visible", false);
            list.at(i)->disconnect();
            list.at(i)->removeEventFilter(this);
            list.at(i)->deleteLater();
    }

    return;
}

void AppDBHeaderWidget::updateFirstLabelText(QString text){
    QList<QObject*> list = this->children();
    if (list.count()>=3){
        list.at(2)->setProperty("text", text);
    }
}
