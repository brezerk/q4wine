/***************************************************************************
 *   Copyright (C) 2008-2017 by Oleksii S. Malakhov <brezerk@gmail.com>     *
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

#include "iconsview.h"

IconsView::IconsView(QString tmpDir, QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
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

    setupUi(this);

    setWindowIcon(CoreLib->loadIcon(CoreLib->getSetting("app", "icon", false, "q4wine").toString()));

	tempDirectory=tmpDir;

	QDir tmp(tmpDir);
	tmp.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = tmp.entryInfoList();

	lstIcons->clear();
	lstIcons->setSelectionMode(QAbstractItemView::SingleSelection);

	// Getting converted icons from temp directory
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().right(3)=="png"){
			std::auto_ptr<QListWidgetItem> iconItem (new QListWidgetItem(lstIcons, 0));
			iconItem->setText(fileInfo.fileName());
			iconItem->setIcon(QIcon(fileInfo.filePath()));
			iconItem.release();
		}
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	return;
}

void IconsView::cmdCancel_Click(){
	reject();
	return;
}

void IconsView::cmdOk_Click(){
	//Getting selected icons

	//FIXME: For multiple icon export
	//QList<QListWidgetItem *> icoList = lstIcons->selectedItems();

	//for (int i=0; i<icoList.count(); i++){
	//		iconBuffer.names.append(icoList.at(i)->text());
	//	}

	if (lstIcons->currentItem()){
		QFile file;

		QString sourceFile, saveFile, saveFileName;
		bool ok=false;

		sourceFile.clear();
		sourceFile.append(tempDirectory);
		sourceFile.append("/");
		sourceFile.append(lstIcons->currentItem()->text());

		if (cbDefaultExport->checkState()==Qt::Checked){
		saveFile.clear();
		saveFile.append(QDir::homePath());
		saveFile.append("/.config/");
		saveFile.append(APP_SHORT_NAME);
		saveFile.append("/icons/");
		saveFile.append(lstIcons->currentItem()->text());

		saveFileName=lstIcons->currentItem()->text();

		QMessageBox message;
			message.setText(tr("Sorry. It seems that the file already exists.<br>Replace existing file or rename current one?"));
			message.setWindowTitle(tr("Exporting icon"));
			message.setIcon(QMessageBox::Question);
			message.addButton(tr("Rename"),QMessageBox::ResetRole);
			message.addButton(tr("Use existing"), QMessageBox::ActionRole);
			message.addButton(tr("Replace"), QMessageBox::DestructiveRole);
			message.addButton(tr("Cancel"), QMessageBox::RejectRole);

			while (QFile::exists (saveFile)){
				switch (message.exec()){
					case 0:
						saveFileName = QInputDialog::getText(this, tr("Sorry. It seems that the file already exists."),
																		tr("Replace existing file or rename current one?"), QLineEdit::Normal,
																	saveFileName , &ok);
						if ((!saveFileName.isEmpty()) && (ok)){
							saveFile.clear();
							saveFile.append(QDir::homePath());
							saveFile.append("/.config/");
							saveFile.append(APP_SHORT_NAME);
							saveFile.append("/icons/");
							saveFile.append(saveFileName);
						} else {
							reject();
							return;
						}
					break;
					case 1:
						saveFile.clear();
						saveFile.append(QDir::homePath());
						saveFile.append("/.config/");
						saveFile.append(APP_SHORT_NAME);
						saveFile.append("/icons/");
						saveFile.append(saveFileName);
						selectedFile=saveFile;
						accept();
						return;
					break;
					case 2:
						if (!file.remove(saveFile)){
							QMessageBox::warning(this, tr("Error"), tr("Sorry, file cannot be removed:<br>%1.").arg(saveFile), QMessageBox::Ok);
						}
					break;
					case 3:
						reject();
						return;
					break;
				}
		}

		} else {
			saveFile.clear();
			saveFile.append(QDir::homePath());
			saveFile.append("/.config/");
			saveFile.append(APP_SHORT_NAME);
			saveFile.append("/icons/");

#if QT_VERSION >= 0x040500
        QFileDialog::Options options;

        if (CoreLib->getSetting("advanced", "useNativeFileDialog", false, 1)==0)
                options = QFileDialog::DontUseNativeDialog;

        saveFile = QFileDialog::getSaveFileName(this, tr("Select file to save"), saveFile , tr("Images (*.png)"), 0, options);
#else
        saveFile = QFileDialog::getSaveFileName(this, tr("Select file to save"), saveFile , tr("Images (*.png)"));
#endif

			if (saveFile.isEmpty()){
				reject();
				return;
			}
		}

		if (!file.copy(sourceFile, saveFile)){
			QMessageBox::warning(this, tr("Error"), tr("Sorry, file cannot be created:<br>%1.").arg(saveFile), QMessageBox::Ok);
		}

		selectedFile=saveFile;
		accept();
		return;

	} else {
		reject();
		return;
	}
}
