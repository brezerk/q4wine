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

#include "infodialog.h"

InfoDialog::InfoDialog(int action, QWidget *parent, Qt::WFlags f) :  QDialog(parent, f)
{
    setupUi(this);

    this->action=action;

    switch (this->action){
    case 0:
        this->setWindowTitle(tr("Winetricks plugin notification"));
        this->txtInfo->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
                               <html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> \
                               p, li { white-space: pre-wrap; } \
                               </style></head><body><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">winetricks</span> is a quick and dirty script to download and install various redistributable runtime libraries sometimes needed to run programs in Wine.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The script is maintained and hosted by <a href=\"http://wiki.winehq.org/DanKegel\"><span style=\" text-decoration: underline; color:#0057ae;\">DanKegel</span></a> at <a href=\"http://www.kegel.com/wine/winetricks\"><span style=\" text-decoration: underline; color:#0057ae;\">http://www.kegel.com/wine/winetricks</span></a>, you can read more here <a href=\"http://www.kegel.com/wine/winetricks\"><span style=\" text-decoration: underline; color:#0057ae;\">http://wiki.winehq.org/winetricks</span></a>.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The <span style=\" font-weight:600;\">winetricks </span>plugin for<span style=\" font-weight:600;\"> Q4Wine </span>was written by <a href=\"http://wiki.winehq.org/DanKegel\"><span style=\" text-decoration: underline; color:#0057ae;\">Pavel Zinin</span></a>.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">NOTE: Q4Wine</span> officially does not support <span style=\" font-weight:600;\">winetricks</span>, so report all bugs directly to the developers of <span style=\" font-weight:600;\">winetricks</span>.</p></body></html>");
        break;
    case 1:
        this->setWindowTitle(tr("AppDB browser notification"));
        this->txtInfo->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
                               <html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> \
                               p, li { white-space: pre-wrap; } \
                               </style></head><body><p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">AppDB browser</span> allows you to browse <a href=\"http://appdb.winehq.org/\"><span style=\" text-decoration: underline; color:#0057ae;\">wine AppDB database</span></a>.</p> \
                               <p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Note</span>: Because we have <span style=\" color:#aa0000;\">no direct access</span> to AppDB database, we cannot use it actual copy.</p> \
                               <p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Note</span>: In case you want to add my xmlexport code to AppDB engine, I can provide information and support, just feel free to contact <a href=\"mailto:brezerk@gmail.com\"><span style=\" text-decoration: underline; color:#0057ae;\">me</span></a>.</p></body></html>");
        break;
    }

    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_clicked()));
    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_clicked()));
}

void InfoDialog::cmdOk_clicked(){
    if (cbDontShow->isChecked()){
        QSettings settings(APP_SHORT_NAME, "default");
                settings.beginGroup("DialogFlags");
    switch(this->action){
    case 0:
        settings.setValue("winetricksPlugin", 1);
        break;
    case 1:
        settings.setValue("appdbBrowser", 1);
        break;
    }
    settings.endGroup();
}

    this->accept();
}

void InfoDialog::cmdCancel_clicked(){
    this->reject();
}
