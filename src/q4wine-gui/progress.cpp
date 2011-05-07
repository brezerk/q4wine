/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Malakhov Alexey                                 *
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

#include "progress.h"

Progress::Progress(int action, QString path, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    setupUi(this);

    // Loading libq4wine-core.so
#ifdef RELEASE
    libq4wine.setFileName("libq4wine-core");
#else
    libq4wine.setFileName("../q4wine-lib/libq4wine-core");
#endif
    if (!libq4wine.load()){
          libq4wine.load();
    }

    // Getting corelib calss pointer
    CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
    CoreLib.reset((corelib *)CoreLibClassPointer(true));

    this->max=0;
    this->current=0;
    this->action=action;
    this->path=path;

    t.reset(new QTimer(this));
    connect(t.get(), SIGNAL(timeout()), this, SLOT(runAction()));
    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));

    if (action==0){
        lblInfo->setText(tr("Importing wine desktop icons from:<br>\"%1\"<br><br>This can take a while...<br><br><b>Note:</b> To remove processed files see q4wine options dialog.").arg(path));
        setWindowTitle(tr("Importing wine desktop icons: %1 of %2 ").arg(0).arg(max));

        this->max = importIcons(path);

        t->start(0);
    } else if (action==1){
        this->max = 0;
        int cur = CoreLib->getWineProcessList(path).count();
        lblInfo->setText(QString("%1<br>%2<br><br>%3").arg(tr("Waiting for wine process finish.")).arg(tr("There are %1 process running for prefix %2").arg(cur).arg(path)).arg(tr("While process keep running -- end them manually.")));
        setWindowTitle(tr("Running process: %1").arg(cur));
        t->start(1000);
    }


    progressBar->setMaximum(this->max);




    return;
}

void Progress::cmdCancel_Click(){
    t->stop();
    if (action==0){
        /* becouse, of desktop folder can contains user subdirs, DO NOT REMOVE this folders
        if (CoreLib->getSetting("DesktopImport", "remove", false, 0)==1)
            removeEmptyFolders(this->path);
        */
    }
    this->reject();
    return;
}

void Progress::runAction(){
    if (action==0){

        qDebug()<<current<<max;

        if ((current>=max) or (max==0)){
            t->stop();
            /* becouse, of desktop folder can contains user subdirs, DO NOT REMOVE this folders
            if (CoreLib->getSetting("DesktopImport", "remove", false, 0)==1)
                removeEmptyFolders(this->path);
            */
            this->accept();
            return;
        }

        int index = current;
        setWindowTitle(tr("Importing desktop icons: %1 of %2 ").arg(current).arg(max));
        parseDesktopFile(iconFiles.at(index), iconDirs.at(index));
        progressBar->setValue(index);

        current++;
    } else if (action==1){
        int count = CoreLib->getWineProcessList(path).count();
        if (count == 0){
            this->accept();
            return;
        } else {
            lblInfo->setText(QString("%1<br>%2<br><br>%3").arg(tr("Waiting for wine process finish.")).arg(tr("There are %1 process running for prefix %2").arg(count).arg(this->path)).arg(tr("While process keep running -- end them manually.")));
            setWindowTitle(tr("Running process: %1").arg(count));
        }
    }
}

int Progress::importIcons(QString folder){
#ifdef DEBUG
    qDebug()<<"[ii] enter folder: "<<folder;
#endif
    QDir dir(folder);

    if (!dir.exists())
        return 0;

    int files=0;

    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        if (list.at(i).isDir()){
            files = files + this->importIcons(list.at(i).absoluteFilePath());
        } else {
            if (list.at(i).fileName().right(8)==".desktop"){
                files++;
                iconDirs.append(dir.dirName());
                iconFiles.append(list.at(i).absoluteFilePath());
            }
        }
    }

    return files;
}

/*
void Progress::removeEmptyFolders(QString folder){
#ifdef DEBUG
    qDebug()<<"[ii] enter folder: "<<folder;
#endif
    QDir dir(folder);

    if (!dir.exists())
        return;

    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        if (list.at(i).isDir()){
            this->removeEmptyFolders(list.at(i).absoluteFilePath());
        }
    }


#ifdef DEBUG
        qDebug()<<"[ii] Dir removed: "<<dir.rmdir(dir.path());
#else
        dir.rmdir(dir.path());
#endif
    return;
}
*/

void Progress::parseDesktopFile(QString filePath, QString dirName){

#ifdef DEBUG
    qDebug()<<"* [ii] Parsing file: "<<filePath<<" at "<<dirName;
#endif

    QString name, path, type, icon, exec, args, prefix_path;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(QRegExp("^Name=*"))){
            name=dirName;
            name.append(" - ");
            name.append(line.right(line.length()-5));
        } else if (line.contains(QRegExp("^Type=*"))){
            type=line.right(line.length()-5);
        } else if (line.contains(QRegExp("^Path=*"))){
            path=line.right(line.length()-5);
        } else if (line.contains(QRegExp("^Icon=*"))){
            icon=line.right(line.length()-5);
            if (!icon.contains(QRegExp("(png$|xpm$|gif$|jpg$)"))){
                icon.append(".png");
            }
        } else if (line.contains(QRegExp("^Exec=*"))){
            //Parse exec string
            QRegExp rxlen("env WINEPREFIX=\"(.+)\" wine (.*)");
            if (rxlen.indexIn(line) != -1){
                QStringList cap = rxlen.capturedTexts();
#ifdef DEBUG
                qDebug()<<"[ii] Captured Texts :"<<cap;
#endif
                if (cap.count()>=3){
                    prefix_path = cap.at(1).trimmed();
                    exec = cap.at(2).trimmed().replace("\\\\ ", " ").replace("\\\\\\\\", "\\");
                    if (cap.count()>=4){
                        args = cap.at(3).trimmed();
                    }
                }
            }
        }
    }

    file.close();

#ifdef DEBUG
    qDebug()<<"= [ii] =======================================================";
    qDebug()<<"type: "<<type;
    qDebug()<<"name: "<<name;
    qDebug()<<"path: "<<path;
    qDebug()<<"exec: "<<exec;
    qDebug()<<"args: "<<args;
    qDebug()<<"prefix_path: "<<prefix_path;
    qDebug()<<"icon: "<<icon;
    qDebug()<<"= [ii] =======================================================";
#endif

    QString prefix_name = db_prefix.getName(prefix_path);
#ifdef DEBUG
    qDebug()<<" [ii] Get prefix by path: "<<prefix_path<<" name is: "<<prefix_name;
#endif

    if (prefix_name.isEmpty())
        return;

    if (!db_dir.isExistsByName(prefix_name, "import"))
        if (!db_dir.addDir(prefix_name, "import")){
             qDebug()<<"Can't create dir:"<<"\"import\""<<" for prefix:"<<prefix_name;
             return;
        }

    if (!db_icon.isExistsByName(prefix_name, "import", name)){
#ifdef DEBUG
        qDebug()<<" [ii] adding icon...";
#endif
        QString res = CoreLib->getSetting("advanced", "defaultDesktopSize", false, "").toString();
        if (db_icon.addIcon(args, exec, icon, "", prefix_name, "import", name, "", "", "", "", path, res)){
#ifdef DEBUG
            qDebug()<<" [ii] adding icon OK.";
#endif
            if (CoreLib->getSetting("DesktopImport", "remove", false, 0)==1){
#ifdef DEBUG
                qDebug()<<"[ii] removed: "<<file.remove();
#else
                file.remove();
#endif
            }

        }
    }


    return;
}
