/***************************************************************************
 *   Copyright (C) 2008-2019 by Oleksii S. Malakhov <brezerk@gmail.com>    *
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

#include "prefixtreewidget.h"

PrefixTreeWidget::PrefixTreeWidget(QWidget *parent) :
      QTreeWidget(parent)
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

      //this->setHeaderLabel(tr("Current prefixes list"));
      this->header()->close();

      connect(this, SIGNAL(itemClicked (QTreeWidgetItem *, int)), this, SLOT(itemClicked (QTreeWidgetItem *, int)));

      this->dirName="";
      this->prefixName="";
      this->prefixMontPoint="";

      //Disabled due to buggy logic
      this->setAcceptDrops(false);

      this->cdromDevices=CoreLib->getCdromDevices();
}

PrefixTreeWidget::~PrefixTreeWidget(){
    std::unique_ptr<QTreeWidgetItem> treeItem (this->currentItem());

    QSettings settings(APP_SHORT_NAME, "default");
    settings.beginGroup("LastPrefix");
    if (treeItem.get()){
        if (treeItem->parent()){
            settings.setValue("prefix", treeItem->parent()->text(0));
            settings.setValue("dir", treeItem->text(0));
        } else {
            settings.setValue("prefix", treeItem->text(0));
            settings.setValue("dir", "");
        }
    } else {
        settings.setValue("prefix", "");
        settings.setValue("dir", "");
    }

    treeItem.release();
    settings.endGroup();
    return;
}

void PrefixTreeWidget::itemClicked (QTreeWidgetItem *item, int){
      if (!item)
            return;

      if (item->parent()){
            this->prefixName=item->parent()->text(0);
            this->dirName=item->text(0);
      } else {
            this->prefixName=item->text(0);
            this->dirName="";
      }

      this->prefixMontPoint = db_prefix.getMountPoint(this->prefixName);

      emit(showFolderContents(this->prefixName, this->dirName));
      emit(prefixIndexChanged(this->prefixName));
      emit(setSearchFocus());
      return;
}

void PrefixTreeWidget::dirAdd_Click(void){
      bool ok=false;
      QString dirname = QInputDialog::getText(this, tr("Enter new directory name"), tr("Directory name:"), QLineEdit::Normal, "" , &ok);
      std::unique_ptr<QTreeWidgetItem> treeItem (this->currentItem());

      if (!treeItem.get()){
            treeItem.release();
            return;
      }

      if (ok && !dirname.isEmpty()){
           while (db_dir.isExistsByName(this->prefixName, dirname)){
                dirname = QInputDialog::getText(this, tr("Sorry. It seems that the directory already exists."), tr("Sorry. It seems that the directory already exists.<br>Please choose another name or cancel the operation."), QLineEdit::Normal, dirname, &ok);
                if ((!ok) || (dirname.isEmpty())){
                    treeItem.release();
                    return;
                }
           }

            if (!db_dir.addDir(this->prefixName, dirname)){
                  treeItem.release();
                  return;
            }
#ifndef _OS_DARWIN_
            if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                sys_menu.create_dir_info(this->prefixName, dirname);
                sys_menu.writeXMLSystemMenu();
            }
#endif

            if (treeItem->parent()){
                std::unique_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(treeItem->parent()));
                prefixItem->setText(0, dirname);
                if (dirname=="import"){
                    prefixItem->setIcon(0, CoreLib->loadIcon("folder-cyan"));
                } else {
                    prefixItem->setIcon(0, CoreLib->loadIcon("folder"));
                }
                prefixItem.release();
            } else {
                std::unique_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(treeItem.get()));
                prefixItem->setText(0, dirname);
                if (dirname=="import"){
                    prefixItem->setIcon(0, CoreLib->loadIcon("folder-cyan"));
                } else {
                    prefixItem->setIcon(0, CoreLib->loadIcon("folder"));
                }
                prefixItem.release();
            }


      }
      treeItem.release();
      return;
}

void PrefixTreeWidget::dirRename_Click(void){
    std::unique_ptr<QTreeWidgetItem> treeItem (this->currentItem());

      if (!treeItem.get())
            return;

      if (treeItem->parent()){
            bool ok = false;
            QString newName = QInputDialog::getText(this, tr("Enter new name for directory"), tr("Directory name:"), QLineEdit::Normal, treeItem->text(0) , &ok);

            if (ok && !newName.isEmpty()){
                  while (db_dir.isExistsByName(treeItem->parent()->text(0), newName)){
                        newName = QInputDialog::getText(this, tr("Sorry. It seems that the directory already exists."), tr("Sorry. It seems that the directory already exists.<br>Please choose another name or cancel the operation."), QLineEdit::Normal, newName, &ok);
                        if ((!ok) || (newName.isEmpty())){
                            treeItem.release();
                            return;
                        }
                  }
                  db_dir.renameDir(treeItem->text(0), treeItem->parent()->text(0), newName);
#ifndef _OS_DARWIN_
                  if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                      sys_menu.move_dir_info(treeItem->parent()->text(0), treeItem->text(0), newName);
                      sys_menu.writeXMLSystemMenu();
                  }
#endif
                  treeItem->setText(0, newName);
                  this->itemClicked(treeItem.get(), 0);
            }

      }
      treeItem.release();
      return;
}

void PrefixTreeWidget::dirDelete_Click(void){
    std::unique_ptr<QTreeWidgetItem> treeItem (this->currentItem());

    if (!treeItem.get())
        return;

    if (treeItem->parent()){
        if (QMessageBox::warning(this, tr("Q4Wine"), tr("Do you really wish to delete the folder named \"%1\" and all associated icons?\n").arg(treeItem->text(0)),
                                 QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes){

            if (db_icon.delIcon(this->prefixName, this->dirName, ""))
                if (db_dir.delDir(this->prefixName, this->dirName)){
#ifndef _OS_DARWIN_
                    if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                        sys_menu.remove_dir_info(this->prefixName, this->dirName);
                        sys_menu.writeXMLSystemMenu();
                    }
#endif
                    treeItem->parent()->removeChild(treeItem.get());
                }
            this->itemClicked(this->currentItem(), 0);

        }
    }
    treeItem.release();
    return;
}

void PrefixTreeWidget::getPrefixes(){
      this->clear();

      QStringList list = db_prefix.getPrefixList();
      for (int i = 0; i < list.size(); ++i) {
            // Inserting root items into programs tree view
            std::unique_ptr<QTreeWidgetItem> prefixItem (new QTreeWidgetItem(this));
            prefixItem->setText(0, QString("%1").arg(list.at(i)));
            prefixItem->setIcon(0, CoreLib->loadIcon("wine"));
            if (CoreLib->getSetting("TreeWidget", "State", false, D_TREE_EXPAND).toInt()==D_TREE_EXPAND){
                prefixItem->setExpanded (true);
            } else {
                prefixItem->setExpanded (false);
            }
            this->addTopLevelItem(prefixItem.get());

            // Inserting subfolders items into programs tree view
            QStringList subresult = db_dir.getDirList(list.at(i));
            for (int j = 0; j < subresult.size(); ++j) {
                  std::unique_ptr<QTreeWidgetItem> subPrefixItem (new QTreeWidgetItem(prefixItem.get(), 0));
                  subPrefixItem->setText(0, QString("%1").arg(subresult.at(j)));
                  if (subresult.at(j)=="import"){
                      subPrefixItem->setIcon(0, CoreLib->loadIcon("folder-cyan"));
                  } else {
                      subPrefixItem->setIcon(0, CoreLib->loadIcon("folder"));
                  }
                  subPrefixItem.release();
            }
            prefixItem.release();
      }

      return;
}

void PrefixTreeWidget::contextMenuEvent (QContextMenuEvent * event){
      if (this->prefixName.isEmpty())
            return;

      this->itemClicked (this->currentItem(), 0);

      std::unique_ptr<QMenu> menu (new QMenu(this));
      std::unique_ptr<QMenu> menuMount (new QMenu(tr("Mount ISO..."), this));

      if (this->prefixMontPoint.isEmpty()){
            menuMount->setEnabled(false);
      } else {
            std::unique_ptr<QMenu> submenuMount (new QMenu(tr("Mount [%1]").arg(CoreLib->getMountedImages(this->prefixMontPoint)), this));
            std::unique_ptr<QAction> entry;


            if (this->cdromDevices.count() > 0){
                QString drive;
                foreach (drive, this->cdromDevices){
                    entry.reset (new QAction(CoreLib->loadIcon("drive-optical"), drive, this));
                    entry->setStatusTip(drive);
                    submenuMount->addAction(entry.release());
                }
            } else {
                entry.reset (new QAction(CoreLib->loadIcon("drive-optical"), tr("[none]"), this));
                entry->setStatusTip(tr("No media drives detected."));
                submenuMount->addAction(entry.release());
            }

            QList<QStringList> images = db_image.getFields();

            if (images.size()>0)
                  submenuMount->addSeparator();

            for (int i = 0; i < images.size(); ++i) {
                  entry.reset (new QAction(CoreLib->loadIcon("media-optical"), images.at(i).at(0), this));
                  entry->setStatusTip(tr("Mount media image."));
                  submenuMount->addAction(entry.release());
            }

            connect(submenuMount.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuMount_triggered(QAction*)));
            menuMount->addMenu(submenuMount.release());
            menuMount->addSeparator();

            submenuMount.reset (new QMenu(tr("Mount ..."), this));

            entry.reset (new QAction(CoreLib->loadIcon("drive-optical"), tr("Browse..."), this));
            entry->setStatusTip(tr("Browse for media image."));
            submenuMount->addAction(entry.release());

            QStringList files = CoreLib->getSetting("", "recent_images", false).toStringList();

            if (files.size()>0)
                  submenuMount->addSeparator();

            for (int i = 0; i < files.size(); ++i){
                  entry.reset (new QAction(CoreLib->loadIcon("media-optical"),files.at(i).split("/").last(), this));
                  entry->setStatusTip(files.at(i));
                  submenuMount->addAction(entry.release());
            }

            connect(submenuMount.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuMount_triggered(QAction*)));
            menuMount->addMenu(submenuMount.release());
            menuMount->addSeparator();

            entry.reset (new QAction(tr("Unmount [%1]").arg(this->prefixMontPoint), this));
            entry->setStatusTip(tr("Unmount media."));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuUmount_Click()));
            menuMount->addAction(entry.release());

      }

      // Default menu
            std::unique_ptr<QMenu> menuRun (new QMenu(tr("Run..."), this));

            std::unique_ptr<QAction> entry (new QAction(CoreLib->loadIcon("document-open"), tr("Browse ..."), this));
            entry->setStatusTip(tr("Browse for wine binary"));

            connect(menuRun.get(), SIGNAL(triggered(QAction*)), this, SLOT(menuRun_triggered(QAction*)));
            menuRun->addAction(entry.release());

            Last_Run_Icon db_last_run_icon;
            QStringList result = db_last_run_icon.getIcons();

            if (result.size()>0){
                  menuRun->addSeparator();
                  for (int i=0; i<result.size(); ++i){
                        entry.reset(new QAction(result.at(i).split("/").last(), this));
                        entry->setStatusTip(result.at(i));
                        menuRun->addAction(entry.release());
                  }
            }

            menu->addMenu(menuRun.release());
            menu->addSeparator();

            entry.reset(new QAction(tr("New directory"), this));
            entry->setStatusTip(tr("Create new directory"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(dirAdd_Click()));
            menu->addAction(entry.release());

            if (this->currentItem()->parent()){
                entry.reset(new QAction(tr("Rename"), this));
                entry->setStatusTip(tr("Rename current directory"));
                connect(entry.get(), SIGNAL(triggered()), this, SLOT(dirRename_Click()));
                menu->addAction(entry.release());

                entry.reset(new QAction(tr("Delete"), this));
                entry->setStatusTip(tr("Delete current directory"));
                connect(entry.get(), SIGNAL(triggered()), this, SLOT(dirDelete_Click()));
                menu->addAction(entry.release());
            }

            menu->addSeparator();
            menu->addMenu(menuMount.release());

            menu->addSeparator();

            std::unique_ptr<QMenu> subMenu (new QMenu(tr("Browser"), this));
            entry.reset(new QAction(CoreLib->loadIcon("drive-harddisk"), tr("Open prefix directory"), this));
            entry->setStatusTip(tr("Open prefix directory in system file browser"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenPrefixDir_Click()));
            subMenu->addAction(entry.release());

            entry.reset(new QAction(CoreLib->loadIcon("drive-optical"), tr("Open mount point directory"), this));
            entry->setStatusTip(tr("Open mount point directory in system file browser"));
            if (this->prefixMontPoint.isEmpty())
                  entry->setEnabled(false);
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(xdgOpenMountDir_Click()));
            subMenu->addAction(entry.release());

            menu->addSeparator();
            menu->addMenu(subMenu.release());

            subMenu.reset(new QMenu(tr("Wine Browser"), this));
            entry.reset(new QAction(CoreLib->loadIcon("drive-harddisk"), tr("Open prefix directory"), this));
            entry->setStatusTip(tr("Open prefix directory in wine file browser"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenPrefixDir_Click()));
            subMenu->addAction(entry.release());

            entry.reset(new QAction(CoreLib->loadIcon("drive-optical"), tr("Open mount point directory"), this));
            entry->setStatusTip(tr("Open mount point directory in wine file browser"));
            if (this->prefixMontPoint.isEmpty())
                  entry->setEnabled(false);
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(winefileOpenMountDir_Click()));
            subMenu->addAction(entry.release());

            menu->addMenu(subMenu.release());

                        subMenu.reset(new QMenu(tr("Console"), this));

                        entry.reset(new QAction(CoreLib->loadIcon("utilities-terminal"), tr("Open console in prefix directory"), this));
                        entry->setStatusTip(tr("Open system console in prefix directory"));
                        connect(entry.get(), SIGNAL(triggered()), this, SLOT(consoleToPrefixDir_Click()));
                        subMenu->addAction(entry.release());

                        entry.reset(new QAction(CoreLib->loadIcon("utilities-terminal"), tr("Open console in mount point directory"), this));
                        entry->setStatusTip(tr("Open system console in mount point directory"));
                        if (this->prefixMontPoint.isEmpty())
                                  entry->setEnabled(false);
                        connect(entry.get(), SIGNAL(triggered()), this, SLOT(consoleToMountDir_Click()));
                        subMenu->addAction(entry.release());

                        menu->addMenu(subMenu.release());


            menu->addSeparator();

            entry.reset(new QAction(CoreLib->loadIcon("document-new"), tr("Create new prefix"), this));
            entry->setStatusTip(tr("Create new prefix"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuCreatePrefix_Click()));
            menu->addAction(entry.release());

            entry.reset(new QAction(CoreLib->loadIcon("document-edit"), tr("Configure prefix settings"), this));
            entry->setStatusTip(tr("Edit prefix settings"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuConfigurePrefix_Click()));
            menu->addAction(entry.release());

            entry.reset(new QAction(CoreLib->loadIcon("configure"), tr("Configure fake drive settings"), this));
            entry->setStatusTip(tr("Set up prefix fake drive and applications"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuSetupPrefix_Click()));
            menu->addAction(entry.release());

            menu->addSeparator();

            entry.reset(new QAction(CoreLib->loadIcon("edit-delete"), tr("Delete prefix"), this));
            entry->setStatusTip(tr("Delete prefix"));
            connect(entry.get(), SIGNAL(triggered()), this, SLOT(menuDeletePrefix_Click()));
            menu->addAction(entry.release());

      menu->exec(QCursor::pos());
      return;
}

void PrefixTreeWidget::keyReleaseEvent ( QKeyEvent * event ){
    if (event->key()==Qt::Key_F2){
        dirRename_Click();
    } else if (event->key() == Qt::Key_V && event->modifiers() & Qt::ControlModifier) {
       emit(pasteAction());
    } else if (event->key()==Qt::Key_Delete){
        dirDelete_Click();
    } else if (event->key()==Qt::Key_Return){
        if (this->selectedItems().count()>0)
            this->itemClicked(this->selectedItems().at(0), 0);
    } else {
        QTreeWidget::keyPressEvent(event);
    }
}

bool PrefixTreeWidget::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{
    QList<QUrl> urlList;

    urlList = data->urls(); // retrieve list of urls

    foreach(QUrl url, urlList) // iterate over list
    {
        if (!parent)
              return false;

        if (parent->parent()){
              moveDesktopFile(url, parent->parent()->text(0), parent->text(0));
        } else {
              moveDesktopFile(url, parent->text(0), "");
        }
    }

    if (this->selectedItems().count()>0)
        this->itemClicked(this->selectedItems().at(0), 0);

    return true;
}

void PrefixTreeWidget::moveDesktopFile(QUrl url, QString prefixName, QString dirName){

    QFile file(url.path());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString prefix="", dir="", icon="";

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(QRegExp("^Exec=.*q4wine-cli.* "))){
            QStringList parse = line.split(" ");
            for (int i=1; i<parse.count(); i++){
                if (parse.at(i)=="-p"){
                    prefix=parse.at(++i);
                    prefix.replace("\"", "");
                } else if (parse.at(i)=="-d"){
                    dir=parse.at(++i);
                    dir.replace("\"", "");
                } else if (parse.at(i)=="-i"){
                    icon=parse.at(++i);
                    icon.replace("\"", "");
                }
            }
        }
    }

    if (icon.isEmpty())
        return;

    if (prefix.isEmpty())
        return;

    bool ok=false;
    QString newName = icon;

    while (db_icon.isExistsByName(prefixName, dirName, newName)){
          newName = QInputDialog::getText(this, tr("Sorry. It seems that the icon already exists."), tr("Sorry. It seems that the icon already exists.<br>Please choose another name or cancel the operation."), QLineEdit::Normal, icon , &ok);
          if (!ok){
                return;
          }
    }
    if (!db_icon.updateIcon(newName, db_prefix.getId(prefixName), db_dir.getId(dirName, prefixName), db_prefix.getId(prefix), db_dir.getId(dir, prefix), icon))
        return;

#ifndef _OS_DARWIN_
    if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
        CoreLib->deleteDesktopFile(prefix, dir, icon);
        CoreLib->createDesktopFile(prefixName, dirName, newName, true);
        sys_menu.writeXMLSystemMenu();
    }
#endif
}

QStringList PrefixTreeWidget::mimeTypes () const
{
    QStringList qstrList;
    // list of accepted mime types for drop
    qstrList.append("text/uri-list");
    return qstrList;
}

void PrefixTreeWidget::dragEnterEvent(QDragEnterEvent *event){
#ifdef DEBUG
    qDebug()<<"[ii] mime"<<event->mimeData()->formats();
#endif
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

Qt::DropActions PrefixTreeWidget::supportedDropActions () const
{
    // returns what actions are supported when dropping
    return Qt::CopyAction;
}

void PrefixTreeWidget::menuRun_triggered(QAction* action){
      if (action->text().isEmpty())
            return;

      if (action->statusTip()==tr("Browse for wine binary")){
            Run run;
            run.prepare(this->prefixName);
            run.exec();
            return;
      }

      Last_Run_Icon db_last_run_icon;
      QStringList result = db_last_run_icon.getByExec(action->statusTip());

#ifdef DEBUG
      qDebug()<<"Config key: advanced.openRunDialog="<<CoreLib->getSetting("advanced", "openRunDialog", false, 0).toString();
#endif

      if (CoreLib->getSetting("advanced", "openRunDialog", false, 0).toInt()==0){
            ExecObject execObj;
            execObj.execcmd=action->statusTip();
            execObj.wrkdir=result.at(0);
            execObj.override=result.at(1);
            execObj.winedebug=result.at(2);
            execObj.useconsole=result.at(3);
            execObj.display=result.at(4);
            execObj.cmdargs=result.at(5);
            execObj.desktop=result.at(6);
            execObj.nice=result.at(7);
            execObj.lang=result.at(8);
            CoreLib->runWineBinary(execObj, this->prefixName);
      } else {
            Run run;
            run.prepare(this->prefixName, result.at(0), result.at(1), result.at(2), result.at(3), result.at(4), result.at(5), result.at(6), result.at(7).toInt(), action->statusTip(), result.at(8));
            run.exec();
            return;
      }
      return;
}

void PrefixTreeWidget::menuMount_triggered(QAction* action){
      /*
       * This slot process menuDirMountImages and menuIconMountImages triggered signal
       */

      if (action->text()==tr("[none]")){
            QMessageBox::warning(this, tr("Error"),  tr("No device drive specified in prefix settings."), QMessageBox::Ok);
            return;
      } else if (action->statusTip()==tr("Browse for media image.")) {
            /*
            Request for unmounting cdrom drve described at wine prefix settings
            */

          QString fileFilter;
#ifdef _OS_LINUX_
    fileFilter = tr("Disc image files (*.iso *.ISO *.nrg *.NRG *.img *.IMG *.bin *.BIN *.mdf *.MDF);;All files (*)");
#endif
#ifdef _OS_FREEBSD_
    fileFilter =  tr("ISO image files (*.iso *.ISO);;All files (*)");
#endif

#if QT_VERSION >= 0x040500
    QFileDialog::Options options;

    if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
        options = QFileDialog::DontUseNativeDialog;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Disc Image Files"), QDir::homePath(), fileFilter, 0, options);
#else
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Disc Image Files"), QDir::homePath(), fileFilter);
#endif

            if(fileName.isEmpty()){
                  return;
            }

            if (CoreLib->mountImage(fileName, this->prefixName)){
                  emit(changeStatusText(tr("%1 successfully mounted.").arg(fileName)));
            } else {
                  emit(changeStatusText(tr("Fail to mount %1.").arg(fileName)));
            }

            CoreLib->updateRecentImagesList(fileName);
            return;
      }

      bool ret=false;
      if (action->statusTip().contains("/")){
            ret=CoreLib->mountImage(action->statusTip(), this->prefixName);
      } else {
            ret=CoreLib->mountImage(action->text(), this->prefixName);
      }

      if (ret){
            emit(changeStatusText(tr("%1 successfully mounted.").arg(action->text())));
      } else {
            emit(changeStatusText(tr("Failed to mount %1.").arg(action->text())));
      }
      return;
}

