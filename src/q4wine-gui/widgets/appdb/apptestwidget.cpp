/***************************************************************************
 *   Copyright (C) 2008-2013 by Alexey S. Malakhov <brezerk@gmail.com>     *
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

#include "apptestwidget.h"

AppTestWidget::AppTestWidget(const WineAppDBInfo appinfo, QWidget *parent) : QWidget(parent)
{
        setupUi(this);
        this->setAppName(QString("%1 - %2").arg(appinfo.name).arg(appinfo.appver));
        this->setAppDesc(appinfo.desc);
        this->appid=appinfo.id;
        this->verid=appinfo.ver_id;

        lblWineVer->setText(appinfo.winever);
        lblLicense->setText(appinfo.license);
        lblRating->setText(appinfo.rating);
        lblAddComments->setText(appinfo.comment);
        lblWhatWorks->setText(appinfo.works);
        lblWhatNotWorks->setText(appinfo.notworks);
        lblWhatWasNotTested->setText(appinfo.nottested);

        if (!appinfo.url.isEmpty()){
            std::auto_ptr<LinkItemWidget> label(new LinkItemWidget("App web page", 6));
            label->setSearchUrl(appinfo.url);
            label->setToolTip(appinfo.url);
            connect (label.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
            AppDetailsLayout->addWidget(label.release());
        }

        addTestResults(appinfo.tests);
        addBugs(appinfo.bugs);
        addComments(appinfo.comments);
        return;
}

void AppTestWidget::requestParentComment(int id){
    QList<QObject*> list = this->children();
    QPalette p(palette());
    int y_pos = -1;
    // Start from 1 becouse of 0 -- is VBoxLayout
    for (int i=1; i<list.count(); i++){
        if (list.at(i)->objectName().toInt()==id){
            p.setColor(QPalette::Window, QColor(255,0,0));
            list.at(i)->setProperty("palette", p);
            y_pos = list.at(i)->property("y").toInt();
        } else {
            p.setColor(QPalette::Window, QPalette().color(QPalette::Window));
            list.at(i)->setProperty("palette", p);
        }
    }

    if (y_pos>0)
        emit(scrollToPos(y_pos));

    return;
}

void AppTestWidget::setAppName(QString name){
    lblAppName->setText(name);
    return;
}

void AppTestWidget::setAppDesc(QString desc){
    lblAppDesc->setText(desc);
    return;
}

void AppTestWidget::addBugs(QList<WineAppDBBug> bugs){
    if (bugs.count()==0)
        grpKnownBugs->setVisible(false);

    for (int i=0; i<bugs.count(); i++){
        std::auto_ptr<LineItemWidget> version(new LineItemWidget(7));
        version->setAppId(bugs.at(i).id);
        version->addLabel(QString("%1").arg(bugs.at(i).id), 70, 1);
        version->addLabel(bugs.at(i).desc, -1, 3, true);
        version->addLabel(bugs.at(i).status, 120, 1);
        version->addLabel(bugs.at(i).resolution, 120, 1);
        connect(version.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        BugsLayout->addWidget(version.release());
    }
    return;
}

void AppTestWidget::addComments(QList<WineAppDBComment> comments){
    for (int i=0; i<comments.count(); i++){
        std::auto_ptr<CommentWidget> comment (new CommentWidget(comments.at(i)));
        connect(comment.get(), SIGNAL(requestParentComment(int)), this, SLOT(requestParentComment(int)));
        TestLayout->addWidget(comment.release());
    }

    return;
}

void AppTestWidget::addTestResults(QList<WineAppDBTestResult> tests){
    if (tests.count()==0)
        grpTestResults->setVisible(false);

    for (int i=0; i<tests.count(); i++){
        std::auto_ptr<LineItemWidget> version (new LineItemWidget(4));
        version->setAppId(this->appid);
        version->setAppVerId(this->verid);
        version->setTestId(tests.at(i).id);

        if (tests.at(i).current){
            version->setEnabled(false);
        }

        version->addLabel(QString("%1").arg(tests.at(i).distrib), -1, 3, true);
        version->addLabel(tests.at(i).date, 120, 1);
        version->addLabel(QString("%1").arg(tests.at(i).winever), 120, 1);
        if (tests.at(i).install){
            version->addLabel("Installs", 60, 1);
        } else {
            version->addLabel("", 60, 1);
        }
        if (tests.at(i).run){
            version->addLabel("Runs", 60, 1);
        } else {
            version->addLabel("", 60, 1);
        }

        version->addLabel(tests.at(i).rating, 120, 1);
        connect(version.get(), SIGNAL(itemTrigged(short int, QString, int, int, int)), this, SIGNAL(itemTrigged(short int, QString, int, int, int)));
        Top5ResultsLayout->addWidget(version.release());
    }
}
