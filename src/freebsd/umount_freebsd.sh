#!/bin/sh
# /***************************************************************************
# *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
# *   brezerk@gmail.com                                                     *
# *                                                                         *
# *   This program is free software: you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation, either version 3 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
# *                                                                         *
# *   In addition, as a special exception, the copyright holders give       *
# *   permission to link the code of this program with any edition of       *
# *   the Qt library by Trolltech AS, Norway (or with modified versions     *
# *   of Qt that use the same license as Qt), and distribute linked         *
# *   combinations including the two.  You must obey the GNU General        *
# *   Public License in all respects for all of the code used other than    *
# *   Qt.  If you modify this file, you may extend this exception to        *
# *   your version of the file, but you are not obligated to do so.  If     *
# *   you do not wish to do so, delete this exception statement from        *
# *   your version.                                                         *
# ***************************************************************************/

#
# In fact, FreeBSD don't destroy md device automatically, so i need
# to create this script for q4wine. It is better to use sh, then make it on C++.
#
# With this hack, q4wine umounts iso images property and have support
# for Quick Mount profiles, as well, as it can be done.
#

# Arguments: $1 -- umount binary
#            $2 -- mount point

# Check is $2 a directory?
if [ ! -d $2 ]; then
	echo "Error: Can't get mount point directory."
	exit 255
fi

# Check is $1 an executable bianry
if [ ! -x $1 ]; then
	echo "Error: $1 is not an executable file."
	exit 255
fi

#Getting device name:
device_name="$(mount | grep $2 | awk '{print $1}')"

#Getting mdconfig binary and check is it executable binary or not.
mdconfig="$(which mdconfig 2>&1)"
if [ ! -x "$mdconfig" ]; then
	echo "Error: Can't find mdconfig binary. Does it accesible by PATH variable?"
	exit 255
fi

#Now it's time to umount device
$1 $2
if [ $? > 0 ]; then
	echo "Error: \"$1 $2\" returns an a error."
	exit 255
fi

#Now it's time to get: Is it md device or not? If yes -- use mdconfig to remove it.
if [ "$device_name" =~ md* ]; then
	$mdconfig -d -u$device_name
	if [ $? > 0 ]; then
		echo "Error: \"mdconfig -d -u$device_name\" returns an a error."
		exit 255
	fi
fi

exit 0
