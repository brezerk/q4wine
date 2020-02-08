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

#include "appdbscrollwidget.h"

AppDBScrollWidget::AppDBScrollWidget(QWidget *parent) : QScrollArea(parent)
{
    this->setAutoFillBackground(true);
    this->setFrameShape(QFrame::NoFrame);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setWidgetResizable(true);

    contentWidget.reset(new QWidget());
    contentLayout.reset(new QVBoxLayout(contentWidget.get()));
    contentLayout->setMargin(3);

    this->setWidget(contentWidget.get());

    return;
}

void AppDBScrollWidget::addSearchWidget(const WineAppDBInfo appinfo){
    if (contentLayout.get()){
        std::unique_ptr<AppInfoWidget> AppDBWidget(new AppInfoWidget(appinfo.name, appinfo.desc, appinfo.id, appinfo.versions));
        connect(AppDBWidget.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        contentLayout->addWidget(AppDBWidget.release());
    }
    return;
}

void AppDBScrollWidget::addTestWidget(const WineAppDBInfo appinfo){
    if (contentLayout.get()){
        std::unique_ptr<AppTestWidget> AppDBTestWidget (new AppTestWidget(appinfo));
        AppDBTestWidget->setObjectName("appViewTestWidget");
        connect(AppDBTestWidget.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        connect(AppDBTestWidget.get(), SIGNAL(scrollToPos(int)), this, SLOT(scrollToPos(int)));

        contentLayout->addWidget(AppDBTestWidget.release());
    }
    return;
}

void AppDBScrollWidget::scrollToPos(int pos){
    //FIXME: Use a signal\slot MODEL!!!!!!!!!!!!!!!!!!!
    //int y_pos = AppDBTestWidget->selectParentCommentById(id);
    if (pos>0)
        this->ensureVisible(0, pos, 0, this->height()/3);
}

void AppDBScrollWidget::clear(void){
    if (contentWidget.get()){
        QList<QObject*> list = contentWidget->children();
        // Start from 1 because of 0 -- is VBoxLayout
        for (int i=1; i<list.count(); i++){
#ifdef DEBUG
            qDebug()<<"[ii] Schedule QObject for deletetion. object type is:"<<list.at(i)->metaObject()->className();
#endif
            list.at(i)->setProperty("visible", false);
            list.at(i)->disconnect();
            list.at(i)->deleteLater();
        }
    }
    return;
}

void AppDBScrollWidget::insertStretch(void){
    if (contentLayout.get()){
        std::unique_ptr<QWidget> visibleStrech(new QWidget());
        visibleStrech->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        contentLayout->addWidget(visibleStrech.release());
    }
}

void AppDBScrollWidget::addVersionFrame(QList<WineAppDBCategory> list, QString frame_caption, short int action){
    if (list.count()<=0)
        return;

    std::unique_ptr<QGroupBox> frame(new QGroupBox(QString("%1:").arg(frame_caption)));
    std::unique_ptr<QVBoxLayout> layout(new QVBoxLayout());

    for (int i=0; i<list.count(); i++){
        std::unique_ptr<LineItemWidget> version(new LineItemWidget(action));
        version->setAppId(list.at(i).id);
        version->addLabel(list.at(i).name, 240, 3);
        version->addLabel(list.at(i).desc, -1, 3, true);
        connect(version.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        layout->addWidget(version.release());
    }

    frame->setLayout(layout.release());
    contentLayout->addWidget(frame.release());
    return;
}


