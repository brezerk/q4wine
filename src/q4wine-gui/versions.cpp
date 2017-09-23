/***************************************************************************
 *   Copyright (C) 2008-2011 by Oleksii Malakhov                            *
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

#include "src/q4wine-gui/versions.h"

VersionManager::VersionManager(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
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
    CoreLib.reset(static_cast<corelib *>(CoreLibClassPointer(true)));

    setupUi(this);
    setWindowTitle(tr("Wine Version Manager"));
    setWindowIcon(CoreLib->loadIcon(CoreLib->getSetting("app", "icon", false, "q4wine").toString()));

    createActions();
    loadThemeIcons();

    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));

    std::unique_ptr<QToolBar> tbPrefix (new QToolBar(this));
    tbPrefix->setIconSize(QSize(24, 24));
    tbPrefix->addAction(searchClear.get());

    searchField.reset(new QLineEdit(this));
    connect(searchField.get(), SIGNAL(textChanged(QString)), this, SLOT(searchFilterChange(QString)));
    tbPrefix->addWidget(searchField.get());

    //listPrefix.reset(new VersionListWidget(this));

    std::unique_ptr<QVBoxLayout> layout (new QVBoxLayout(wPrefix));
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(tbPrefix.release());
    layout->addWidget(listPrefix);

    wPrefix->setLayout(layout.release());

    std::unique_ptr<QToolBar> tbSettings (new QToolBar(this));
    tbSettings->setIconSize(QSize(24, 24));
    tbSettings->addAction(saveVersion.get());
    tbSettings->addSeparator();
    tbSettings->addAction(addVersion.get());
    tbSettings->addAction(delVersion.get());


    std::unique_ptr<QVBoxLayout> s_layout (new QVBoxLayout(wSettings));
    s_layout->setSpacing(0);
    s_layout->setContentsMargins(0,0,0,0);
    s_layout->addWidget(tbSettings.release());
    s_layout->addWidget(gSettings);

    wSettings->setLayout(s_layout.release());

    splitter.reset(new QSplitter(wContent));
    splitter->addWidget(wPrefix);
    splitter->addWidget(wSettings);

    QList<int> a;
    a.append(CoreLib->getSetting("VersionManager", "splitterSize0", false, 170).toInt());
    a.append(CoreLib->getSetting("VersionManager", "splitterSize1", false, 379).toInt());

    splitter->setSizes(a);

    std::unique_ptr<QVBoxLayout> g_layout (new QVBoxLayout(wContent));
    g_layout->setSpacing(0);
    g_layout->setContentsMargins(6,0,6,0);
    g_layout->addWidget(splitter.get());

    wContent->setLayout(g_layout.release());

    listPrefix->setContextMenuPolicy(Qt::CustomContextMenu);
    listPrefix->setSelectionMode(QAbstractItemView::SingleSelection);
    listPrefix->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listPrefix->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    listPrefix->setMovement(QListView::Static);
    int nSize = CoreLib->getSetting("IconWidget", "IconSize", false, 32).toInt();
    listPrefix->setViewMode(QListView::ListMode);
    listPrefix->setGridSize(QSize(nSize, nSize));
    listPrefix->setIconSize(QSize(nSize, nSize));
    listPrefix->setWrapping(false);
    listPrefix->setWordWrap(false);
    connect(listPrefix, SIGNAL(currentItemChanged (QListWidgetItem *, QListWidgetItem *)), this, SLOT(currentItemChanged (QListWidgetItem *, QListWidgetItem *)));

    cmdGetWineBin->installEventFilter(this);
    cmdGetWineLibs32->installEventFilter(this);
    cmdGetWineLibs64->installEventFilter(this);
    cmdGetWineLoaderBin->installEventFilter(this);
    cmdGetWineServerBin->installEventFilter(this);

    connect(txtWineBin, SIGNAL(textChanged(QString)), this, SLOT(versionSettingsChanged(QString)));
    connect(txtWineLibs32, SIGNAL(textChanged(QString)), this, SLOT(versionSettingsChanged(QString)));
    connect(txtWineLibs64, SIGNAL(textChanged(QString)), this, SLOT(versionSettingsChanged(QString)));
    connect(txtWineLoaderBin, SIGNAL(textChanged(QString)), this, SLOT(versionSettingsChanged(QString)));
    connect(txtWineServerBin, SIGNAL(textChanged(QString)), this, SLOT(versionSettingsChanged(QString)));

    currentVersion.reset(NULL);
    currentVersionChanged_ = false;

    getVersions();

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);
    return;
}

VersionManager::~VersionManager(){
    if (splitter->sizes().at(0) != splitter->sizes().at(1)){
        QSettings settings(APP_SHORT_NAME, "default");
        settings.beginGroup("VersionManager");
        settings.setValue("splitterSize0", splitter->sizes().at(0));
        settings.setValue("splitterSize1", splitter->sizes().at(1));
        settings.endGroup();
    }
}

void VersionManager::setVersionFocus(QString version){

    QList<QListWidgetItem *> items = listPrefix->findItems(version, Qt::MatchExactly);
    if (items.size() > 0){
        listPrefix->setCurrentItem(items.at(0));
        currentVersion->name_ = version;
        currentVersion->load();
    } else {
        delVersion->setEnabled(false);
        saveVersion->setEnabled(false);
        if (currentVersion.get())
            currentVersion->clear();
        cmdGetWineBin->setEnabled(false);
        cmdGetWineLibs32->setEnabled(false);
        cmdGetWineLibs64->setEnabled(false);
        cmdGetWineLoaderBin->setEnabled(false);
        cmdGetWineServerBin->setEnabled(false);
        txtWineBin->setEnabled(false);
        txtWineBin->clear();
        txtWineLibs32->setEnabled(false);
        txtWineLibs32->clear();
        txtWineLibs64->setEnabled(false);
        txtWineLibs64->clear();
        txtWineLoaderBin->setEnabled(false);
        txtWineLoaderBin->clear();
        txtWineServerBin->setEnabled(false);
        txtWineServerBin->clear();
    }
}

void VersionManager::cmdOk_Click()
{
    if (!askSaveChangeds())
        return;

    if (!fileExists(this->txtWineBin->text()))
        return;

    if (!fileExists(this->txtWineLoaderBin->text()))
        return;

    if (!fileExists(this->txtWineServerBin->text()))
        return;

    if (!this->txtWineLibs32->text().isEmpty())
        if (!fileExists(this->txtWineLibs32->text(), true))
            return;

    if (!this->txtWineLibs64->text().isEmpty())
        if (!fileExists(this->txtWineLibs64->text(), true))
            return;

    accept();
    return;
}

void VersionManager::cmdHelp_Click(){
    QString rawurl="14-version-manager-dialog.html";
    CoreLib->openHelpUrl(rawurl);
}

bool VersionManager::askSaveChangeds(){
    if (currentVersionChanged_){
        QMessageBox msgBox;
        msgBox.setText(tr("The Version settings has been modified."));
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch (msgBox.exec()){
        case QMessageBox::Save:
            return saveChanges();
            break;
        case QMessageBox::Cancel:
            return false;
            break;
        case QMessageBox::Discard:
            break;
        default:
            break;
        }
    }
    return true;
}

void VersionManager::addVersion_Click(){

    if (askSaveChangeds()){
        currentVersionChanged_ = false;
    } else {
        return;
    }

    bool ok;
    QString name;
    Version *vers = new Version();

    name = QInputDialog::getText(this, tr("Add version"),
                                 tr("Version name:"), QLineEdit::Normal,
                                 "", &ok);
    if (ok && !name.isEmpty()){
        vers->name_ = name;
        while (vers->load()){
            name = QInputDialog::getText(this, tr("Add version"),
                                         tr("Sorry. It seems that the version name already exists.<br>Please choose another version name."), QLineEdit::Normal,
                                         name, &ok);
            if (!ok || name.isEmpty())
                break;

            vers->clear();
            vers->name_ = name;
        }

        if (vers->save()){
            getVersions();
            setVersionFocus(vers->name_);
            currentVersionChanged_ = true;
        }
    }
}

void VersionManager::delVersion_Click(){
    if (askSaveChangeds()){
        currentVersionChanged_ = false;
    } else {
        return;
    }

    if (currentVersion->name_ == "Default"){
        QMessageBox::critical(this, tr("Error"),
                             tr("Default version can not be removed."));
        return;
    }

    currentVersion->remove();
    getVersions();
    setVersionFocus("Default");
}

void VersionManager::saveVersion_Click(){
    saveChanges();
}

bool VersionManager::saveChanges(){

    if (!fileExists(this->txtWineBin->text()))
        return false;

    if (!fileExists(this->txtWineLoaderBin->text()))
        return false;

    if (!fileExists(this->txtWineServerBin->text()))
        return false;

    if (!this->txtWineLibs32->text().isEmpty())
        if (!fileExists(this->txtWineLibs32->text(), true))
            return false;

    if (!this->txtWineLibs64->text().isEmpty())
        if (!fileExists(this->txtWineLibs64->text(), true))
            return false;

    currentVersion->wine_exec_ = this->txtWineBin->text();
    currentVersion->wine_loader_ = this->txtWineLoaderBin->text();
    currentVersion->wine_server_ = this->txtWineServerBin->text();
    currentVersion->wine_dllpath32_ = this->txtWineLibs32->text();
    currentVersion->wine_dllpath64_ = this->txtWineLibs64->text();

    if (currentVersion->save()){
        currentVersionChanged_ = false;
    } else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Can't save changes..."));
    }
    return true;
}

bool VersionManager::fileExists(QString path, bool check_dir){
    QFileInfo file(path);
    if (!file.exists()){
            QMessageBox::critical(this, tr("Error"),
                             tr("Path '%1' is not exists.").arg(path));
        return false;
    }
    if (check_dir){
        if (!file.isDir()){
            QMessageBox::critical(this, tr("Error"),
                                  tr("Path '%1' is not a directory.").arg(path));
            return false;
        }
    } else {
        if (!file.isFile()){
            QMessageBox::critical(this, tr("Error"),
                                  tr("Path '%1' is not a file.").arg(path));
            return false;
        }
    }
    return true;
}

void VersionManager::loadThemeIcons(){
    cmdGetWineBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineServerBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineLoaderBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineLibs32->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWineLibs64->setIcon(CoreLib->loadIcon("document-open"));
    return;
}

void VersionManager::createActions(){
    addVersion.reset(new QAction(CoreLib->loadIcon("document-new"), tr("Add version"), this));
    addVersion->setStatusTip(tr("Add version"));
    connect(addVersion.get(), SIGNAL(triggered()), this, SLOT(addVersion_Click()));
    delVersion.reset(new QAction(CoreLib->loadIcon("edit-delete"), tr("Remove version"), this));
    delVersion->setStatusTip(tr("Remove version"));
    connect(delVersion.get(), SIGNAL(triggered()), this, SLOT(delVersion_Click()));
    saveVersion.reset(new QAction(CoreLib->loadIcon("document-save"), tr("Save changes"), this));
    saveVersion->setStatusTip(tr("Save changes"));
    connect(saveVersion.get(), SIGNAL(triggered()), this, SLOT(saveVersion_Click()));
    searchClear.reset(new QAction(CoreLib->loadIcon("edit-clear"), tr("Clear search field"), this));
    searchClear->setStatusTip(tr("Clear search field"));
    connect(searchClear.get(), SIGNAL(triggered()), this, SLOT(searchClear_Click()));
}

void VersionManager::getVersions(){
    listPrefix->clear();

    Version ver;
    QList<Version> list = ver.load_all(searchField->text());
    std::unique_ptr<QListWidgetItem> iconItem;

    for (int i = 0; i < list.size(); i++){
        iconItem.reset(new QListWidgetItem(listPrefix, 0));
        iconItem->setText(list.at(i).name_);
        iconItem->setToolTip(list.at(i).name_);
        iconItem->setIcon(CoreLib->loadIcon("application-x-executable"));
        iconItem.release();
    }

    if (!currentVersion.get())
        setVersionFocus("Default");
}

void VersionManager::searchFilterChange(QString){
    getVersions();
}

void VersionManager::searchClear_Click(){
    searchField->clear();
}

void VersionManager::itemClicked (QListWidgetItem *item){
    if (!item)
        return;

    if (currentVersion.get())
        if (item->text() == currentVersion->name_)
            return;

    Version* ver = new Version();
    ver->name_ = item->text();

    emit(setVersion(ver->name_));

    if (!ver->load()){
        qDebug() << "[EE] Fail to load version by name: " << ver->name_;
        return;
    }

    currentVersion.reset(ver);

    this->txtWineBin->setText(currentVersion->wine_exec_);
    this->txtWineServerBin->setText(currentVersion->wine_server_);
    this->txtWineLoaderBin->setText(currentVersion->wine_loader_);
    this->txtWineLibs32->setText(currentVersion->wine_dllpath32_);
    this->txtWineLibs64->setText(currentVersion->wine_dllpath64_);

    delVersion->setEnabled(true);
    saveVersion->setEnabled(true);
    cmdGetWineBin->setEnabled(true);
    cmdGetWineLibs32->setEnabled(true);
    cmdGetWineLibs64->setEnabled(true);
    cmdGetWineLoaderBin->setEnabled(true);
    cmdGetWineServerBin->setEnabled(true);
    txtWineBin->setEnabled(true);
    txtWineLibs32->setEnabled(true);
    txtWineLibs64->setEnabled(true);
    txtWineLoaderBin->setEnabled(true);
    txtWineServerBin->setEnabled(true);

    currentVersionChanged_ = false;

    return;
}

void VersionManager::keyPressEvent (QKeyEvent * event){
    QListWidgetItem *item = listPrefix->currentItem();

    if (!item)
        return;

    if (event->key() == Qt::Key_Return)
        itemClicked(item);
}

bool VersionManager::eventFilter(QObject *obj, QEvent *event){
    /*
     * Select folder dialog
     */

    if (!obj->property("enabled").toBool())
        return false;
        //if (obj->property("enabled"))

    if (event->type() == QEvent::MouseButtonRelease) {

        QString file="";

        if (obj->objectName().right(3)=="Bin"){
#if QT_VERSION >= 0x040500
            QFileDialog::Options options;

            if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog;

            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "All files (*)", 0, options);
#else
            file = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "All files (*)");
