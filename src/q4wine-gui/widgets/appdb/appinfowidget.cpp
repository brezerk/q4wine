/***************************************************************************
 *   Copyright (C) 2008-2016 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "appinfowidget.h"

AppInfoWidget::AppInfoWidget(QString name, QString desc, const int appid, QList<WineAppDBVersionInfo> versions, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    this->setAppName(name);
    this->setAppDesc(desc);
    this->appid=appid;

    for (int i=0; i<versions.count(); i++){
        std::auto_ptr<LineItemWidget> version(new LineItemWidget(4));
        version->setAppId(appid);
        version->setAppVerId(versions.at(i).id);
        version->addLabel(versions.at(i).appver);

        version->insertStretch();
        version->addLabel(versions.at(i).rating, 120, 1);
        version->addLabel(QString("Wine: %1").arg(versions.at(i).winever), 120, 1);
        connect(version.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        AppVersionListerLayout->addWidget(version.release());
    }

    lblAppName->installEventFilter(this);
    lblAppName->setCursor(Qt::PointingHandCursor);
    lblIcon->setPixmap(CoreLib->loadPixmap("application-x-ms-dos-executable").scaled(64, 64, Qt::KeepAspectRatio));

    return;
}

AppInfoWidget::~AppInfoWidget(){
    //nothig but...
}

void AppInfoWidget::setAppName(QString name){
    //FIXME: url might point to web XML engine
    lblAppName->setText(name);
    return;
}

void AppInfoWidget::setAppDesc(QString desc){
    if (desc.length()>=255){
        lblAppDesc->setText(QString("%1...").arg(desc.left(255)));
    } else {
        lblAppDesc->setText(desc);
    }
    return;
}


bool AppInfoWidget::eventFilter(QObject *obj, QEvent *event){

    if (event->type()==QEvent::MouseButtonRelease){
        emit(itemTrigged(3, "", this->appid, 0, 0));
    }

    if (event->type()==QEvent::Enter){
        QPalette p(palette());
        // Set colour
        p.setColor(QPalette::WindowText, QPalette().color(QPalette::Highlight));
        this->lblAppName->setPalette(p);
    } else if (event->type()==QEvent::Leave){
        // Restore default color
        this->lblAppName->setPalette(QPalette());
    }
    return false;
}
