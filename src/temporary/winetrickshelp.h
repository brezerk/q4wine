#ifndef WINETRICKSHELP_H
#define WINETRICKSHELP_H

#include <QtGui/QDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>
namespace Ui {
    class winetrickshelp;
}

class winetrickshelp : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(winetrickshelp)
public:
    explicit winetrickshelp(QWidget *parent = 0);
    virtual ~winetrickshelp();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::winetrickshelp *m_ui;



private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
};

#endif // WINETRICKSHELP_H