void PrefixTreeWidget::menuUmount_Click(void){
      if (CoreLib->umountImage(this->prefixName)){
            emit(changeStatusText(tr("Drive successfully unmounted.")));
      } else {
            emit(changeStatusText(tr("Failed to unmount drive.")));
      }
      return;
}

void PrefixTreeWidget::menuCreatePrefix_Click(void){
    PrefixSettings settings;
    if (settings.exec()==QDialog::Accepted){
        emit(updateDatabaseConnections());
        emit(prefixIndexChanged(settings.getPrefixName()));
    }
    return;
}

void PrefixTreeWidget::menuConfigurePrefix_Click(void){
    PrefixSettings settings(this->prefixName);
    if (settings.exec()==QDialog::Accepted){
        emit(updateDatabaseConnections());
        emit(prefixIndexChanged(settings.getPrefixName()));
    }
    return;
}

void PrefixTreeWidget::menuSetupPrefix_Click(void){
    QString sysregPath, prefixPath;
    prefixPath = db_prefix.getPath(this->prefixName);

    sysregPath.append(prefixPath);
    sysregPath.append("/system.reg");

    FakeDriveSettings settings(this->prefixName);

    QFile sysreg_file (sysregPath);
    if (sysreg_file.exists()){
        settings.loadPrefixSettings();
    } else {
        settings.loadDefaultPrefixSettings();
    }

    if (settings.exec()==QDialog::Accepted){
        emit(updateDatabaseConnections());
    }
    return;
}

