/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QVBoxLayout *vboxLayout;
    QWidget *widget_7;
    QHBoxLayout *hboxLayout;
    QLabel *label_7;
    QFrame *line;
    QWidget *widget;
    QVBoxLayout *vboxLayout1;
    QTabWidget *twbGeneral;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QTextEdit *txtAuthors;
    QWidget *tab_5;
    QGridLayout *gridLayout2;
    QGroupBox *groupBox_4;
    QGridLayout *_2;
    QTextEdit *txtPAuthors;
    QWidget *tab_2;
    QGridLayout *gridLayout3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout4;
    QTextEdit *txtTranslation;
    QWidget *tab_6;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_5;
    QGridLayout *_3;
    QTextEdit *txtThanks;
    QWidget *tab_4;
    QGridLayout *gridLayout5;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout6;
    QTextEdit *txtLicense;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *cmdOk;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(585, 420);
        About->setMinimumSize(QSize(340, 370));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/q4wine.png"), QSize(), QIcon::Normal, QIcon::Off);
        About->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(About);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        widget_7 = new QWidget(About);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        QPalette palette;
        widget_7->setPalette(palette);
        widget_7->setAutoFillBackground(false);
        hboxLayout = new QHBoxLayout(widget_7);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_7 = new QLabel(widget_7);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QPalette palette1;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        label_7->setPalette(palette1);
        label_7->setAutoFillBackground(true);
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/data/info.png")));

        hboxLayout->addWidget(label_7);


        vboxLayout->addWidget(widget_7);

        line = new QFrame(About);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        widget = new QWidget(About);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout1 = new QVBoxLayout(widget);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        twbGeneral = new QTabWidget(widget);
        twbGeneral->setObjectName(QString::fromUtf8("twbGeneral"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout = new QVBoxLayout(tab_3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFrameShape(QFrame::NoFrame);
        label->setFrameShadow(QFrame::Plain);
        label->setPixmap(QPixmap(QString::fromUtf8(":/data/q4wine-128x128.png")));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        verticalSpacer_3 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        verticalSpacer_2 = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        twbGeneral->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout1 = new QGridLayout(groupBox);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        txtAuthors = new QTextEdit(groupBox);
        txtAuthors->setObjectName(QString::fromUtf8("txtAuthors"));
        txtAuthors->setReadOnly(true);
        txtAuthors->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout1->addWidget(txtAuthors, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        twbGeneral->addTab(tab, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout2 = new QGridLayout(tab_5);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        groupBox_4 = new QGroupBox(tab_5);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        _2 = new QGridLayout(groupBox_4);
        _2->setObjectName(QString::fromUtf8("_2"));
        txtPAuthors = new QTextEdit(groupBox_4);
        txtPAuthors->setObjectName(QString::fromUtf8("txtPAuthors"));
        txtPAuthors->setReadOnly(true);
        txtPAuthors->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        _2->addWidget(txtPAuthors, 0, 0, 1, 1);


        gridLayout2->addWidget(groupBox_4, 0, 0, 1, 1);

        twbGeneral->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout3 = new QGridLayout(tab_2);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout4 = new QGridLayout(groupBox_3);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        txtTranslation = new QTextEdit(groupBox_3);
        txtTranslation->setObjectName(QString::fromUtf8("txtTranslation"));
        txtTranslation->setReadOnly(true);
        txtTranslation->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout4->addWidget(txtTranslation, 0, 0, 1, 1);


        gridLayout3->addWidget(groupBox_3, 0, 0, 1, 1);

        twbGeneral->addTab(tab_2, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        gridLayout_2 = new QGridLayout(tab_6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox_5 = new QGroupBox(tab_6);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        _3 = new QGridLayout(groupBox_5);
        _3->setObjectName(QString::fromUtf8("_3"));
        txtThanks = new QTextEdit(groupBox_5);
        txtThanks->setObjectName(QString::fromUtf8("txtThanks"));
        txtThanks->setReadOnly(true);
        txtThanks->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        _3->addWidget(txtThanks, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_5, 0, 0, 1, 1);

        twbGeneral->addTab(tab_6, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout5 = new QGridLayout(tab_4);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        groupBox_2 = new QGroupBox(tab_4);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout6 = new QGridLayout(groupBox_2);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        txtLicense = new QTextEdit(groupBox_2);
        txtLicense->setObjectName(QString::fromUtf8("txtLicense"));
        txtLicense->setReadOnly(true);
        txtLicense->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        gridLayout6->addWidget(txtLicense, 0, 0, 1, 1);


        gridLayout5->addWidget(groupBox_2, 0, 0, 1, 1);

        twbGeneral->addTab(tab_4, QString());

        vboxLayout1->addWidget(twbGeneral);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);

        cmdOk = new QPushButton(widget);
        cmdOk->setObjectName(QString::fromUtf8("cmdOk"));

        hboxLayout1->addWidget(cmdOk);


        vboxLayout1->addLayout(hboxLayout1);


        vboxLayout->addWidget(widget);


        retranslateUi(About);

        twbGeneral->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About %1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label->setText(QString());
        label_2->setText(QApplication::translate("About", "Utility for wine applications and prefixes management.", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("About", "(C) 2008-2010, brezblock core team.", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_3), QApplication::translate("About", "About", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("About", "Authors", 0, QApplication::UnicodeUTF8));
        txtAuthors->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab), QApplication::translate("About", "Authors", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("About", "Authors", 0, QApplication::UnicodeUTF8));
        txtPAuthors->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_5), QApplication::translate("About", "Packages Authors", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("About", "Authors", 0, QApplication::UnicodeUTF8));
        txtTranslation->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_2), QApplication::translate("About", "Translation", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("About", "Special thanks", 0, QApplication::UnicodeUTF8));
        txtThanks->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_6), QApplication::translate("About", "Thanks", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("About", "License agreement", 0, QApplication::UnicodeUTF8));
        txtLicense->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        twbGeneral->setTabText(twbGeneral->indexOf(tab_4), QApplication::translate("About", "License agreement", 0, QApplication::UnicodeUTF8));
        cmdOk->setText(QApplication::translate("About", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
