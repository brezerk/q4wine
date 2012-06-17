/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey                           *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "prefixconfigwidget.h"

PrefixConfigWidget::PrefixConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName(_CORELIB_PATH_);
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif

    if (!libq4wine.load()){
          libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->sort_order = CoreLib->getSetting("WinetricksWidget", "IconSort", false, D_SORT_TYPE_BY_DATE_ASC).toInt();

    this->createActions();

    std::auto_ptr<QToolBar> treeToolBar (new QToolBar(this));
    treeToolBar->setIconSize(QSize(24, 24));

    cbPrefixes.reset(new QComboBox(this));
    cbPrefixes->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    cbPrefixes->setMinimumWidth(180);
    connect(cbPrefixes.get(), SIGNAL(currentIndexChanged(QString)), this, SIGNAL(prefixIndexChanged(QString)));


    treeToolBar->addWidget(cbPrefixes.get());
    treeToolBar->addAction(prefixManage.release());

    treeWidget.reset(new QTreeWidget(this));
    treeWidget->setColumnCount(1);
    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    treeWidget->header()->close();
    connect (treeWidget.get(), SIGNAL(currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(treeWidget_currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)));

    treeWidget->installEventFilter(this);

    std::auto_ptr<QToolBar> iconsToolBar (new QToolBar(this));
    iconsToolBar->setIconSize(QSize(24, 24));

    searchField.reset (new QLineEdit(this));
    connect(searchField.get(), SIGNAL(textChanged(QString)), this, SLOT(searchFilterChange(QString)));

    //connect(searchField.get(), SIGNAL(returnPressed()), this, SLOT(appdbSearch_Click()));
    iconsToolBar->addAction(searchClear.get());
    iconsToolBar->addWidget(searchField.get());
    iconsToolBar->addAction(sortAlpha.get());

    listWidget.reset(new QListWidget(this));
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setMovement(QListView::Static);
    int nSize = CoreLib->getSetting("IconWidget", "IconSize", false, 32).toInt();
    listWidget->setViewMode(QListView::ListMode);
    listWidget->setGridSize(QSize(nSize, nSize));
    listWidget->setIconSize(QSize(nSize, nSize));
    listWidget->setWrapping(false);
    listWidget->setWordWrap(false);
    connect(listWidget.get(), SIGNAL(itemClicked (QListWidgetItem *)), this, SLOT(itemClicked (QListWidgetItem *)));
    connect(listWidget.get(), SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(itemDoubleClicked (QListWidgetItem *)));

    std::auto_ptr<QWidget> treeFrameWidget(new QWidget(this));

    std::auto_ptr<QVBoxLayout> layout (new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(treeToolBar.release());
    layout->addWidget(treeWidget.get());

    treeFrameWidget->setLayout(layout.release());

    std::auto_ptr<QWidget> iconsFrameWidget(new QWidget(this));

    layout.reset(new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(iconsToolBar.release());
    layout->addWidget(listWidget.get());

    iconsFrameWidget->setLayout(layout.release());


    splitter.reset(new QSplitter(this));
    splitter->addWidget(treeFrameWidget.release());
    splitter->addWidget(iconsFrameWidget.release());

    infoName.reset(new QLabel(QString("%1:\n\%2:").arg(tr("Name")).arg(tr("Description")), this));
    infoName->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    std::auto_ptr<QFrame> frame(new QFrame(this));

    layout.reset(new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(0,6,0,0);
    layout->addWidget(infoName.get());

    frame->setLayout(layout.release());
    frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    infoName->setFrameStyle(QFrame::Raised);
    infoName->setFrameShape(QFrame::StyledPanel);

    layout.reset(new QVBoxLayout(this));
    layout->setSpacing(0);
    layout->setContentsMargins(3,0,3,3);
    layout->addWidget(splitter.get());
    layout->addWidget(frame.release());

    this->setLayout(layout.release());

    QList<int> a;
    a.append(CoreLib->getSetting("MainWindow", "splitterPrefixConfigSize0", false, 170).toInt());
    a.append(CoreLib->getSetting("MainWindow", "splitterPrefixConfigSize1", false, 379).toInt());

    splitter->setSizes(a);

    this->getPrefixes();
    this->createTree();
}

PrefixConfigWidget::~PrefixConfigWidget(){
    treeWidget.release();
    listWidget.release();
    infoName.release();

    if (splitter->sizes().at(0) != splitter->sizes().at(1)){
        QSettings settings(APP_SHORT_NAME, "default");
        settings.beginGroup("MainWindow");
        settings.setValue("splitterPrefixConfigSize0", splitter->sizes().at(0));
        settings.setValue("splitterPrefixConfigSize1", splitter->sizes().at(1));
        settings.endGroup();
        settings.beginGroup("WinetricksWidget");
        settings.setValue("IconSort", this->sort_order);
        settings.endGroup();
    }
    splitter.release();
}

void PrefixConfigWidget::createActions(){
    prefixManage.reset(new QAction(CoreLib->loadIcon("data/configure.png"), tr("Manage prefixes"), this));
    prefixManage->setStatusTip(tr("Manage prefixes"));
    connect(prefixManage.get(), SIGNAL(triggered()), this, SLOT(prefixManage_Click()));

    searchClear.reset(new QAction(CoreLib->loadIcon("data/clear-ltr.png"), tr("Clear search field"), this));
    searchClear->setStatusTip(tr("Clear search field"));
    connect(searchClear.get(), SIGNAL(triggered()), this, SLOT(searchClear_Click()));

    sortAlpha.reset(new QAction(this));
    connect(sortAlpha.get(), SIGNAL(triggered()), this, SLOT(sortAlpha_Click()));

    if (this->sort_order == D_SORT_TYPE_BY_NAME_ASC){
        sortAlpha->setStatusTip(tr("Alphabetic sort descending"));
        sortAlpha->setText(tr("Alphabetic sort descending"));
        sortAlpha->setIcon(CoreLib->loadIcon("data/sort-desc.png"));
    } else {
        sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
        sortAlpha->setText(tr("Alphabetic sort ascending"));
        sortAlpha->setIcon(CoreLib->loadIcon("data/sort-asc.png"));
    }

    return;
}

void PrefixConfigWidget::createTree(){
    this->treeWidget.get()->clear();
    this->listWidget->clear();

    QList<ProviderItem> providers = db_sysconfig.getProviders();
    std::auto_ptr<QTreeWidgetItem> prefixItem;
    for (int i = 0; i < providers.size(); i++){
        prefixItem.reset(new QTreeWidgetItem(this->treeWidget.get()));
        prefixItem->setText(0, providers.at(i).name);
        prefixItem->setIcon(0, CoreLib->loadIcon(QString("data/%1").arg(providers.at(i).icon)));
        prefixItem->setExpanded(true);

        QStringList subtypes = db_sysconfig.getProviderSubtypes(providers.at(i).id);
        for (int j = 0; j < subtypes.size(); j++){
            if (!subtypes.at(j).isEmpty()){
                std::auto_ptr<QTreeWidgetItem> subPrefixItem (new QTreeWidgetItem(prefixItem.get(), 0));
                subPrefixItem->setText(0, QString("%1").arg(subtypes.at(j)));
                subPrefixItem->setIcon(0, CoreLib->loadIcon("data/folder.png"));
                subPrefixItem.release();
            }
        }

        treeWidget->addTopLevelItem(prefixItem.release());
    }
    prefixItem.release();
    if (this->provider.isEmpty())
        this->provider = "System";

    for(int i=0; i < this->treeWidget.get()->topLevelItemCount(); i++)
    {
        std::auto_ptr<QTreeWidgetItem> item(this->treeWidget.get()->topLevelItem(i));
        if (item->text(0)==this->provider){

            this->treeWidget.get()->setCurrentItem(item.get());
            this->treeWidget.get()->topLevelItem(i)->setExpanded(TRUE);
            this->treeWidget_itemClicked(item.get(), 0);
            item.release();
            break;
        } else {
            item.release();
        }
    }
}

void PrefixConfigWidget::getPrefixes(){
    cbPrefixes->clear();
    QStringList list = db_prefix.getPrefixList();
    cbPrefixes->addItems (list);
}

void PrefixConfigWidget::treeWidget_itemClicked (QTreeWidgetItem * item, int colum){
    this->provider = "";
    this->subtype = "";

    if (!item){
        return;
    }

    if (!item->parent()){
        this->provider = item->text(0);
    } else {
        std::auto_ptr<QTreeWidgetItem> p_item (item->parent());
        this->provider = p_item->text(0);
        this->subtype = item->text(0);
        p_item.release();
    }

    this->get_icons();

    return;
}

void PrefixConfigWidget::get_icons(){
    this->listWidget.get()->clear();

    QList<SysconfigItem> items = db_sysconfig.getItems(this->provider, this->subtype, this->sort_order, this->searchField->text());
    std::auto_ptr<QListWidgetItem> iconItem;
    for (int i = 0; i < items.size(); i++){
        iconItem.reset(new QListWidgetItem(this->listWidget.get(), 0));
        if (this->subtype.isEmpty()){
            QString name;
            QString desc;
            if (this->provider=="System"){
                if (items.at(i).name == "%CREATE_FAKE%"){
                    name = tr("Create Fake Drive");
                    desc = tr("Create new Fake Drive configuration. Old Fake Drive will be removed.");
                } else if (items.at(i).name == "%UPDATE_FAKE%"){
                    name = tr("Update Fake Drive");
                    desc = tr("Update Fake Drive configuration.");
                }
            } else if (this->provider=="Winetricks"){
                if (items.at(i).name == "%INSTALL_WINETRICKS%"){
                    name = tr("Install or update Winetricks script");
                    desc = tr("Install new or update installed Winetricks script.");
                } else if (items.at(i).name == "%REFRESH_WINETRICKS%"){
                    name = tr("Refresh Winetricks application list");
                    desc = tr("Create or update Q4Wine's cache of Winetricks appications.");
                }
            }
            iconItem->setText(name);
            iconItem->setToolTip(desc);
        } else {
            iconItem->setText(items.at(i).name);
            iconItem->setToolTip(items.at(i).desc);
        }
        iconItem->setIcon(CoreLib->loadIcon(QString("data/%1").arg(items.at(i).icon)));
        iconItem.release();
    }
}

void PrefixConfigWidget::treeWidget_currentItemChanged (QTreeWidgetItem *item, QTreeWidgetItem *){
    this->listWidget->clear();

    if (!item)
        return;

    treeWidget_itemClicked (item, 0);
    return;
}

void PrefixConfigWidget::itemClicked (QListWidgetItem *item){
    if (!item)
        return;

    this->infoName->setText(QString("%1: %2\n\%3: %4").arg(tr("Name")).arg(item->text()).arg(tr("Description")).arg(item->toolTip()));
    return;
}

void PrefixConfigWidget::itemDoubleClicked (QListWidgetItem *item){
    if (!item)
        return;

    if (this->provider == "System"){
        QString prefixPath = db_prefix.getPath(this->cbPrefixes->currentText());

        QString sysregPath;
        sysregPath.append(prefixPath);
        sysregPath.append("/system.reg");

        QFile sysreg_file (sysregPath);

        if (item->text() == tr("Create Fake Drive")){
            if (sysreg_file.exists()){
                if (QMessageBox::warning(this, tr("Warning"), tr("A fake drive already exists within <b>%1</b>.<br><br>Do you wish to remove <b>all</b> files from this prefix?").arg(prefixPath), QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

                    QStringList args;
                    args << "-rf";
                    args << prefixPath;
                    Process exportProcess(args, "/bin/rm", QDir::homePath(), tr("Removing old fake drive.<br>This may take awhile..."), tr("Removing old fake drive"));

                    if (exportProcess.exec()!=QDialog::Accepted){
                        return;
                    }
                } else {
                    return;
                }
            }
            FakeDriveSettings settings(this->cbPrefixes->currentText());
            settings.loadDefaultPrefixSettings();

            if (settings.exec()==QDialog::Accepted){
                emit(updateDatabaseConnections());
            }
        } else if (item->text() == tr("Update Fake Drive")){
            if (!sysreg_file.exists()){
                QMessageBox::warning(this, tr("Error"), tr("Sorry, no fake drive configuration found.<br>Create fake drive configuration before update it!"));
            } else {
                FakeDriveSettings settings(cbPrefixes->currentText());
                settings.loadPrefixSettings();

                if (settings.exec()==QDialog::Accepted){
                    emit(updateDatabaseConnections());
                }
            }
        }
    } if (this->provider == "Winetricks"){
        winetricks wine_t(this->cbPrefixes->currentText());
        if (item->text() == tr("Refresh Winetricks application list")){
            wine_t.parse();
            this->createTree();
        } else if (item->text() == tr("Install or update Winetricks script")){
            wine_t.install_winetricks();
        } else {
            wine_t.run_winetricks(item->text());
        }
    }
    return;
}

bool PrefixConfigWidget::eventFilter(QObject *obj, QEvent *event)
 {
     if (obj == this->treeWidget.get()) {
         if (event->type() == QEvent::KeyPress) {
             QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

             if (keyEvent->matches(QKeySequence::Delete)){
                 return true;
             }
         } else {
             return false;
         }
     }
     // pass the event on to the parent class
     return QWidget::eventFilter(obj, event);
 }

void PrefixConfigWidget::setPrefix(QString prefix){
    this->cbPrefixes.get()->setCurrentIndex(cbPrefixes.get()->findText(prefix));
}

void PrefixConfigWidget::searchClear_Click(){
    this->searchField->setText("");
    this->get_icons();
}

void PrefixConfigWidget::sortAlpha_Click(){
    if (this->sort_order == D_SORT_TYPE_BY_NAME_ASC){
        this->sort_order = D_SORT_TYPE_BY_NAME_DSC;
        sortAlpha->setStatusTip(tr("Alphabetic sort ascending"));
        sortAlpha->setText(tr("Alphabetic sort ascending"));
        sortAlpha->setIcon(CoreLib->loadIcon("data/sort-asc.png"));
    } else {
        this->sort_order = D_SORT_TYPE_BY_NAME_ASC;
        sortAlpha->setStatusTip(tr("Alphabetic sort descending"));
        sortAlpha->setText(tr("Alphabetic sort descending"));
        sortAlpha->setIcon(CoreLib->loadIcon("data/sort-desc.png"));
    }
    this->get_icons();
}

void PrefixConfigWidget::searchFilterChange(QString){
    this->get_icons();
}

void PrefixConfigWidget::prefixManage_Click(){
    emit(setTabIndex(3));
}
