/********************************************************************************
** Form generated from reading UI file 'AppInfoWidget.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPINFOWIDGET_H
#define UI_APPINFOWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppInfoWidget
{
public:
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lblAppName;
    QSpacerItem *horizontalSpacer;
    QFrame *line_2;
    QLabel *lblAppDesc;
    QWidget *AppVersionLister;
    QVBoxLayout *AppVersionListerLayout;

    void setupUi(QWidget *AppInfoWidget)
    {
        if (AppInfoWidget->objectName().isEmpty())
            AppInfoWidget->setObjectName(QString::fromUtf8("AppInfoWidget"));
        AppInfoWidget->resize(601, 71);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AppInfoWidget->sizePolicy().hasHeightForWidth());
        AppInfoWidget->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(AppInfoWidget);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(AppInfoWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setPixmap(QPixmap(QString::fromUtf8(":/data/winecfg.png")));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label->setWordWrap(false);
        label->setMargin(5);
        label->setTextInteractionFlags(Qt::NoTextInteraction);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 2, 2, 2);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lblAppName = new QLabel(widget_2);
        lblAppName->setObjectName(QString::fromUtf8("lblAppName"));
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


        verticalLayout_2->addWidget(widget_2);

        line_2 = new QFrame(widget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        lblAppDesc = new QLabel(widget);
        lblAppDesc->setObjectName(QString::fromUtf8("lblAppDesc"));
        sizePolicy.setHeightForWidth(lblAppDesc->sizePolicy().hasHeightForWidth());
        lblAppDesc->setSizePolicy(sizePolicy);
        lblAppDesc->setTextFormat(Qt::PlainText);
        lblAppDesc->setWordWrap(true);
        lblAppDesc->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout_2->addWidget(lblAppDesc);

        AppVersionLister = new QWidget(widget);
        AppVersionLister->setObjectName(QString::fromUtf8("AppVersionLister"));
        sizePolicy.setHeightForWidth(AppVersionLister->sizePolicy().hasHeightForWidth());
        AppVersionLister->setSizePolicy(sizePolicy);
        AppVersionListerLayout = new QVBoxLayout(AppVersionLister);
        AppVersionListerLayout->setSpacing(0);
        AppVersionListerLayout->setContentsMargins(0, 0, 0, 0);
        AppVersionListerLayout->setObjectName(QString::fromUtf8("AppVersionListerLayout"));

        verticalLayout_2->addWidget(AppVersionLister);


        gridLayout->addWidget(widget, 0, 1, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(AppInfoWidget);

        QMetaObject::connectSlotsByName(AppInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *AppInfoWidget)
    {
        AppInfoWidget->setWindowTitle(QApplication::translate("AppInfoWidget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        lblAppName->setText(QApplication::translate("AppInfoWidget", "Some App Name", 0, QApplication::UnicodeUTF8));
        lblAppDesc->setText(QApplication::translate("AppInfoWidget", "Some App Name description which might be not more then 255 characters longer...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AppInfoWidget: public Ui_AppInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPINFOWIDGET_H
