#include "winetrickshelp.h"
#include "ui_winetrickshelp.h"
#include <QtDebug>

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
      core = new CoreMethods ();
      this->winetricks_bin.append(QDir::homePath());
       this->winetricks_bin.append("/.config/");
       this->winetricks_bin.append(APP_SHORT_NAME);
       this->winetricks_bin.append("/winetricks --kegelfuck"); //Kegel - idiot!

       parse();
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
void winetrickshelp::parse() {
    //create a Winetricks process
      QProcess *p = new QProcess (this);
    QString pargs;
    pargs.append(winetricks_bin);
qDebug () <<  pargs;

   p->start(pargs);

   p->waitForFinished();
     //get output
   QString lang;
                // Getting env LANG variable
                lang = getenv("LANG");
                lang = lang.split(".").at(1);

                if (lang.isNull())
                        lang = "UTF8";
                QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
                QString string = codec->toUnicode(p->readAllStandardError());
qDebug() << string;


    delete p;

    //parse now

}