#endif
        } else {
#if QT_VERSION >= 0x040500
            QFileDialog::Options options;

            if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog;

            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), options);
#else
            file = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::DontResolveSymlinks);
#endif
        }

        if (!file.isEmpty()){
            if (file.right(1)=="/"){
                file=file.left(file.length()-1);
            }

            QString a="";

            a.append("txt");
            a.append(obj->objectName().right(obj->objectName().length()-6));

            std::unique_ptr<QLineEdit> lineEdit (findChild<QLineEdit *>(a));

            if (lineEdit.get()){
                lineEdit->setText(file);
            } else {
                qDebug("Error");
            }

            lineEdit.release();
        }
    }

    return false;
}

void VersionManager::currentItemChanged (QListWidgetItem *item, QListWidgetItem *old_item){
    if (!item)
        return;

    if (currentVersion.get()){
        if (currentVersion->name_ != item->text()){
            if (askSaveChangeds()){
                currentVersionChanged_ = false;
            } else {
                QTimer::singleShot( 0, this, SLOT(restoreItemSelection()));
                return;
            }
        }
    }

    itemClicked(item);
    return;
}

void VersionManager::restoreItemSelection(){
    setVersionFocus(currentVersion->name_);
}

void VersionManager::versionSettingsChanged(QString){
    currentVersionChanged_ = true;
}
