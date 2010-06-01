#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <memory>

#include "config.h"

#include "src/q4wine-gui/ui_InfoDialog.h"

#include <QDialog>
#include <QWidget>
#include <QSettings>

class InfoDialog : public QDialog, public Ui::InfoDialog
{
Q_OBJECT
public:
    explicit InfoDialog(int action, QWidget *parent = 0, Qt::WFlags f = 0);

private:
    int action;

signals:

public slots:

private slots:
    void cmdOk_clicked();
    void cmdCancel_clicked();

};

#endif // INFODIALOG_H
