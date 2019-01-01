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

#include "run.h"

Run::Run(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    // Setup base UI
    setupUi(this);
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

    setWindowIcon(CoreLib->loadIcon(CoreLib->getSetting("app", "icon", false, "q4wine").toString()));

    loadThemeIcons();

    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
    connect(cmdGetProgramBin, SIGNAL(clicked()), this, SLOT(cmdGetProgram_Click()));
    connect(cmdGetWorkDir, SIGNAL(clicked()), this, SLOT(cmdGetWorkDir_Click()));
    connect(cmdAdd, SIGNAL(clicked()), this, SLOT(cmdAdd_Click()));
    connect(cmdHelp, SIGNAL(clicked()), this, SLOT(cmdHelp_Click()));
    connect(comboPrefixes, SIGNAL(currentIndexChanged (int)), this, SLOT(comboPrefixes_indexChanged (int)));
    connect(cbUseConsole, SIGNAL(stateChanged(int)), this, SLOT(cbUseConsole_stateChanged(int)));

    QString res = CoreLib->getSetting("advanced", "defaultDesktopSize", false, "").toString();
    if (res.isEmpty()){
        cboxDesktopSize->setCurrentIndex(0);
    } else {
        int i = cboxDesktopSize->findText(res);
        if (i >= 0) {
            cboxDesktopSize->setCurrentIndex(i);
        } else {
            cboxDesktopSize->setCurrentText(res);
        }
    }

    twDlls->horizontalHeader()->setStretchLastSection(true);
    twDlls->setColumnWidth(0, 400);

    cmdGetProgramBin->installEventFilter(this);
    cmdGetWorkDir->installEventFilter(this);
    cmdOk->setFocus(Qt::ActiveWindowFocusReason);

    return;
}

void Run::prepare(QString prefix_name, QString wrkdir, QString override, QString winedebug, QString useconsole, QString display, QString cmdargs, QString desktop, int nice, QString exec, QString lang)
{
    if (!wrkdir.isEmpty())
        txtWorkDir->setText(wrkdir);

    if (!winedebug.isEmpty())
        txtWinedebug->setText(winedebug);

    if (!display.isEmpty())
        txtDisplay->setText(display);

    if (!cmdargs.isEmpty())
        txtCmdArgs->setText(cmdargs);

    if (!lang.isEmpty())
        txtEnvLang->setText(lang);

    txtNice->setValue(nice);

    if (!exec.isEmpty())
        txtProgramBin->setText(exec);

    if (useconsole=="1")
        cbUseConsole->setChecked(true);

    if (!desktop.isEmpty()) {
        int i = cboxDesktopSize->findText(desktop);
        if (i >= 0) {
            cboxDesktopSize->setCurrentIndex(i);
        } else {
            cboxDesktopSize->setCurrentText(desktop);
        }
    }

    if (!override.isEmpty()){
        QStringList overrideS = override.split(";");

        QString overrideorder;

        for (int i=0; i<overrideS.count()-1; i++){

            QStringList list2 = overrideS.at(i).split("=");
            twDlls->insertRow (0);
            std::unique_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(list2.at(0)));
            newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            twDlls->setItem(0, 0, newItem.release());

            if (list2.at(1)=="n")
                overrideorder = tr("Native");
            if (list2.at(1)=="b")
                overrideorder = tr("Built-in");
            if (list2.at(1)=="n,b")
                overrideorder = tr("Native, Built-in");
            if (list2.at(1)=="b,n")
                overrideorder = tr("Built-in, Native");

            newItem.reset(new QTableWidgetItem(overrideorder));
            newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            twDlls->setItem(0, 1, newItem.release());
        }
    }

    this->prefix_name = prefix_name;
    getPrefixes();
    return;
}

void Run::loadThemeIcons(){
    cmdGetProgramBin->setIcon(CoreLib->loadIcon("document-open"));
    cmdGetWorkDir->setIcon(CoreLib->loadIcon("document-open"));
    return;
}

void Run::cmdCancel_Click(){
    reject();
    return;
}

