#include "infodialog.h"

InfoDialog::InfoDialog(int action, QWidget *parent, Qt::WFlags f) :  QDialog(parent, f)
{
    setupUi(this);

    this->action=action;

    switch (this->action){
    case 0:
        this->setWindowTitle(tr("Winetriks plugin notification"));
        this->txtInfo->setHtml("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
                               <html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> \
                               p, li { white-space: pre-wrap; } \
                               </style></head><body><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">winetricks</span> is a quick and dirty script to download and install various redistributable runtime libraries sometimes needed to run programs in Wine.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The script is maintained and hosted by <a href=\"http://wiki.winehq.org/DanKegel\"><span style=\" text-decoration: underline; color:#0057ae;\">DanKegel</span></a> at <a href=\"http://www.kegel.com/wine/winetricks\"><span style=\" text-decoration: underline; color:#0057ae;\">http://www.kegel.com/wine/winetricks</span></a>, you can read more here <a href=\"http://www.kegel.com/wine/winetricks\"><span style=\" text-decoration: underline; color:#0057ae;\">http://wiki.winehq.org/winetricks</span></a>.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">winetricks </span>plugin for<span style=\" font-weight:600;\"> q4wine </span>was writed by <a href=\"http://wiki.winehq.org/DanKegel\"><span style=\" text-decoration: underline; color:#0057ae;\">Pavel Zinin</span></a>.</p> \
                               <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p> \
                               <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">NOTE: q4wine</span> officially do not support <span style=\" font-weight:600;\">winetriks</span>, so repport all bugs directly to <span style=\" font-weight:600;\">winetriks</span> developers.</p></body></html>");
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
        settings.setValue("winetriksPlugin", 1);
        break;
    }
    settings.endGroup();
}

    this->accept();
}

void InfoDialog::cmdCancel_clicked(){
    this->reject();
}
