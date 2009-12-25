<?php

/***************************************************************************
 *   Copyright (C) 2009,2010 by Malakhov Alexey                            *
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

require_once("./engine/check.php");

//Check for walid useragent for details see config.inc file
if (checkUserAgent()==0){
	print showAbout();
} else {
	require_once("./engine/db.php");
	require_once("./engine/memcache.php");

	$DB = new DB();

	//Getting action value
	$action = (int)$_POST['action'];
		 
	switch ($action){
		case 1:
			//Export results of searching application by search string
			print $DB->exportAppByName($_POST['search'], $_POST['page']);
		break;
		case 3:
			//Export application by id
			print $DB->exportAppById($_POST['appid']);
		break;
		case 4:
			//Export application test results
			print $DB->exportTestResults($_POST['appid'], $_POST['verid'], $_POST['testid']);
		break;
		case 5:
			//Export category view
			print $DB->exportCategory($_POST['catid']);		  
		break;
		default:
			//Just show about page
			print showAbout();
		break;
	}
}
?>
