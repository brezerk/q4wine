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
#include <QListWidget>
#include <QSplitter>
#include <QLineEdit>

//q4wine lib
#include "q4wine-lib.h"

class LoggingWidget : public QWidget
{
Q_OBJECT
public:
    explicit LoggingWidget(QWidget *parent = 0);

signals:

public slots:

private slots:
    void logClear_Click(void);
    void logDelete_Click(void);
    void logExport_Click(void);
    void logClearSearch_Click(void);
    void logSearch_Click(void);

private:
    //! This is need for libq4wine-core.so import.
    QLibrary libq4wine;
    typedef void *CoreLibPrototype (bool);
    CoreLibPrototype *CoreLibClassPointer;
    std::auto_ptr<corelib> CoreLib;

    std::auto_ptr<QAction> logClear;
    std::auto_ptr<QAction> logDelete;
    std::auto_ptr<QAction> logExport;
    std::auto_ptr<QAction> logClearSearch;
    std::auto_ptr<QAction> logSearch;

    std::auto_ptr<QLineEdit> searchField;

    std::auto_ptr<QTreeWidget> treeWidget;
    std::auto_ptr<QListWidget> listWidget;

    void createActions(void);
};

#endif // LOGGINGWIDGET_H
