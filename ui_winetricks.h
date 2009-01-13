/********************************************************************************
** Form generated from reading ui file 'winetricks.ui'
**
** Created: Tue Jan 13 11:38:31 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WINETRICKS_H
#define UI_WINETRICKS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_winetricks
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *lstMain;
    QLabel *label;
    QPushButton *btnInstWinetricks;
    QLabel *label_2;

    void setupUi(QDialog *winetricks)
    {
    if (winetricks->objectName().isEmpty())
        winetricks->setObjectName(QString::fromUtf8("winetricks"));
    winetricks->resize(536, 339);
    buttonBox = new QDialogButtonBox(winetricks);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(370, 10, 161, 61));
    buttonBox->setOrientation(Qt::Vertical);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    lstMain = new QListWidget(winetricks);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    new QListWidgetItem(lstMain);
    lstMain->setObjectName(QString::fromUtf8("lstMain"));
    lstMain->setGeometry(QRect(0, 10, 361, 281));
    label = new QLabel(winetricks);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(10, 300, 351, 20));
    btnInstWinetricks = new QPushButton(winetricks);
    btnInstWinetricks->setObjectName(QString::fromUtf8("btnInstWinetricks"));
    btnInstWinetricks->setGeometry(QRect(370, 300, 161, 27));
    label_2 = new QLabel(winetricks);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(10, 320, 351, 20));

    retranslateUi(winetricks);
    QObject::connect(buttonBox, SIGNAL(accepted()), winetricks, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), winetricks, SLOT(reject()));

    QMetaObject::connectSlotsByName(winetricks);
    } // setupUi

    void retranslateUi(QDialog *winetricks)
    {
    winetricks->setWindowTitle(QApplication::translate("winetricks", "Dialog", 0, QApplication::UnicodeUTF8));

    const bool __sortingEnabled = lstMain->isSortingEnabled();
    lstMain->setSortingEnabled(false);
    lstMain->item(0)->setText(QApplication::translate("winetricks", "art2kmin ", 0, QApplication::UnicodeUTF8));
    lstMain->item(1)->setText(QApplication::translate("winetricks", "colorprofile", 0, QApplication::UnicodeUTF8));
    lstMain->item(2)->setText(QApplication::translate("winetricks", "comctl32", 0, QApplication::UnicodeUTF8));
    lstMain->item(3)->setText(QApplication::translate("winetricks", "comctl32.ocx", 0, QApplication::UnicodeUTF8));
    lstMain->item(4)->setText(QApplication::translate("winetricks", "controlpad", 0, QApplication::UnicodeUTF8));
    lstMain->item(5)->setText(QApplication::translate("winetricks", "corefonts", 0, QApplication::UnicodeUTF8));
    lstMain->item(6)->setText(QApplication::translate("winetricks", "dcom98", 0, QApplication::UnicodeUTF8));
    lstMain->item(7)->setText(QApplication::translate("winetricks", "dirac0.8", 0, QApplication::UnicodeUTF8));
    lstMain->item(8)->setText(QApplication::translate("winetricks", "directx9", 0, QApplication::UnicodeUTF8));
    lstMain->item(9)->setText(QApplication::translate("winetricks", "divx", 0, QApplication::UnicodeUTF8));
    lstMain->item(10)->setText(QApplication::translate("winetricks", "dotnet11", 0, QApplication::UnicodeUTF8));
    lstMain->item(11)->setText(QApplication::translate("winetricks", "dotnet20", 0, QApplication::UnicodeUTF8));
    lstMain->item(12)->setText(QApplication::translate("winetricks", "ffdshow", 0, QApplication::UnicodeUTF8));
    lstMain->item(13)->setText(QApplication::translate("winetricks", "flash", 0, QApplication::UnicodeUTF8));
    lstMain->item(14)->setText(QApplication::translate("winetricks", "fm20", 0, QApplication::UnicodeUTF8));
    lstMain->item(15)->setText(QApplication::translate("winetricks", "fontfix", 0, QApplication::UnicodeUTF8));
    lstMain->item(16)->setText(QApplication::translate("winetricks", "gdiplus", 0, QApplication::UnicodeUTF8));
    lstMain->item(17)->setText(QApplication::translate("winetricks", "gecko", 0, QApplication::UnicodeUTF8));
    lstMain->item(18)->setText(QApplication::translate("winetricks", "icodecs", 0, QApplication::UnicodeUTF8));
    lstMain->item(19)->setText(QApplication::translate("winetricks", "jet40", 0, QApplication::UnicodeUTF8));
    lstMain->item(20)->setText(QApplication::translate("winetricks", "liberation", 0, QApplication::UnicodeUTF8));
    lstMain->item(21)->setText(QApplication::translate("winetricks", "mdac25", 0, QApplication::UnicodeUTF8));
    lstMain->item(22)->setText(QApplication::translate("winetricks", "mdac27", 0, QApplication::UnicodeUTF8));
    lstMain->item(23)->setText(QApplication::translate("winetricks", "mdac28", 0, QApplication::UnicodeUTF8));
    lstMain->item(24)->setText(QApplication::translate("winetricks", "mfc40", 0, QApplication::UnicodeUTF8));
    lstMain->item(25)->setText(QApplication::translate("winetricks", "mfc42", 0, QApplication::UnicodeUTF8));
    lstMain->item(26)->setText(QApplication::translate("winetricks", "mono20", 0, QApplication::UnicodeUTF8));
    lstMain->item(27)->setText(QApplication::translate("winetricks", "msi2", 0, QApplication::UnicodeUTF8));
    lstMain->item(28)->setText(QApplication::translate("winetricks", "mshfixgd", 0, QApplication::UnicodeUTF8));
    lstMain->item(29)->setText(QApplication::translate("winetricks", "msls31", 0, QApplication::UnicodeUTF8));
    lstMain->item(30)->setText(QApplication::translate("winetricks", "msmask", 0, QApplication::UnicodeUTF8));
    lstMain->item(31)->setText(QApplication::translate("winetricks", "msscript", 0, QApplication::UnicodeUTF8));
    lstMain->item(32)->setText(QApplication::translate("winetricks", "msxml3", 0, QApplication::UnicodeUTF8));
    lstMain->item(33)->setText(QApplication::translate("winetricks", "msxml4", 0, QApplication::UnicodeUTF8));
    lstMain->item(34)->setText(QApplication::translate("winetricks", "msxmk6", 0, QApplication::UnicodeUTF8));
    lstMain->item(35)->setText(QApplication::translate("winetricks", "ogg", 0, QApplication::UnicodeUTF8));
    lstMain->item(36)->setText(QApplication::translate("winetricks", "ole2", 0, QApplication::UnicodeUTF8));
    lstMain->item(37)->setText(QApplication::translate("winetricks", "pgh", 0, QApplication::UnicodeUTF8));
    lstMain->item(38)->setText(QApplication::translate("winetricks", "quicktime72", 0, QApplication::UnicodeUTF8));
    lstMain->item(39)->setText(QApplication::translate("winetricks", "riched20", 0, QApplication::UnicodeUTF8));
    lstMain->item(40)->setText(QApplication::translate("winetricks", "riched30", 0, QApplication::UnicodeUTF8));
    lstMain->item(41)->setText(QApplication::translate("winetricks", "tahoma", 0, QApplication::UnicodeUTF8));
    lstMain->item(42)->setText(QApplication::translate("winetricks", "urlmon", 0, QApplication::UnicodeUTF8));
    lstMain->item(43)->setText(QApplication::translate("winetricks", "vb3run", 0, QApplication::UnicodeUTF8));
    lstMain->item(44)->setText(QApplication::translate("winetricks", "vb4run", 0, QApplication::UnicodeUTF8));
    lstMain->item(45)->setText(QApplication::translate("winetricks", "vb5run", 0, QApplication::UnicodeUTF8));
    lstMain->item(46)->setText(QApplication::translate("winetricks", "vb6run", 0, QApplication::UnicodeUTF8));
    lstMain->item(47)->setText(QApplication::translate("winetricks", "vcrun6", 0, QApplication::UnicodeUTF8));
    lstMain->item(48)->setText(QApplication::translate("winetricks", "vcrun2003", 0, QApplication::UnicodeUTF8));
    lstMain->item(49)->setText(QApplication::translate("winetricks", "vcrun2005", 0, QApplication::UnicodeUTF8));
    lstMain->item(50)->setText(QApplication::translate("winetricks", "vcrun2005sp1", 0, QApplication::UnicodeUTF8));
    lstMain->item(51)->setText(QApplication::translate("winetricks", "vcrun2008", 0, QApplication::UnicodeUTF8));
    lstMain->item(52)->setText(QApplication::translate("winetricks", "wininet", 0, QApplication::UnicodeUTF8));
    lstMain->item(53)->setText(QApplication::translate("winetricks", "wmp9", 0, QApplication::UnicodeUTF8));
    lstMain->item(54)->setText(QApplication::translate("winetricks", "wmp10", 0, QApplication::UnicodeUTF8));
    lstMain->item(55)->setText(QApplication::translate("winetricks", "wsh56", 0, QApplication::UnicodeUTF8));
    lstMain->item(56)->setText(QApplication::translate("winetricks", "wsh56js", 0, QApplication::UnicodeUTF8));
    lstMain->item(57)->setText(QApplication::translate("winetricks", "wsh56vb", 0, QApplication::UnicodeUTF8));
    lstMain->item(58)->setText(QApplication::translate("winetricks", "xvid", 0, QApplication::UnicodeUTF8));
    lstMain->item(59)->setText(QApplication::translate("winetricks", "firefox3", 0, QApplication::UnicodeUTF8));
    lstMain->item(60)->setText(QApplication::translate("winetricks", "ie6", 0, QApplication::UnicodeUTF8));
    lstMain->item(61)->setText(QApplication::translate("winetricks", "kde", 0, QApplication::UnicodeUTF8));
    lstMain->item(62)->setText(QApplication::translate("winetricks", "mpc", 0, QApplication::UnicodeUTF8));
    lstMain->item(63)->setText(QApplication::translate("winetricks", "vlc", 0, QApplication::UnicodeUTF8));
    lstMain->item(64)->setText(QApplication::translate("winetricks", "allfonts", 0, QApplication::UnicodeUTF8));
    lstMain->item(65)->setText(QApplication::translate("winetricks", "allcodecs", 0, QApplication::UnicodeUTF8));
    lstMain->item(66)->setText(QApplication::translate("winetricks", "fakeie6", 0, QApplication::UnicodeUTF8));
    lstMain->item(67)->setText(QApplication::translate("winetricks", "native_mdac", 0, QApplication::UnicodeUTF8));
    lstMain->item(68)->setText(QApplication::translate("winetricks", "native_oleaut32", 0, QApplication::UnicodeUTF8));

    lstMain->setSortingEnabled(__sortingEnabled);
    label->setText(QApplication::translate("winetricks", "Compatible with winetricks >= 20081223", 0, QApplication::UnicodeUTF8));
    btnInstWinetricks->setText(QApplication::translate("winetricks", "Install Winetricks", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("winetricks", "Uses cabextract", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(winetricks);
    } // retranslateUi

};

namespace Ui {
    class winetricks: public Ui_winetricks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINETRICKS_H
