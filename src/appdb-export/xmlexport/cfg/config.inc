<?php
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

    //MemChace connection settings
    $memcache_host="localhost";
    $memcache_port=11211;
?>
