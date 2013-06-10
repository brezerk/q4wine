/***************************************************************************
 *   Copyright (C) 2008-2011 by Alexey Malakhov                            *
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

#include "about.h"

About::About(QWidget * parent, Qt::WFlags f) : QDialog(parent, f)
{
    setupUi(this);
    setWindowTitle(tr("About %1").arg(APP_NAME));
    connect(cmdOk, SIGNAL(clicked()), this, SLOT(cmdOk_Click()));

    lblVersion->setText(QString("<span style=\"font-size:14pt; font-weight:600;\">%1 %2</span>").arg(APP_NAME).arg(APP_VERS));

    txtAuthors->setHtml(T_DEVELOPERS);
    txtPAuthors->setHtml(T_PKG_MAINTAINERS);
    txtTranslation->setHtml(T_TRANSLATIONS);
    txtThanks->setHtml(T_THANKS);

    cmdOk->setFocus(Qt::ActiveWindowFocusReason);

    return;
}

void About::cmdOk_Click()
{
    accept();
    return;
}
