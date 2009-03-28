#include "winetrickshelp.h"
#include "ui_winetrickshelp.h"

winetrickshelp::winetrickshelp(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::winetrickshelp)
{
    m_ui->setupUi(this);
    /*
    QString WINETRICKS_FILE = QDir::homePath() + ".config/q4wine/winetricks";
if (!QFile::exists(WINETRICKS_FILE)) {
    QMessageBox msg;
    msg.setText(tr("Please install winetricks: winetricks not found"));
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle(tr("winetricks not found"));
    msg.show();
this->close();
return;

}
*/
QFile file;
file.setFileName(QDir::homePath() + "/.config/q4wine/winetricks_help");
if (!file.open(QIODevice::ReadOnly)) {
   QMessageBox msg;
    msg.setText("error open " + file.fileName());
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle(tr("winetricks not found"));
    msg.show();
}
QTextStream out (&file);
out.setCodec(QTextCodec::codecForLocale());
QString str;
str = out.readAll();
file.close();
m_ui->txtOutput->setText(str);
}

winetrickshelp::~winetrickshelp()
{
    delete m_ui;
}

void winetrickshelp::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void winetrickshelp::on_buttonBox_clicked(QAbstractButton* button){

   return;
}

