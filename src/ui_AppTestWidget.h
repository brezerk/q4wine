/********************************************************************************
** Form generated from reading UI file 'AppTestWidget.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPTESTWIDGET_H
#define UI_APPTESTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppTestWidget
{
public:
    QVBoxLayout *TestLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lblAppName;
    QSpacerItem *horizontalSpacer;
    QLabel *lblAppDesc;
    QWidget *widget;
    QHBoxLayout *AppDetailsLayout;
    QLabel *label_4;
    QLabel *lblLicense;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *grpResults;
    QVBoxLayout *verticalLayout;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_10;
    QLabel *lblWineVer;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_11;
    QLabel *lblRating;
    QLabel *lblHeader0;
    QLabel *lblWhatWorks;
    QLabel *lblHeader1;
    QLabel *lblWhatNotWorks;
    QLabel *lblHeader2;
    QLabel *lblWhatWasNotTested;
    QLabel *lblHeader3;
    QLabel *lblAddComments;
    QGroupBox *grpTestResults;
    QGridLayout *Top5ResultsLayout;
    QGroupBox *grpKnownBugs;
    QGridLayout *BugsLayout;

    void setupUi(QWidget *AppTestWidget)
    {
        if (AppTestWidget->objectName().isEmpty())
            AppTestWidget->setObjectName(QString::fromUtf8("AppTestWidget"));
        AppTestWidget->resize(604, 410);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AppTestWidget->sizePolicy().hasHeightForWidth());
        AppTestWidget->setSizePolicy(sizePolicy);
        TestLayout = new QVBoxLayout(AppTestWidget);
        TestLayout->setObjectName(QString::fromUtf8("TestLayout"));
        widget_2 = new QWidget(AppTestWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lblAppName = new QLabel(widget_2);
        lblAppName->setObjectName(QString::fromUtf8("lblAppName"));
        sizePolicy.setHeightForWidth(lblAppName->sizePolicy().hasHeightForWidth());
        lblAppName->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        lblAppName->setFont(font);
        lblAppName->setTextFormat(Qt::PlainText);
        lblAppName->setScaledContents(false);
        lblAppName->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(lblAppName);

        horizontalSpacer = new QSpacerItem(387, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        TestLayout->addWidget(widget_2);

        lblAppDesc = new QLabel(AppTestWidget);
        lblAppDesc->setObjectName(QString::fromUtf8("lblAppDesc"));
        lblAppDesc->setWordWrap(true);
        lblAppDesc->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        TestLayout->addWidget(lblAppDesc);

        widget = new QWidget(AppTestWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        AppDetailsLayout = new QHBoxLayout(widget);
        AppDetailsLayout->setSpacing(2);
        AppDetailsLayout->setContentsMargins(0, 0, 0, 0);
        AppDetailsLayout->setObjectName(QString::fromUtf8("AppDetailsLayout"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        AppDetailsLayout->addWidget(label_4);

        lblLicense = new QLabel(widget);
        lblLicense->setObjectName(QString::fromUtf8("lblLicense"));
        sizePolicy1.setHeightForWidth(lblLicense->sizePolicy().hasHeightForWidth());
        lblLicense->setSizePolicy(sizePolicy1);

        AppDetailsLayout->addWidget(lblLicense);

        horizontalSpacer_3 = new QSpacerItem(93, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        AppDetailsLayout->addItem(horizontalSpacer_3);


        TestLayout->addWidget(widget);

        grpResults = new QGroupBox(AppTestWidget);
        grpResults->setObjectName(QString::fromUtf8("grpResults"));
        sizePolicy.setHeightForWidth(grpResults->sizePolicy().hasHeightForWidth());
        grpResults->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(grpResults);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget_3 = new QWidget(grpResults);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_10 = new QLabel(widget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy1.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_10);

        lblWineVer = new QLabel(widget_3);
        lblWineVer->setObjectName(QString::fromUtf8("lblWineVer"));
        sizePolicy1.setHeightForWidth(lblWineVer->sizePolicy().hasHeightForWidth());
        lblWineVer->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(lblWineVer);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        label_11 = new QLabel(widget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_11);

        lblRating = new QLabel(widget_3);
        lblRating->setObjectName(QString::fromUtf8("lblRating"));
        sizePolicy1.setHeightForWidth(lblRating->sizePolicy().hasHeightForWidth());
        lblRating->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(lblRating);


        verticalLayout->addWidget(widget_3);

        lblHeader0 = new QLabel(grpResults);
        lblHeader0->setObjectName(QString::fromUtf8("lblHeader0"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        lblHeader0->setFont(font1);
        lblHeader0->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblHeader0);

        lblWhatWorks = new QLabel(grpResults);
        lblWhatWorks->setObjectName(QString::fromUtf8("lblWhatWorks"));
        sizePolicy.setHeightForWidth(lblWhatWorks->sizePolicy().hasHeightForWidth());
        lblWhatWorks->setSizePolicy(sizePolicy);
        lblWhatWorks->setWordWrap(true);
        lblWhatWorks->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblWhatWorks);

        lblHeader1 = new QLabel(grpResults);
        lblHeader1->setObjectName(QString::fromUtf8("lblHeader1"));
        lblHeader1->setFont(font1);
        lblHeader1->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblHeader1);

        lblWhatNotWorks = new QLabel(grpResults);
        lblWhatNotWorks->setObjectName(QString::fromUtf8("lblWhatNotWorks"));
        sizePolicy.setHeightForWidth(lblWhatNotWorks->sizePolicy().hasHeightForWidth());
        lblWhatNotWorks->setSizePolicy(sizePolicy);
        lblWhatNotWorks->setWordWrap(true);
        lblWhatNotWorks->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblWhatNotWorks);

        lblHeader2 = new QLabel(grpResults);
        lblHeader2->setObjectName(QString::fromUtf8("lblHeader2"));
        lblHeader2->setFont(font1);
        lblHeader2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblHeader2);

        lblWhatWasNotTested = new QLabel(grpResults);
        lblWhatWasNotTested->setObjectName(QString::fromUtf8("lblWhatWasNotTested"));
        sizePolicy.setHeightForWidth(lblWhatWasNotTested->sizePolicy().hasHeightForWidth());
        lblWhatWasNotTested->setSizePolicy(sizePolicy);
        lblWhatWasNotTested->setWordWrap(true);
        lblWhatWasNotTested->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblWhatWasNotTested);

        lblHeader3 = new QLabel(grpResults);
        lblHeader3->setObjectName(QString::fromUtf8("lblHeader3"));
        lblHeader3->setFont(font1);
        lblHeader3->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblHeader3);

        lblAddComments = new QLabel(grpResults);
        lblAddComments->setObjectName(QString::fromUtf8("lblAddComments"));
        sizePolicy.setHeightForWidth(lblAddComments->sizePolicy().hasHeightForWidth());
        lblAddComments->setSizePolicy(sizePolicy);
        lblAddComments->setWordWrap(true);
        lblAddComments->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblAddComments);


        TestLayout->addWidget(grpResults);

        grpTestResults = new QGroupBox(AppTestWidget);
        grpTestResults->setObjectName(QString::fromUtf8("grpTestResults"));
        sizePolicy.setHeightForWidth(grpTestResults->sizePolicy().hasHeightForWidth());
        grpTestResults->setSizePolicy(sizePolicy);
        Top5ResultsLayout = new QGridLayout(grpTestResults);
        Top5ResultsLayout->setSpacing(0);
        Top5ResultsLayout->setContentsMargins(0, 0, 0, 0);
        Top5ResultsLayout->setObjectName(QString::fromUtf8("Top5ResultsLayout"));

        TestLayout->addWidget(grpTestResults);

        grpKnownBugs = new QGroupBox(AppTestWidget);
        grpKnownBugs->setObjectName(QString::fromUtf8("grpKnownBugs"));
        sizePolicy.setHeightForWidth(grpKnownBugs->sizePolicy().hasHeightForWidth());
        grpKnownBugs->setSizePolicy(sizePolicy);
        BugsLayout = new QGridLayout(grpKnownBugs);
        BugsLayout->setSpacing(0);
        BugsLayout->setContentsMargins(0, 0, 0, 0);
        BugsLayout->setObjectName(QString::fromUtf8("BugsLayout"));

        TestLayout->addWidget(grpKnownBugs);


        retranslateUi(AppTestWidget);

        QMetaObject::connectSlotsByName(AppTestWidget);
    } // setupUi

    void retranslateUi(QWidget *AppTestWidget)
    {
        AppTestWidget->setWindowTitle(QApplication::translate("AppTestWidget", "Form", 0, QApplication::UnicodeUTF8));
        lblAppName->setText(QApplication::translate("AppTestWidget", "Some App Name - retail Expanstion on 5 dvd v  3.1", 0, QApplication::UnicodeUTF8));
        lblAppDesc->setText(QApplication::translate("AppTestWidget", "Some App Name description which might be fulland contain html. may be?", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AppTestWidget", "License:", 0, QApplication::UnicodeUTF8));
        lblLicense->setText(QApplication::translate("AppTestWidget", "EULA", 0, QApplication::UnicodeUTF8));
        grpResults->setTitle(QApplication::translate("AppTestWidget", "Selected test results:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("AppTestWidget", "Wine:", 0, QApplication::UnicodeUTF8));
        lblWineVer->setText(QApplication::translate("AppTestWidget", "3.1.14", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("AppTestWidget", "Rating:", 0, QApplication::UnicodeUTF8));
        lblRating->setText(QApplication::translate("AppTestWidget", "Gold", 0, QApplication::UnicodeUTF8));
        lblHeader0->setText(QApplication::translate("AppTestWidget", "What works:", 0, QApplication::UnicodeUTF8));
        lblWhatWorks->setText(QApplication::translate("AppTestWidget", "The game itself installs and runs fine.  I mounted the ISO and used winecfg to abstract that loopback mount as a drive.  I had to use both the SMP fix as well as the network patch.", 0, QApplication::UnicodeUTF8));
        lblHeader1->setText(QApplication::translate("AppTestWidget", "What does not:", 0, QApplication::UnicodeUTF8));
        lblWhatNotWorks->setText(QApplication::translate("AppTestWidget", "Edge detection of the mouse is an issue.   In the game to scroll the screen you are suppose to move the mouse to the edge of it. owever it's very easy to move off the screen  which causes the screen not to scroll.", 0, QApplication::UnicodeUTF8));
        lblHeader2->setText(QApplication::translate("AppTestWidget", "What was not tested:", 0, QApplication::UnicodeUTF8));
        lblWhatWasNotTested->setText(QApplication::translate("AppTestWidget", "Multiplayer.", 0, QApplication::UnicodeUTF8));
        lblHeader3->setText(QApplication::translate("AppTestWidget", "Additional Comments:", 0, QApplication::UnicodeUTF8));
        lblAddComments->setText(QApplication::translate("AppTestWidget", "Mouse control issues, but overall works well.", 0, QApplication::UnicodeUTF8));
        grpTestResults->setTitle(QApplication::translate("AppTestWidget", "Top 5 test results:", 0, QApplication::UnicodeUTF8));
        grpKnownBugs->setTitle(QApplication::translate("AppTestWidget", "Known bugs:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AppTestWidget: public Ui_AppTestWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPTESTWIDGET_H
