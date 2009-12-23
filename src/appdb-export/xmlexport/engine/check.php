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

require_once("./cfg/config.inc");

function checkUserAgent($agent = ""){
		if (!$agent)
			$agent = $_SERVER['HTTP_USER_AGENT'];
			
		global $white_agents;
		
		if (count($white_agents)==0)
			return 1;
		
		foreach ($white_agents as $white_agent){
				if (eregi($white_agent, $agent)==1)
					return 1;
		}
		
		return 0;
}

function showAbout(){
		$ret = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">
<html>
	<head>
		<title>Wine AppDB xmlexport script</title>
		<meta http-equiv=Content-Type content=\"text/html; charset=utf8\">
		<meta name=\"language\" content=\"English\">
		<meta name=\"author\" content=\"Malakhov Alexey aka John Brezerk\">
		<meta name=\"copyright\" content=\"All trademarks and copyrights on this page are owned by their respective owners. The Rest &#169 1997-2010 BrezBlock.\">
		<meta name=\"robots\" content=\"noindex, nofollow\">
	</head>
	<body>
		<h2>Wine AppDB xmlexport</h2>
		<hr>
		<p>Welcome to Wine AppDB xmlexport script page. This script provides xml view interface for Wine AppDB database.</p>
		<p>Originally this script was created by Malakhov Alexey aka <a href='mailto: brezerk@gmail.com'>John Brezerk</a> for <a href='http://q4wine.brezblock.org.ua/'>q4wine</a> project.</p>
		<p>Current licese is GPL v3.</p>
		<hr>
		<p>All trademarks and copyrights on this page are owned by their respective owners. The Rest &#169 1997-2010 BrezBlock.</p>
	</body>
</html>";
		return $ret;
}

?>
