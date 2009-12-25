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
		$this->bugzilla_dblink = mysql_pconnect($bugzilla_host, $bugzilla_user, $bugzilla_pass) or die("[ee] Could not connect to appdb database: " . mysql_error());
		mysql_select_db($bugzilla_base, $this->bugzilla_dblink) or die("[ee] Could not select appdb database");
		 
		$this->XML = new XMLExport();
		return;
   }
      
   function exportAppByName($search, $page = 1){
		/* This funct will search for a application name
		 * 
		 * TODO: Make more advanced search criterias like
		 * rating: category: license: 
		 */
		
		//Get only first 20 characters
		//this will makes sql injection mode harder ;)
		$search = substr($search, 0, 20);
				
		$page = (int)$page;
		if ($page <= 0){
			$sql_page = 0;
		} else {
			$sql_page = ($page - 1) * 10;
		}
		
		global $appdb_base;
		
		$search = mysql_real_escape_string($search, $this->appdb_dblink);
		
		$query = "SELECT COUNT(appId) FROM {$appdb_base}.appFamily WHERE appName LIKE \"%${search}%\" AND state='accepted'";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
			if (mysql_num_rows($result) > 0){
				$row = mysql_fetch_row($result);
				$xml_view = $this->XML->openHeader(1);
				$xml_view .= $this->XML->createPages($page, ceil($row[0]/10));
			}
			
		mysql_free_result($result);
		
		$query = "SELECT appId, appName, description, catId, webPage FROM {$appdb_base}.appFamily WHERE appName LIKE \"%${search}%\" AND state='accepted' LIMIT ${sql_page}, 9";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
		
		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n	<app-list>";
			while ($row = mysql_fetch_assoc($result)) {
				//Remove html tegs
				$appver_list = $this->getVersion($row['appId']);
				$xml_view .= $this->XML->createAppInfo($row['appId'], 0, $row['appName'], $row['description'], $row['catId'], $row['webPage'], $appver_list);
			}
			$xml_view .= "\n	</app-list>";
		} else {
			$xml_view = $this->XML->openHeader(6);
			$xml_view .= "<message>No matches found</message>";
		}
		
		$xml_view .= $this->XML->closeHeader();
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		mysql_free_result($result);
		return $xml_view;
	}
	
	function exportAppById($appid){
		/* This funct will search for a application by id
		*/
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
		
		$xml_view = $this->XML->openHeader(3);
		$xml_view .= $this->getApp($appid);
		$xml_view .= $this->XML->closeHeader();
	
		return $xml_view;
	}
	
	function exportTestResults($appid, $verid = 0, $testid = 0){
		$appid = (int)$appid;
		if ($appid < 0)
			return;
			
		$verid = (int)$verid;
		if ($verid < 0)
			return;

		$testid = (int)$testid;
		if ($testid < 0)
			return;
			
		$xml_view = $this->XML->openHeader(4);
		$xml_view .= $this->getApp($appid, $verid, $testid);
		$xml_view .= $this->XML->closeHeader();
				
		return $xml_view;
	}
	
	
	function exportCategory($catid){
		/* This funct will search for a application by id
		*/
		$catid = (int)$catid;
		if ($catid < 0)
			return;
		
		global $appdb_base;
		
		$xml_view = $this->XML->openHeader(5);
		$xml_view .= "\n		<category-list>";
		$xml_view .= $this->XML->createCategoryInfo(0, "Main", "");
		if ($catid>0)
			$xml_view .= $this->getCategory($catid);
		$xml_view .= "\n		</category-list>";
		
		$query = "SELECT catId, catName, catDescription FROM {$appdb_base}.appCategory WHERE catParent={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachCategoryById: Invalid query");
			}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<subcategory-list>";
			while ($row = mysql_fetch_assoc($result))
				$xml_view .= $this->XML->createCategoryInfo($row['catId'], $row['catName'], $row['catDescription']);
			$xml_view .= "\n		</subcategory-list>";
		} 
		mysql_free_result($result);
		
		
		$query = "SELECT appId, appName, description FROM {$appdb_base}.appFamily WHERE state='accepted' and catId={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachCategoryById: Invalid query");
			}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<app-list>";
			while ($row = mysql_fetch_assoc($result))
				$xml_view .= $this->XML->createCategoryAppInfo($row['appId'], $row['appName'], $row['description']);
			$xml_view .= "\n		</app-list>";
			//trim(substr(preg_replace("/(<\/?)(\w+)([^>]*>)/e", "", $row['description']), 0, 60))
		} 
				
		$xml_view .= $this->XML->closeHeader();
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		// mysql_free_result($result);
			
		return $xml_view;
	}
	
	function getVersion($appid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
			
		global $appdb_base;
		
		$query = "SELECT versionId, versionName, rating, ratingRelease FROM {$appdb_base}.appVersion WHERE appId={$appid} AND state='accepted'";

		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getVersion: Invalid query");
			}
		
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_assoc($result)) {
				$xml_view .= $this->XML->createAppVersionInfo($row['versionId'], $row['versionName'], $row['rating'], $row['ratingRelease']);
			}
		}
		
		// Free the resources associated with the result set
		// This is done automatically at the end of the script
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getApp($appid, $verid=0, $testid=0){
		if ($appid <= 0)
			return;
			
		global $appdb_base;
			
		$query = "SELECT appId, appName, description, catId, webPage FROM {$appdb_base}.appFamily WHERE appId={$appid} AND state='accepted' LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getApp: Invalid app info query");
			}
				
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			
			$category_list = $this->XML->createCategoryInfo(0, "Main", "");
			$category_list .= $this->getCategory($row['catId']);
			
			if ($verid<=0){		
				$versions_list = $this->getVersion($row['appId']);
			} else {
				$test_results = $this->getTestResults($verid, $testid);
				$comment_list = $this->getNotes($appid, $verid);
				$comment_list .= $this->getComments($verid);
				$bugs_list = $this->getBugs($verid);
			}
			
			$xml_view .= $this->XML->createAppInfo($row['appId'], $verid, $row['appName'], $row['description'], $row['catId'], $row['webPage'], $versions_list, $category_list, $test_results, $comment_list, $verinfo, $bugs_list);
		}
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getTestResults($verid, $testid){
		if (($testid <= 0) and ($verid <= 0))
			return;

		global $appdb_base;
			
		if ($testid <= 0){
			$query = "SELECT testingId, testedRating, testedRelease, whatWorks, whatDoesnt, whatNotTested, comments, versionId FROM {$appdb_base}.testResults WHERE versionId={$verid} AND state='accepted' ORDER BY submitTime DESC LIMIT 0 , 1";
		} else {
			$query = "SELECT testingId, testedRating, testedRelease, whatWorks, whatDoesnt, whatNotTested, comments, versionId FROM {$appdb_base}.testResults WHERE testingId={$testid} AND state='accepted' LIMIT 0 , 1";			
		}
		
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getTestResults: Invalid app info query");
			}
				
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_array($result);
			
			$query = "SELECT versionName, license FROM {$appdb_base}.appVersion WHERE versionId={$row[7]}";
			$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getTestResults: Invalid app info query");
			}
			if (mysql_num_rows($result) > 0){
				$verrow = mysql_fetch_array($result);
			}
			
			$xml_view = $this->XML->createAppTestingInfo($row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $verrow[0], $verrow[1]);
			$xml_view .= $this->getTestResults_Top5($verid, $row[0]);	
		}
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getCategory($catid){
		/* This function will gets category name and description by category id
		 */
		$catid = (int)$catid;
		if ($catid < 0)
			return;
	
		global $appdb_base;
	
		$query = "SELECT catId, catName, catDescription, catParent FROM {$appdb_base}.appCategory WHERE catId={$catid} LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getCategory: Invalid category info query");
			}
		
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			if ($row['catParent']>0)
				$xml_view .= $this->getCategory($row['catParent']);
			$xml_view .= $this->XML->createCategoryInfo($row['catId'], $row['catName'], $row['catDescription']);
		}
		
		mysql_free_result($result);	
		return $xml_view;
	}
	
	function getTestResults_Top5($verid, $curtest){
		$verid = (int)$verid;
		if ($verid < 0)
			return;
			
		$curtest = (int)$curtest;
		if ($curtest < 0)
			return;

		global $appdb_base;			
		$query = "SELECT testingId, testedRating, testedRelease, DATE_FORMAT(testedDate, '%e %b %Y'), (SELECT name FROM {$appdb_base}.distributions WHERE {$appdb_base}.distributions.distributionId={$appdb_base}.testResults.distributionId), installs, runs FROM {$appdb_base}.testResults WHERE versionId={$verid} AND state='accepted' ORDER BY submitTime DESC LIMIT 0 , 5";			
		
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getTestResults_Top5: Invalid app info query!");
			}
				
		if (mysql_num_rows($result) > 0){
			$xml_view = "\n		<test-list>";
			while ($row = mysql_fetch_array($result)){
				if ($curtest==$row[0]){
					$curr=1;
				} else {
					$curr=0;
				}
				$xml_view .= $this->XML->createTop5TestResults($row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $curr);
			}
			$xml_view .= "\n		</test-list>";
		}
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getComments($verid, $parent=0){
		$verid = (int)$verid;
		if ($verid <= 0)
			return;
			
		$parent = (int)$parent;
		if ($parent < 0)
			return;
			
		global $appdb_base;
			
		$query = "SELECT commentId, parentId, (SELECT realname FROM {$appdb_base}.user_list WHERE userid={$appdb_base}.appComments.userId), subject, body, DATE_FORMAT(time, '%e %b %Y %H:%i:%s') FROM {$appdb_base}.appComments WHERE versionId={$verid} AND parentId={$parent} ORDER BY time DESC";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getComments: Invalid comments info query");
			}
				
		if (mysql_num_rows($result) > 0){
			//$xml_view = "\n		<test-list>";
			while ($row = mysql_fetch_array($result)){
				$xml_view .= $this->XML->createComment($row[0], $row[1], $row[2], $row[3], $row[4], $row[5]);
				$xml_view .= $this->getComments($verid, $row[0]);
			}
		}
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getNotes($appid, $verid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
		
		$verid = (int)$verid;
		if ($verid <= 0)
			return;
			
		global $appdb_base;
			
		$query = "SELECT noteTitle, noteDesc, DATE_FORMAT(submitTime, '%e %b %Y %H:%i:%s'), (SELECT realname FROM {$appdb_base}.user_list WHERE userid={$appdb_base}.appNotes.submitterId) FROM {$appdb_base}.appNotes WHERE versionId={$verid} OR (appId={$appid} AND versionId=0) ORDER BY noteId";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getComments: Invalid comments info query " . $query);
			}
				
		if (mysql_num_rows($result) > 0){
			//$xml_view = "\n		<test-list>";
			while ($row = mysql_fetch_array($result)){
				$xml_view .= $this->XML->createComment(0, 0, $row[5], $row[0], $row[1], $row[2]);
			}
		}
		
		mysql_free_result($result);
		return $xml_view;
	}
	
	function getBugs($appid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
			
		global $bugzilla_base;
		global $appdb_base;
			
		$query = "SELECT bug_id FROM {$appdb_base}.buglinks WHERE versionId={$appid} AND state='accepted' ORDER BY bug_id";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getBugs: Invalid comments info query");
			}
				
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_array($result)){
				$bug_query = "SELECT bug_id, bug_status, resolution, short_desc FROM {$bugzilla_base}.bugs WHERE bugs.bug_id={$row[0]} AND (bugs.bug_status='NEW' OR bugs.bug_status='UNCONFIRMED') LIMIT 0, 1";
				$bug_result = mysql_query($bug_query, $this->bugzilla_dblink);
				if (!$bug_result) {
					die("bug getBugs: Invalid comments info query");
				}
				
				if (mysql_num_rows($bug_result) > 0){
					$bug_row = mysql_fetch_array($bug_result);
					$xml_view .= $this->XML->createBugList($bug_row[0], $bug_row[1], $bug_row[2], $bug_row[3]);
				}
				
				mysql_free_result($bug_result);
			}
		}

		mysql_free_result($result);
		return $xml_view;
	}
	
};

?>