void Run::cmdAdd_Click(){
    if (!cboxDlls->currentText().isEmpty()){
        twDlls->insertRow (0);
        std::unique_ptr<QTableWidgetItem> newItem (new QTableWidgetItem(cboxDlls->currentText()));
        newItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
        twDlls->setItem(0, 0, newItem.release());

        std::unique_ptr<QComboBox> overidebox (new QComboBox(twDlls));
        overidebox->addItem(tr("Native"));
        overidebox->addItem(tr("Built-in"));
        overidebox->addItem(tr("Native, Built-in"));
        overidebox->addItem(tr("Built-in, Native"));
        overidebox->addItem(tr("Disabled"));
        overidebox->setCurrentIndex(overidebox->findText(cboxOveride->currentText()));

        twDlls->setCellWidget( 0, 1, overidebox.release());
    }

    return;
}

void Run::cmdOk_Click(){

    if (txtProgramBin->text().isEmpty()){
        QMessageBox::warning(this, tr("Error"), tr("No program name specified."));
        return;
    }

    if (!QDir(txtWorkDir->text()).exists()){
        QMessageBox::warning(this, tr("Error"), tr("Selected working directory does not exist."));
        return;
    }

    QString override="";
    for (int i=0; i<=twDlls->rowCount()-1; i++){
        override.append(QString("%1=").arg(twDlls->item(i, 0)->text()));
        QString method = twDlls->cellWidget(i, 1)->property("currentText").toString();

        if (method==tr("Native"))
            override.append("n;");
        if (method==tr("Disabled"))
            override.append(";");
        if (method==tr("Built-in"))
            override.append("b;");
        if (method==tr("Native, Built-in"))
            override.append("n,b;");
        if (method==tr("Built-in, Native"))
            override.append("b,n;");
    }

    execObj.execcmd = txtProgramBin->text();

    if (cbUseConsole->checkState()==Qt::Checked){
        execObj.useconsole = "1";
    } else {
        execObj.useconsole = "0";
    }
    execObj.cmdargs = txtCmdArgs->text();
    execObj.override = override;
    execObj.winedebug = txtWinedebug->text();
    execObj.display = txtDisplay->text();
    execObj.wrkdir = txtWorkDir->text();
    execObj.nice = txtNice->text();
    execObj.lang = txtEnvLang->text();

    if (cboxDesktopSize->currentText()==tr("No virtual desktop")){
        execObj.desktop="";
    } else {
        QRegExp res_match("^\\d{3,}x\\d{3,}$");
        if (!res_match.exactMatch(cboxDesktopSize->currentText())){
            QMessageBox::warning(this, tr("Error"), tr("Invalid virtual desktop size."));
            return;
        }
        execObj.desktop=cboxDesktopSize->currentText();
    }

    db_last_run_icon.addIcon(execObj.cmdargs, execObj.execcmd, execObj.override, execObj.winedebug, execObj.useconsole, execObj.display, execObj.wrkdir, execObj.desktop, execObj.nice.toInt(), execObj.lang);

    CoreLib->runWineBinary(execObj, comboPrefixes->currentText());

    accept();
    return;
}

bool Run::eventFilter( QObject *object, QEvent *event )
{
    //  firstly, check whether the object is the QTableWidget and if it's a mouse press event
    if (object == twDlls)
        if (event->type() == QEvent::KeyPress)
        {   // if yes, we need to cast the event
        std::unique_ptr<QKeyEvent> keyEvent (static_cast<QKeyEvent*>(event));
        if (keyEvent->key()==Qt::Key_Delete)
            twDlls->removeRow(twDlls->currentRow());
        keyEvent.release();
        return true;
    }

    return QWidget::eventFilter(object, event);
}

void Run::getWineDlls(QString winelibs_path){
    /*
     * This function Builds Wine dll list for selected prefix
     */
    cboxDlls->clear();
    cboxDlls->addItems (CoreLib->getWineDlls(winelibs_path));
    cboxDlls->setMaxVisibleItems (10);

    return;
}

void Run::getPrefixes(){
    /*
     * Getting prefixes and set default
     */
    QStringList list = db_prefix.getPrefixList();
    for (int i = 0; i < list.size(); ++i) {
        comboPrefixes->addItem(list.at(i));
        if (list.at(i)==this->prefix_name){
            comboPrefixes->setCurrentIndex ( comboPrefixes->findText(list.at(i)) );
            QHash<QString,QString> pref = db_prefix.getByName(prefix_name);
            prefix_dir = pref.value("path");
        }
    }
    return;
}

