<?php

    require_once("./engine/db.php");
    require_once("./engine/memcache.php");

    $DB = new DB();

	 //Getting action value
	 $action = (int)$_GET['action'];
	 
	 switch ($action){
		  case 1:
				//Search app by user requested name
				print $DB->serachAppByName($_GET['search'], $_GET['page']);
		  break;
		  case 3:
				//Search app by Id
				print $DB->serachAppById($_GET['appid']);
		  break;
		  case 4:
				print $DB->searchAppTestResults($_GET['appid'], $_GET['verid'], $_GET['testid']);
		  break;
		  case 5:
				print $DB->serachCategoryById($_GET['catid']);		  
		  break;
	 }
?>
