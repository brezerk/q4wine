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

	 //General xmlexport settings
 
	 /*You can select memcache usage level
	   Valid levels are: 0, 1, 2, 3, 4
	 
		0 -- Turn off memcache engine;
		1 -- Cache only app version array;
		2 -- Cache app version array and app top5 results array;
		3 -- Cache app version array, app top5 results array and comments array;
		4 -- Cache app info;
		5 -- Cache search results;
	 
	 */
	 $memcache_level=5;
	 
	 //How long memcache will store chaced object in seconds
	 $memcache_timeout=43200;

    //Wine Appdb database connection settings
    $appdb_host="localhost";
    $appdb_base="apidb";
    $appdb_user="root";
    $appdb_pass="uahss85";

    //Wine Bugzilla database connection settings
    $bugzilla_host="localhost";
    $bugzilla_base="bugs";
    $bugzilla_user="root";
    $bugzilla_pass="uahss85";
    
    /*We can filter for client header
      It will make ddos a little bit harder ;)
            
      Leave it blank to disable this feature
     */
     
    $white_agents = array();
    $white_agents[0] = "^q4wine/[0-1].[1-9]{3}([ ]|-[0-9a-zA-Z]+[ ])\([0-9a-zA-Z]+; [0-9a-zA-Z]+ [0-9a-zA-Z]+) xmlparser/[0-9].[0-9]$";
	 
    $xmlexport_version = "0.1";

    //MemChace connection settings
    $memcache_host="localhost";
    $memcache_port=11211;
?>
