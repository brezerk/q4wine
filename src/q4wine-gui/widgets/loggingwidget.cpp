#include "loggingwidget.h"

LoggingWidget::LoggingWidget(QWidget *parent) :
    QWidget(parent)
{
    // Loading libq4wine-core.so
    libq4wine.setFileName("libq4wine-core");

    if (!libq4wine.load()){
          libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->createActions();

    std::auto_ptr<QToolBar> toolBar (new QToolBar(this));
    toolBar->addAction(logClear.get());
    toolBar->addSeparator ();
    toolBar->addAction(logDelete.get());
    toolBar->addAction(logExport.get());

    treeWidget.reset(new QTreeWidget(this));
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderLabel("Logging list");

    connect (treeWidget.get(), SIGNAL(itemActivated (QTreeWidgetItem *, int)), this, SLOT(treeWidget_itemClicked (QTreeWidgetItem *, int)));
    connect (treeWidget.get(), SIGNAL(currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(treeWidget_currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)));

    treeWidget->installEventFilter(this);

    listWidget.reset(new QListWidget(this));


    splitter.reset(new QSplitter(this));

    splitter->addWidget(treeWidget.get());
    splitter->addWidget(listWidget.get());

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());
    layout->addWidget(splitter.get());

    this->setLayout(layout.release());

    QList<int> a;
    a.append(CoreLib->getSetting("MainWindow", "splitterLogSize0", false, 170).toInt());
    a.append(CoreLib->getSetting("MainWindow", "splitterLogSize1", false, 379).toInt());

    splitter->setSizes(a);

    //this->clearLogs();
}

LoggingWidget::~LoggingWidget(){
    this->treeWidget.release();
    this->listWidget.release();

    if (splitter->sizes().at(0) != splitter->sizes().at(1)){
        QSettings settings(APP_SHORT_NAME, "default");
        settings.beginGroup("MainWindow");
        settings.setValue("splitterLogSize0", splitter->sizes().at(0));
        settings.setValue("splitterLogSize1", splitter->sizes().at(1));
        settings.endGroup();
    }
}

void LoggingWidget::createActions(){
    logClear.reset(new QAction(CoreLib->loadIcon("data/clear-list.png"), tr("Clear logs"), this));
    logClear->setStatusTip(tr("Clear logs database"));
    connect(logClear.get(), SIGNAL(triggered()), this, SLOT(logClear_Click()));
    logClear->setEnabled(false);

    logDelete.reset(new QAction(CoreLib->loadIcon("data/kill.png"), tr("Delete log"), this));
    logDelete->setStatusTip(tr("Delete current log"));
    connect(logDelete.get(), SIGNAL(triggered()), this, SLOT(logDelete_Click()));
    logDelete->setEnabled(false);

    logExport.reset(new QAction(CoreLib->loadIcon("data/up.png"), tr("Export log"), this));
    logExport->setStatusTip(tr("Export current log to file"));
    connect(logExport.get(), SIGNAL(triggered()), this, SLOT(logExport_Click()));
    logExport->setEnabled(false);

    /*
    menu.reset(new QMenu(this));
    menu->addAction(prefixAdd.get());
    menu->addSeparator();
    menu->addAction(prefixImport.get());
    menu->addAction(prefixExport.get());
    menu->addSeparator();
    menu->addAction(prefixDelete.get());
    menu->addSeparator();
    menu->addAction(prefixSettings.get());
*/
    return;
}

void LoggingWidget::clearLogs(void){
    db_log.clearLogs();
    return;
}

void LoggingWidget::treeWidget_itemClicked (QTreeWidgetItem * item, int colum){
    if (!item){
        logExport->setEnabled(false);
        logDelete->setEnabled(false);
        return;
    }

    QString program, prefix, date;
    if (!item->parent()){
        logExport->setEnabled(false);
        logDelete->setEnabled(true);
        return;
    } else {
        std::auto_ptr<QTreeWidgetItem> p_item (item->parent());
        if (!p_item->parent()){
            p_item.release();
            logExport->setEnabled(false);
            logDelete->setEnabled(true);
            return;
        } else {
            this->listWidget->clear();

            program = p_item->text(0);
            prefix = p_item->parent()->text(0);
            date = item->text(0);
            QString list = db_log.getLogs(prefix, program, date);

                QStringList rows = list.split("\n");
                for (int j=0; j<rows.count(); j++){
                    if (!rows.at(j).isEmpty()){

                        std::auto_ptr<QListWidgetItem> iconItem (new QListWidgetItem(this->listWidget.get(), 0));
                        iconItem->setText(rows.at(j));

                        if( j & 1 ){
                            QPalette pal;
                            iconItem->setBackground(pal.midlight() );
                        }

                        if (rows.at(j) == tr("Exec string:")){
                             iconItem->setIcon(CoreLib->loadIcon("data/down_log.png"));
                         } else if (rows.at(j) == tr("Exit code:")){
                             iconItem->setIcon(CoreLib->loadIcon("data/down_log.png"));
                         } else if (rows.at(j) == tr("App STDOUT and STDERR output:")){
                             iconItem->setIcon(CoreLib->loadIcon("data/down_log.png"));
                         }

                        iconItem.release();
                    }
                }
                logExport->setEnabled(true);
                logDelete->setEnabled(true);
        }
        p_item.release();
    }

    return;
}