void Run::comboPrefixes_indexChanged (int){
    /*
     * If user select prefix -- rebuild wine dlls list
     */
    QHash<QString,QString> result = db_prefix.getByName(comboPrefixes->currentText());
    QString libs_path;

    if (result.value("path").isEmpty()){
        prefix_dir = QDir::homePath();
        prefix_dir.append("/.wine/drive_c/");
    } else {
        prefix_dir = result.value("path");
        libs_path = result.value("libs");
    }

    if (libs_path.isEmpty()){
        Version vers;
        vers.id_ = result.value("version_id");
        if (vers.load()){
            if (result.value("arch") == "win32"){
                libs_path = vers.wine_dllpath32_;
            } else if (result.value("arch") == "win64"){
                libs_path = vers.wine_dllpath64_;
            } else {
                if (vers.wine_dllpath64_.isEmpty()){
                    libs_path = vers.wine_dllpath32_;
                } else {
                    libs_path = vers.wine_dllpath64_;
                }
            }
        }
    }

    getWineDlls(libs_path);

    return;
}

/*************************************************************\
|		CheckBox Events	    					  |
\*************************************************************/

void Run::cbUseConsole_stateChanged(int){
    switch(cbUseConsole->checkState()){
 case Qt::Checked:
        txtWinedebug->setEnabled(true);
        break;
 default:
        txtWinedebug->setEnabled(false);
        break;
    }

    return;
}

void Run::cmdGetProgram_Click(){

    QString fileName, searchPath;

    if (txtProgramBin->text().isEmpty()){
        searchPath = prefix_dir;
    } else {
        if (QFile(txtProgramBin->text()).exists()){
            searchPath = QFileInfo(txtProgramBin->text()).absolutePath();
        } else {
            searchPath = prefix_dir;
        }
    }

    QFileDialog dialog(this);
    dialog.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden);
    dialog.setWindowTitle(tr("Open Exe file"));
    dialog.setDirectory(searchPath);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Exe, MSI, BAT files (*.exe *.EXE *.msi *.MSI *.bat *.BAT);;Exe files (*.exe *.EXE);;MSI files (*.msi *.MSI);;BAT files (*.bat *.BAT);;All files (*)"));
    //dialog.setSidebarUrls(prefix_urls);

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

    if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
        QStringList list1 = fileName.split("/");
        txtProgramBin->setText(fileName);

        QString wrkDir="";
        wrkDir = fileName.left(fileName.length() - list1.last().length());
        txtWorkDir->setText(wrkDir);
    }

    return;
}

void Run::cmdGetWorkDir_Click(){

    QString fileName, searchPath;

    if (txtWorkDir->text().isEmpty()){
        searchPath = prefix_dir;
    } else {
        if (QDir(txtWorkDir->text()).exists()) {
            searchPath = txtWorkDir->text();
        } else {
            searchPath = prefix_dir;
        }
    }

    QList<QUrl> prefix_urls;
    // Adding side bar urls for FileOpen dialogs
    if (QDir(prefix_dir).exists())
        prefix_urls << QUrl::fromLocalFile(prefix_dir);

    if ((searchPath != prefix_dir) && (QDir(searchPath).exists()))
        prefix_urls << QUrl::fromLocalFile(searchPath);

    prefix_urls << QUrl::fromLocalFile(QDir::homePath());
    prefix_urls << QUrl::fromLocalFile(QDir::rootPath());

    QFileDialog dialog(this);
    dialog.setFilter(QDir::Dirs | QDir::Hidden);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setWindowTitle(tr("Open Directory"));
    dialog.setDirectory(searchPath);
    // This option wirksonly it qt 4.5. In fact this not works correctly with QDir::Hidden,  so I comment it out for a some  time
    //dialog.setOption(QFileDialog::ShowDirsOnly, true);

#if QT_VERSION >= 0x040500
      if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0){
          dialog.setOptions(QFileDialog::DontUseNativeDialog);
      }
#endif

    dialog.setSidebarUrls(prefix_urls);
    if (dialog.exec())
        fileName = dialog.selectedFiles().first();

    if(!fileName.isEmpty()){
        txtWorkDir->setText(fileName);
    }
    return;
}

void Run::cmdHelp_Click(){
    QString rawurl="";
    switch (twbGeneral->currentIndex()){
 case 0:
        rawurl = "12-run-dialog.html#general";
        break;
 case 1:
        rawurl = "12-run-dialog.html#override";
        break;
 case 2:
        rawurl = "12-run-dialog.html#advanced";
        break;
 default:
        return;
        break;
    }

    CoreLib->openHelpUrl(rawurl);
}

