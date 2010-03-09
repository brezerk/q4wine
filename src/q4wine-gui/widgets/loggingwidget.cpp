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
    toolBar->addSeparator ();
    toolBar->addAction(logClearSearch.get());
    searchField.reset (new QLineEdit(this));
    connect(searchField.get(), SIGNAL(returnPressed()), this, SLOT(logSearch_Click()));
    toolBar->addWidget(searchField.get());
    toolBar->addAction(logSearch.get());

    treeWidget.reset(new QTreeWidget(this));
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderLabel("Logging list");

    listWidget.reset(new QListWidget(this));

    std::auto_ptr<QSplitter> splitter;
    splitter.reset(new QSplitter(this));

    splitter->addWidget(treeWidget.get());
    splitter->addWidget( listWidget.get());

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(toolBar.release());
    layout->addWidget(splitter.release());

    this->setLayout(layout.release());
}

void LoggingWidget::createActions(){
    logClear.reset(new QAction(CoreLib->loadIcon("data/wizard.png"), tr("Create new"), this));
    logClear->setStatusTip(tr("Create new prefix"));
    connect(logClear.get(), SIGNAL(triggered()), this, SLOT(logClear_Click()));

    logDelete.reset(new QAction(CoreLib->loadIcon("data/down.png"), tr("Import prefix"), this));
    logDelete->setStatusTip(tr("Import prefix"));
    connect(logDelete.get(), SIGNAL(triggered()), this, SLOT(logDelete_Click()));
    logDelete->setEnabled(FALSE);

    logExport.reset(new QAction(CoreLib->loadIcon("data/up.png"), tr("Export prefix"), this));
    logExport->setStatusTip(tr("Export prefix"));
    connect(logExport.get(), SIGNAL(triggered()), this, SLOT(logExport_Click()));
    logExport->setEnabled(FALSE);

    logClearSearch.reset(new QAction(CoreLib->loadIcon("data/clear-ltr.png"), tr("Clear search field"),this));
    logClearSearch->setStatusTip(tr("Clear search field"));
    connect(logClearSearch.get(), SIGNAL(triggered()), this, SLOT(logClearSearch_Click()));

    logSearch.reset(new QAction(CoreLib->loadIcon("data/find.png"), tr("Search in appdb"),this));
    logSearch->setStatusTip(tr("Search in wine appdb"));
    connect(logSearch.get(), SIGNAL(triggered()), this, SLOT(logSearch_Click()));

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

void LoggingWidget::logClear_Click(){
    return;
}

void LoggingWidget::logDelete_Click(){
    return;
}

void LoggingWidget::logExport_Click(){
    return;
}

void LoggingWidget::logClearSearch_Click(void){
    return;
}

void LoggingWidget::logSearch_Click(void){
    return;
}