void LoggingWidget::treeWidget_currentItemChanged (QTreeWidgetItem *item, QTreeWidgetItem *){
    this->listWidget->clear();

    if (!item)
        return;

    treeWidget_itemClicked (item, 0);
    return;
}

void LoggingWidget::logClear_Click(){
    this->treeWidget->clear();
    this->listWidget->clear();
    db_log.clearLogs();
    logClear->setEnabled(false);
    logExport->setEnabled(false);
    logDelete->setEnabled(false);
    return;
}

void LoggingWidget::logDelete_Click(){
    if (!this->treeWidget->currentItem())
        return;

    std::auto_ptr<QTreeWidgetItem> item (this->treeWidget->currentItem());

    if (item->parent()){
        std::auto_ptr<QTreeWidgetItem> parentItem (item->parent());

        if (parentItem->parent()){
            db_log.deleteLogs(parentItem->parent()->text(0), parentItem->text(0), item->text(0));
        } else {
            db_log.deleteLogs(parentItem->text(0), item->text(0));
        }

        getLogRecords();
        parentItem.release();
    } else {
        db_log.deleteLogs(item->text(0));
        getLogRecords();
    }

    item.release();
    return;
}

void LoggingWidget::logExport_Click(){
    return;
}

void LoggingWidget::getLogRecords(void){

    this->treeWidget->clear();
    this->listWidget->clear();

    QStringList p_list = db_prefix.getPrefixList();

    for (int i=0; i<p_list.count(); i++){
        QStringList app_list = db_log.getApps(p_list.at(i));

        if (app_list.count()>0){

            logClear->setEnabled(true);

            std::auto_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(treeWidget.get()));
            prefixItem->setText(0, QString("%1").arg(p_list.at(i)));
            prefixItem->setIcon(0, CoreLib->loadIcon("data/wine.png"));
            prefixItem->setExpanded (TRUE);
            treeWidget->addTopLevelItem(prefixItem.get());

            for (int j=0; j<app_list.count(); j++){
                std::auto_ptr<QTreeWidgetItem> appItem (new QTreeWidgetItem(prefixItem.get(), 0));
                appItem->setText(0, QString("%1").arg(app_list.at(j)));
                appItem->setIcon(0, CoreLib->loadIcon("data/exec.png"));
                appItem->setExpanded(true);

                QHash<uint, int> date_list = db_log.getDates(p_list.at(i), app_list.at(j));

                QList<uint> date_keys = date_list.keys();
                qSort(date_keys.begin(), date_keys.end(), qGreater<int>());

                for (int n=0; n<date_keys.count(); n++){
                    QDateTime date;
                    date.setTime_t(date_keys.at(n));

                    int exit = date_list.value(date_keys.at(n));

                    std::auto_ptr<QTreeWidgetItem> dateItem (new QTreeWidgetItem(appItem.get(), 0));
                    dateItem->setText(0, QString("%1").arg(date.toString("dd.MM.yyyy hh:mm:ss")));

                    if (exit==0){
                        dateItem->setIcon(0, CoreLib->loadIcon("data/ok.png"));
                    } else {
                        dateItem->setIcon(0, CoreLib->loadIcon("data/fail.png"));
                    }

                    dateItem.release();
                }
                appItem.release();
            }
            prefixItem.release();
        }
    }
    return;
}

bool LoggingWidget::eventFilter(QObject *obj, QEvent *event)
 {
     if (obj == this->treeWidget.get()) {
         if (event->type() == QEvent::KeyPress) {
             QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

             if (keyEvent->matches(QKeySequence::Delete)){
                 this->logDelete_Click();
                 return true;
             }
         } else {
             return false;
         }
     } else {
         // pass the event on to the parent class
         return QWidget::eventFilter(obj, event);
     }
 }
