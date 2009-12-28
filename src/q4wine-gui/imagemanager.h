/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/
#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <ui_ImageManager.h>

#include "memory"

#include "config.h"

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QString>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QListWidget>
#include <QFile>
#include <QLibrary>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QMenu>
#include <QHeaderView>
#include <QTableWidget>

#include "src/core/database/image.h"
#include <q4wine-lib/main.h>

/*!
 * \class ImageManager
 * \ingroup q4wine-gui
 * \brief This class provide image manager dialog functions.
 *
 */
class ImageManager : public QDialog, public Ui::ImageManager
{
	Q_OBJECT
	public:
		//! Class constructor
		ImageManager(QWidget * parent = 0, Qt::WFlags f = 0);

	private slots:
		//! Ok button click slot
		void cmdOk_Click();
		//! Image add slot
		void actionAddImage(void);
		//! Image rename slot
		void actionRenameImage(void);
		//! Image remove slot
		void actionRemoveImage(void);
		//! Image list refresh slot
		void actionRefreshImageList(void);

		//! Context menu show slot
		void tableImage_showContextMenu(const QPoint);
		//! Update lilPAthInfo slot
		void update_lblPathInfo(const QModelIndex);
		/*!
		 * \bref Help button click slot
		 */
		void cmdHelp_Click();


	private:
		std::auto_ptr<QToolBar> managerToolBar;
		//! Loading theme icons
		void loadThemeIcons(QString themePath);
		//! Creating menus
		void createMenus(void);
		//! Load icons
		QIcon loadIcon(QString iconName, QString themePath);
		//! Getting CD images list
		void getCDImages(void);

		 //! This is need for libq4wine-core.so import;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		std::auto_ptr<corelib> CoreLib;
		QLibrary libq4wine;

		Image db_image;

		//! Actions
		std::auto_ptr<QAction> actionAdd;
		std::auto_ptr<QAction> actionRemove;
		std::auto_ptr<QAction> actionRename;
		std::auto_ptr<QAction> actionRefresh;
};

#endif