void PrefixTreeWidget::menuDeletePrefix_Click(void){
    if (this->prefixName=="Default"){
        QMessageBox::warning(this, tr("Error"), tr("Sorry, you cannot delete the Default prefix."), QMessageBox::Ok);
        return;
    }

    if(QMessageBox::warning(this, tr("Warning"), tr("Do you really wish to delete the prefix named \"%1\" and all associated icons?").arg(prefixName), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok){
        if (db_icon.delIconsByPrefixName(this->prefixName))
            if(db_dir.delDir(this->prefixName)){
                db_prefix.delByName(this->prefixName);
#ifndef _OS_DARWIN_
                if (CoreLib->getSetting("Plugins", "enableMenuDesktop", false, true).toBool()){
                    sys_menu.remove_dir_info(this->prefixName);
                    sys_menu.writeXMLSystemMenu();
                }
#endif
            }

        emit(updateDatabaseConnections());
    }
    emit(prefixIndexChanged("Default"));
}

void PrefixTreeWidget::xdgOpenPrefixDir_Click(void){
    QDesktopServices::openUrl(QUrl(QString("file://%1").arg(db_prefix.getPath(this->prefixName)), QUrl::TolerantMode));
    return;
}

void PrefixTreeWidget::xdgOpenMountDir_Click(void){
    QDesktopServices::openUrl(QUrl(QString("file://%1").arg(this->prefixMontPoint), QUrl::TolerantMode));
    return;
}

void PrefixTreeWidget::winefileOpenPrefixDir_Click(void){
      QString result = db_prefix.getPath(this->prefixName);
      result.append("/");

      ExecObject execObj;
      execObj.cmdargs = result;
      execObj.execcmd = "winefile";

      CoreLib->runWineBinary(execObj, this->prefixName);
      return;
}

void PrefixTreeWidget::winefileOpenMountDir_Click(void){
    ExecObject execObj;
    execObj.cmdargs = this->prefixMontPoint + "/";
    execObj.execcmd = "winefile";

    CoreLib->runWineBinary(execObj, this->prefixName);
    return;
}

void PrefixTreeWidget::consoleToPrefixDir_Click(void){
    if (this->prefixName.isEmpty())
        return;

    QString prefix_path = db_prefix.getPath(this->prefixName);

    if (prefix_path.isEmpty()){
        qDebug()<<"[EE] Cannot get prefix path";
    } else {
        CoreLib->openConsole(prefix_path, this->prefixName);
    }

    return;
}

void PrefixTreeWidget::consoleToMountDir_Click(void){
    if (this->prefixMontPoint.isEmpty())
        return;

    CoreLib->openConsole(this->prefixMontPoint, this->prefixName);
    return;
}

void PrefixTreeWidget::setDefaultFocus(QString prefixName, QString dirName){
    if (prefixName.isEmpty()){
        prefixName="Default";
        if (this->topLevelItemCount()>0){
            this->setCurrentItem(this->topLevelItem(0));
            this->itemClicked(this->topLevelItem(0), 0);
            return;
        }
    } else {
        //skip update
        if (prefixName == this->prefixName)
            return;

        for(int i=0; i < this->topLevelItemCount(); i++)
        {
            std::unique_ptr<QTreeWidgetItem> item(this->topLevelItem(i));
            if (item->text(0)==prefixName){
                if (dirName.isEmpty()){
                    this->setFocus();
                    this->setCurrentItem(item.get());
                    this->topLevelItem(i)->setExpanded(true);
                    this->itemClicked(item.get(), 0);
                    item.release();
                    break;
                } else {
                    for (int j=0; j < item->childCount(); j++){
                        std::unique_ptr<QTreeWidgetItem> childItem(item->child(j));
                        if (childItem->text(0)==dirName){
                            this->setCurrentItem(childItem.get());
                            this->itemClicked(childItem.get(), 0);
                        }
                        childItem.release();
                    }
                }
            }
            item.release();
        }
    }
    return;
}

void PrefixTreeWidget::expandTree(){
    this->expandAll();
}

void PrefixTreeWidget::collapseTree(){
    this->collapseAll();
}

