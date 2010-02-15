/********************************************************************************
** Form generated from reading UI file 'CommentWidget.ui'
**
** Created: Mon Feb 15 09:49:50 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMENTWIDGET_H
#define UI_COMMENTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CommentWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widgetLabel;
    QHBoxLayout *widgetLabelLayout;
    QLabel *lblContent;
    QSpacerItem *verticalSpacer;

    void setupUi(QFrame *CommentWidget)
    {
        if (CommentWidget->objectName().isEmpty())
            CommentWidget->setObjectName(QString::fromUtf8("CommentWidget"));
        CommentWidget->resize(563, 30);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CommentWidget->sizePolicy().hasHeightForWidth());
        CommentWidget->setSizePolicy(sizePolicy);
        CommentWidget->setFrameShape(QFrame::StyledPanel);
        CommentWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(CommentWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widgetLabel = new QWidget(CommentWidget);
        widgetLabel->setObjectName(QString::fromUtf8("widgetLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetLabel->sizePolicy().hasHeightForWidth());
        widgetLabel->setSizePolicy(sizePolicy1);
        widgetLabelLayout = new QHBoxLayout(widgetLabel);
        widgetLabelLayout->setSpacing(2);
        widgetLabelLayout->setObjectName(QString::fromUtf8("widgetLabelLayout"));
        widgetLabelLayout->setContentsMargins(4, 0, 4, 1);

        verticalLayout->addWidget(widgetLabel);

        lblContent = new QLabel(CommentWidget);
        lblContent->setObjectName(QString::fromUtf8("lblContent"));
        sizePolicy.setHeightForWidth(lblContent->sizePolicy().hasHeightForWidth());
        lblContent->setSizePolicy(sizePolicy);
        lblContent->setTextFormat(Qt::RichText);
        lblContent->setWordWrap(true);
        lblContent->setMargin(3);
        lblContent->setOpenExternalLinks(true);
        lblContent->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(lblContent);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(CommentWidget);

        QMetaObject::connectSlotsByName(CommentWidget);
    } // setupUi

    void retranslateUi(QFrame *CommentWidget)
    {
        CommentWidget->setWindowTitle(QApplication::translate("CommentWidget", "Frame", 0, QApplication::UnicodeUTF8));
        lblContent->setText(QApplication::translate("CommentWidget", "Test message.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CommentWidget: public Ui_CommentWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMENTWIDGET_H
