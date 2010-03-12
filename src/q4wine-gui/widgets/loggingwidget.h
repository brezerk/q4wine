#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

//System
#include <memory>

//Global config
#include "config.h"

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QSplitter>
#include <QLineEdit>
#include <QDateTime>
#include <QBrush>
#include <QPalette>
#include <QKeyEvent>

#include "logging.h"
#include "prefix.h"

//q4wine lib
#include "q4wine-lib.h"

class LoggingWidget : public QWidget
{
Q_OBJECT
public:
    explicit LoggingWidget(QWidget *parent = 0);
    ~LoggingWidget();


signals:

public slots:
    void getLogRecords(void);

private slots:
    void logClear_Click(void);
    void logDelete_Click(void);
    void logExport_Click(void);

    void treeWidget_itemClicked (QTreeWidgetItem * item, int colum);
    void treeWidget_currentItemChanged (QTreeWidgetItem *item, QTreeWidgetItem *);
    void treeWidget_keyPressEvent ( QKeyEvent * event );

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    Logging db_log;
    Prefix db_prefix;

    std::auto_ptr<QAction> logClear;
    std::auto_ptr<QAction> logDelete;
    std::auto_ptr<QAction> logExport;

    std::auto_ptr<QTreeWidget> treeWidget;
    std::auto_ptr<QListWidget> listWidget;
    std::auto_ptr<QSplitter> splitter;

    void createActions(void);
    void clearLogs(void);
};

#endif // LOGGINGWIDGET_H
