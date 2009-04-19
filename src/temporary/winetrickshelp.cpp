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

