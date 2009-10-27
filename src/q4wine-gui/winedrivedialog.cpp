#include "winedrivedialog.h"

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	if (removeLetters.count()>0){
		for (int i=0; i<removeLetters.count(); i++){
			if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly)>=0){
				cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
			}
		}
	}

	if (cbDriveLetter->count()>2){
		cbDriveLetter->setCurrentIndex(2);
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetDrivePath, SIGNAL(clicked()), this, SLOT(cmdGetDrivePath_Click()));

	cmdGetDrivePath->setIcon(QIcon(":data/folder.png"));

}

WineDriveDialog::WineDriveDialog(QStringList removeLetters, QString driveLetter, QString drivePath, QString driveType, QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
	setupUi(this);
	if (removeLetters.count()>0){
		for (int i=0; i<removeLetters.count(); i++){
			if (cbDriveLetter->findText(removeLetters.at(i), Qt::MatchExactly)>=0){
				cbDriveLetter->removeItem(cbDriveLetter->findText(removeLetters.at(i)));
			}
		}
	}

	if (cbDriveLetter->findText(driveLetter, Qt::MatchExactly)>=0){
				cbDriveLetter->setCurrentIndex(cbDriveLetter->findText(driveLetter, Qt::MatchExactly));
	}

	txtDrivePath->setText(drivePath);

	if (cbDriveType->findText(driveType, Qt::MatchExactly)>=0){
				cbDriveType->setCurrentIndex(cbDriveType->findText(driveType, Qt::MatchExactly));
	}

	connect(cmdCancel, SIGNAL(clicked()), this, SLOT(cmdCancel_Click()));
	connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));
	connect(cmdGetDrivePath, SIGNAL(clicked()), this, SLOT(cmdGetDrivePath_Click()));

	cmdGetDrivePath->setIcon(QIcon(":data/folder.png"));
}

void WineDriveDialog::cmdCancel_Click(){
	reject();
}

void WineDriveDialog::cmdOk_Click(){

	if (txtDrivePath->text().isEmpty()){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, you need to set existing drive directory."), QMessageBox::Ok);
		return;
	}

	if (!QDir().exists(txtDrivePath->text())){
		QMessageBox::warning(this, tr("Error"), tr("Sorry, drive directory not exists."), QMessageBox::Ok);
		return;
	}

	driveType=cbDriveType->currentText();

	driveDesc.clear();
	driveDesc.append(cbDriveLetter->currentText());
	driveDesc.append(" ");
	driveDesc.append(txtDrivePath->text());
	driveDesc.append("\nType: ");
	driveDesc.append(cbDriveType->currentText());
	accept();
}

void WineDriveDialog::cmdGetDrivePath_Click(){
	QString fileName;

	QFileDialog dialog(this);
	  dialog.setFilter(QDir::Dirs | QDir::Hidden);

	  dialog.setFileMode(QFileDialog::Directory);
	  dialog.setWindowTitle(tr("Open Directory"));
	  // dialog.setDirectory(searchPath);
	  // This option works only it qt 4.5. In fact this not works correctly with QDir::Hidden,  so I comment it out for a some  time
	  // dialog.setOption(QFileDialog::ShowDirsOnly, true);
	  // dialog.setSidebarUrls(add_prefix_urls);

	if (dialog.exec())
		fileName = dialog.selectedFiles().first();

	if(!fileName.isEmpty()){
		txtDrivePath->setText(fileName);
	}

	return;
}
