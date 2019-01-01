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
#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <src/q4wine-gui/ui_ImageManager.h>

#include <memory>

#include "config.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolBar>
#include <QMenu>
#include <QBoxLayout>
#include <QHeaderView>

#include "image.h"
#include "q4wine-lib.h"

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
		ImageManager(QWidget * parent = 0, Qt::WindowFlags f = 0);

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
		//! Loading theme icons
		void loadThemeIcons();
		//! Creating menus
		void createMenus(void);
		//! Getting CD images list
		void getCDImages(void);

		 //! This is need for libq4wine-core.so import;
		typedef void *CoreLibPrototype (bool);
		CoreLibPrototype *CoreLibClassPointer;
		std::unique_ptr<corelib> CoreLib;
		QLibrary libq4wine;

		Image db_image;

		//! Actions
		std::unique_ptr<QAction> actionAdd;
		std::unique_ptr<QAction> actionRemove;
		std::unique_ptr<QAction> actionRename;
		std::unique_ptr<QAction> actionRefresh;
};

#endif


