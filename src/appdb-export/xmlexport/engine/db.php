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

require_once("./cfg/config.inc");
require_once("xml.php");

Class DB { 
	private $appdb_dblink;
   private $bugzilla_dblink;
   private $XML;

   function __construct(){
		/* Database class constructor */

		// Get config values from ./cfg/config.inc
		global $appdb_host;
		global $appdb_base;
		global $appdb_user;
		global $appdb_pass;
	  
		global $bugzilla_host;
		global $bugzilla_base;
		global $bugzilla_user;
		global $bugzilla_pass;

		// Connect to appdb database
		
		/* First, when connecting, the funct would first try to find a 
		 * (persistent) link that's already open with the same host, 
		 * username and password. If one is found, an identifier for 
		 * it will be returned instead of opening a new connection.
		 */

		$this->appdb_dblink = mysql_pconnect($appdb_host, $appdb_user, $appdb_pass) or die("[ee] Could not connect to appdb database: " . mysql_error());
		mysql_select_db($appdb_base, $this->appdb_dblink) or die("[ee] Could not select appdb database");


		//Connect to wine bugzilla database
		//$this->bugzilla_dblink = mysql_pconnect($bugzilla_host, $bugzilla_user, $bugzilla_pass) or die("[ee] Could not connect to appdb database: " . mysql_error());
		//mysql_select_db($bugzilla_base, $this->bugzilla_dblink) or die("[ee] Could not select appdb database");
		 
	
		$this->XML = new XMLExport();
	  
   }
      
   function serachAppByName($search, $page = 1){
		/* This funct will search for a application name
		 * 
		 * TODO: Make more advanced search criterias like
		 * rating: category: license: 
		 */
		
		//Get only first 20 characters
		//this will makes sql injection mode harder ;)
		$search = substr($_GET['search'], 0, 20);
		
		$page = (int)$page;
		if ($page <= 0){
			$sql_page = 0;
		} else {
			$sql_page = ($page - 1) * 10;
		}
		
		$xml_view = $this->XML->createHeader(1);
		$search = mysql_real_escape_string($search, $this->appdb_dblink);
		
		$query = "SELECT COUNT(appId) FROM appFamily WHERE appName LIKE '%${search}%' AND state='accepted'";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
			if (mysql_num_rows($result) > 0){
				$row = mysql_fetch_row($result);
				$xml_view .= $this->XML->createPages($page, ceil($row[0]/10));
			} else {
				die("serachAppByName: Invalid result");
			}
			
		mysql_free_result($result);
		
		$query = "SELECT appId, appName, description, catId, webPage FROM appFamily WHERE appName LIKE '%${search}%' AND state='accepted' LIMIT ${sql_page}, 9";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
		
		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n	<app-list>";
			while ($row = mysql_fetch_assoc($result)) {
				//Remove html tegs
				$xml_view .= $this->XML->createAppInfo($row['appId'], $this->prepareXMLString($row['appName']), $this->prepareXMLString($row['description'], 255), $row['catId'], $this->prepareXMLString($row['webPage']), $this->getVersionByAppId($row['appId']));
			}
			$xml_view .= "\n	</app-list>";
		} else {
			die("serachAppByName: Invalid result");
		}
		
		$xml_view .= $this->XML->closeHeader();
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		mysql_free_result($result);
			
		return $xml_view;
	}
	
	function serachAppById($appid){
		/* This funct will search for a application by id
		*/
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
		
		$xml_view = $this->XML->createHeader(3);
		$xml_view .= $this->getAppById($appid, 1, 1);
		$xml_view .= $this->XML->closeHeader();
	
		return $xml_view;
	}
	
	function searchAppTestResults($appid, $verid = 0, $testid = 0){

		
		$appid = (int)$appid;
		if ($appid < 0)
			return;
			
		$verid = (int)$verid;
		if ($verid < 0)
			return;

		$testid = (int)$testid;
		if ($testid < 0)
			return;
			
		$xml_view = $this->XML->createHeader(4);
		$xml_view .= $this->getAppById($appid, $verid, $testid);
		$xml_view .= $this->XML->closeHeader();
				
		return $xml_view;
	}
	
	
	function serachCategoryById($catid){
		/* This funct will search for a application by id
		*/
		$catid = (int)$catid;
		if ($catid < 0)
			return;
		
		$xml_view = $this->XML->createHeader(5);
		$xml_view .= "\n		<category-list>";
		$xml_view .= $this->XML->createCategoryInfo(0, "Main", "");
		if ($catid>0)
			$xml_view .= $this->getCategoryById($catid);
		$xml_view .= "\n		</category-list>";
		
		$query = "SELECT catId, catName, catDescription FROM appCategory WHERE catParent={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachCategoryById: Invalid query");
			}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<subcategory-list>";
			while ($row = mysql_fetch_assoc($result))
				$xml_view .= $this->XML->createCategoryInfo($row['catId'], $this->prepareXMLString($row['catName']), $this->prepareXMLString($row['catDescription'], 60));
			$xml_view .= "\n		</subcategory-list>";
		} 
		mysql_free_result($result);
		
		
		$query = "SELECT appId, appName, description FROM appFamily WHERE state='accepted' and catId={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachCategoryById: Invalid query");
			}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<app-list>";
			while ($row = mysql_fetch_assoc($result))
				$xml_view .= $this->XML->createCategoryAppInfo($row['appId'], $this->prepareXMLString($row['appName']), $this->prepareXMLString($row['description'], 60));
			$xml_view .= "\n		</app-list>";
			//trim(substr(preg_replace("/(<\/?)(\w+)([^>]*>)/e", "", $row['description']), 0, 60))
		} 
				
		$xml_view .= $this->XML->closeHeader();
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		// mysql_free_result($result);
			
		return $xml_view;
	}
	
	function prepareXMLString($string, $len = 0, $strip_html = 1){
		$ret = $string;
		
		//Remove stupid whitespaces
		$ret = trim(preg_replace("/\s+/", " ", $ret));
		
		//Strip for html tags
		if ($strip_html==1){
			$ret = strip_tags($ret);
		} else {
			$ret = strip_tags($ret, "<p><a><br><pre>");
		}
		//$ret = strip_tags(trim(preg_replace("/(<\/?)(\w+)([^>]*>)/e", "", $ret));
		
		if ($len > 0)
			$ret = substr($ret, 0, $len);
		
		//Convert to html characters
		$ret = str_replace("#", "", $ret);
		$ret = str_replace("&", "&amp;", $ret);
		$ret = str_replace(">", "&gt;", $ret);
		$ret = str_replace("<", "&lt;", $ret);	
		return $ret;
	}
	
	function getVersionByAppId($appid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
		
		$query = "SELECT versionId, versionName, rating, ratingRelease FROM appVersion WHERE appId={$appid} AND state='accepted'";

		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
		
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_assoc($result)) {
				$xml_view .= $this->XML->createAppVersionInfo($row['versionId'], $this->prepareXMLString($row['versionName']), $row['rating'], $row['ratingRelease']);
			}
		}
		
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		mysql_free_result($result);
				
		return $xml_view;
	}
	
	function getAppById($appid, $verid, $testid){
		if ($appid <= 0)
			return;
			
		$query = "SELECT appId, appName, description, catId, webPage FROM appFamily WHERE appId={$appid} AND state='accepted' LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getAppById: Invalid app info query");
			}
				
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			
			$category_list = $this->XML->createCategoryInfo(0, "Main", "");
			$category_list .= $this->getCategoryById($row['catId']);
			
			if ($verid<=0){		
				$versions_list = $this->getVersionByAppId($row['appId']);
			} else {
				$test_results = $this->getTestResultsByTestId($verid, $testid);
			}
			
			$xml_view .= $this->XML->createAppInfo($row['appId'], $this->prepareXMLString($row['appName']), $this->prepareXMLString($row['description']), $row['catId'], $this->prepareXMLString($row['webPage']), $versions_list, $category_list, $test_results);
		}
		
		//FIXME: Siple info about no app by this id
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getTestResultsByTestId($verid, $testid){
		if (($testid <= 0) and ($verid <= 0))
			return;
			
		if ($testid <= 0){
			$query = "SELECT testingId, testedRating, testedRelease, whatWorks, whatDoesnt, whatNotTested, comments FROM testResults WHERE versionId={$verid} AND state='accepted' ORDER BY submitTime DESC LIMIT 0 , 1";
		} else {
			$query = "SELECT testingId, testedRating, testedRelease, whatWorks, whatDoesnt, whatNotTested, comments FROM testResults WHERE testingId={$testid} AND state='accepted' LIMIT 0 , 1";			
		}
		
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getAppById: Invalid app info query");
			}
				
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_array($result);
			$xml_view = $this->XML->createAppTestingInfo($row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6]);
			
			$xml_view .= $this->getTop5TestResults($verid, $row[0]);
			
			
		}
		return $xml_view;
	}
	
	function getCategoryById($catid){
		/* This function will gets category name and description by category id
		 */
		$catid = (int)$catid;
		if ($catid < 0)
			return;
			
		$query = "SELECT catId, catName, catDescription, catParent FROM appCategory WHERE catId={$catid} LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getCategoryById: Invalid category info query");
			}
		
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			if ($row['catParent']>0)
				$xml_view .= $this->getCategoryById($row['catParent']);
			$xml_view .= $this->XML->createCategoryInfo($row['catId'], $this->prepareXMLString($row['catName']), $this->prepareXMLString($row['catDescription'], 60));
		}
		
		mysql_free_result($result);	
		return $xml_view;
	}
	
};

?>
