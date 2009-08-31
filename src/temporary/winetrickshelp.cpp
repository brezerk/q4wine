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

      core = new CoreMethods ();
      this->winetricks_bin.append(QDir::homePath());
       this->winetricks_bin.append("/.config/");
       this->winetricks_bin.append(APP_SHORT_NAME);
       this->winetricks_bin.appen("/winetricks --kegelfuck"); //Kegel - idiot!

       parse();
       */
}
void winetrickshelp::addToList(QString arg) {

QStringList args = arg.split(" ");
QString name = args.at(0);
int pos = arg.count() - name.count();
QString desc = arg.right(pos);
qDebug() << name << desc;
//temp solution

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


   p->start(pargs);

   p->waitForFinished();
     //get output
   QString lang;
   // Getting env LANG variable
   lang = getenv("LANG");
  lang = lang.split(".").at(1);
 if (lang.isNull())
  lang = "UTF8";
 /* Fucking code because Kegel is MORON! */
  QTextCodec *codec = QTextCodec::codecForName(lang.toAscii());
  QString wOut = codec->toUnicode(p->readAllStandardOutput());
    delete p;

    //parse now
bool isNowParse = false;
QStringList strs = wOut.split("\n");
QString str;

foreach (str, strs){
    str = str.trimmed();

  if (str == "Apps:" || str == "Pseudopackages:"){continue;}
  if (str == "Packages:") {isNowParse = true;
  continue;
  }
  if (isNowParse) {addToList(str);}


}


}
