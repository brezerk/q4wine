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

/*!
  \class DB
  \brief General Database calss

  This class provides general database functions
  it will extract data from wine appdb and bugzilla
  bases and request xml class function to create
  xml view

  \author Malakhov Alexey
  \file db.php
*/

class DB {

	//! \brief Wine AppDb database link
	private $appdb_dblink;
	//! \brief Wine Bugzilla database link
	private $bugzilla_dblink;
	//! \brief XML class link
	private $XML;

public:
	/*!
	  \brief Database class constructor
	
	  Database constructor will connects to databases
	  and creates database links.

	  Also we will call XML class constructor
	*/
	function __construct(){
	  
		//! Geting global config values from ./cfg/config.inc
		global $appdb_host;
		global $appdb_base;
		global $appdb_user;
		global $appdb_pass;
	  
		global $bugzilla_host;
		global $bugzilla_base;
		global $bugzilla_user;
		global $bugzilla_pass;

		//! Connecting to appdb database
		/*! First, when connecting, the funct would first try to find a 
		  (persistent) link that's already open with the same host,
		  username and password. If one is found, an identifier for
		  it will be returned instead of opening a new connection.
		*/

		$this->appdb_dblink = mysql_pconnect($appdb_host, $appdb_user, $appdb_pass) or die("[ee] Could not connect to appdb database: " . mysql_error());
		mysql_select_db($appdb_base, $this->appdb_dblink) or die("[ee] Could not select appdb database");

		//!Connect to wine bugzilla database
		$this->bugzilla_dblink = mysql_pconnect($bugzilla_host, $bugzilla_user, $bugzilla_pass) or die("[ee] Could not connect to appdb database: " . mysql_error());
		mysql_select_db($bugzilla_base, $this->bugzilla_dblink) or die("[ee] Could not select appdb database");

		//! Init for XML class link and call for XMLExport constructor
		$this->XML = new XMLExport();
		return;
	}

	/*!
	  \brief Function for creating Application search view by app name
	
	  This function will search for application name
	  and return XML view for 10 search matches.

	  Also it returns pages count

	  \param search		Search criteria (usually it is application name)
	  \param page		Start page, default is 1 (first)

	  \return xml_view	string containg full XML view
	*/
	  
	function exportAppByName($search, $page = 1){
		//! Get only first 20 characters
		//! this will makes sql injection more harder ;)
		$search = substr($search, 0, 20);

		//! Convert page variable into int type
		$page = (int)$page;
		if ($page <= 0){
			$sql_page = 0;
		} else {
			$sql_page = ($page - 1) * 10;
		}
		
		global $appdb_base;

		//! query for search muches count to get page count
		$search = mysql_real_escape_string($search, $this->appdb_dblink);
		$query = "SELECT COUNT(appId) FROM {$appdb_base}.appFamily WHERE appName LIKE \"%${search}%\" AND state='accepted'";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
			if (mysql_num_rows($result) > 0){
				//! Creates xml header and add pages info
				$row = mysql_fetch_row($result);
				$xml_view = $this->XML->openHeader(1);
				$xml_view .= $this->XML->createPages($page, ceil($row[0]/10));
			}
			
		//! Free the resources associated with the result set
		mysql_free_result($result);

		//! query for first 10 application records with specifyed start page
		$query = "SELECT appId, appName, description, catId, webPage FROM {$appdb_base}.appFamily WHERE appName LIKE \"%${search}%\" AND state='accepted' LIMIT ${sql_page}, 9";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("serachAppByName: Invalid query");
			}
		
		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n	<app-list>";
			while ($row = mysql_fetch_assoc($result)) {
				//! Getting aplication version
				//! \sa getVersion()
				$appver_list = $this->getVersion($row['appId']);
				//! Creating AppInfo XML view
				$xml_view .= $this->XML->createAppInfo($row['appId'], 0, $row['appName'], $row['description'], $row['catId'], $row['webPage'], $appver_list);
			}
			$xml_view .= "\n	</app-list>";
		} else {
			//! If no application found -- creates error header
			$xml_view = $this->XML->openHeader(6);
			$xml_view .= "<message>No matches found</message>";
		}

		//! Close XML view header
		$xml_view .= $this->XML->closeHeader();
		
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for creating application XML view
	/*!
	  This function will create application XML view by app ID

	  \param appid		application ID
	  
	  \return xml_view	string containg full XML view
	*/
	
	function exportAppById($appid){
		//! Convert app id into int type
		$appid = (int)$appid;
		if ($appid <= 0)
			return;

		//! Creates XML header
		$xml_view = $this->XML->openHeader(3);
		//! Get app database info by appid
		//! \sa getApp()
		$xml_view .= $this->getApp($appid);
		//! Close XML view header
		$xml_view .= $this->XML->closeHeader();
	
		return $xml_view;
	}

	//! Function for creating application test result XML view
	/*!
	  This function will create application test result view by appid, verid or testid

	  \param appid		application ID
	  \param verid		application version ID, default 0
	  \param testid		application test ID, default 0

	  \return xml_view	string containg full XML view
	*/
	function exportTestResults($appid, $verid = 0, $testid = 0){
		//! Convert into int type
		$appid = (int)$appid;
		if ($appid < 0)
			return;

		//! Convert into int type
		$verid = (int)$verid;
		if ($verid < 0)
			return;

		//! Convert into int type
		$testid = (int)$testid;
		if ($testid < 0)
			return;

		//! Creates XML header
		$xml_view = $this->XML->openHeader(4);
		//! Get app database info by appid
		//! \sa getApp()
		$xml_view .= $this->getApp($appid, $verid, $testid);
		//! Close XML view header
		$xml_view .= $this->XML->closeHeader();
				
		return $xml_view;
	}
	
	//! Function for creating category list XML view
	/*!
	  This function will create application category XML view by catid

	  \param catid		category ID

	  \return xml_view	string containg full XML view
	*/
	function exportCategory($catid){
		//! Convert into int type
		$catid = (int)$catid;
		if ($catid < 0)
			return;
		
		global $appdb_base;

		//! Creates XML header
		$xml_view = $this->XML->openHeader(5);
		$xml_view .= "\n		<category-list>";
		$xml_view .= $this->XML->createCategoryInfo(0, "Main", "");
		if ($catid>0){
			//! Getting category info
			//! \sa getCategory()
			$xml_view .= $this->getCategory($catid);
		}
		$xml_view .= "\n		</category-list>";

		//! query subcategory list by parent ID
		$query = "SELECT catId, catName, catDescription FROM {$appdb_base}.appCategory WHERE catParent={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
		if (!$result) {
			die("serachCategoryById: Invalid query");
		}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<subcategory-list>";
			while ($row = mysql_fetch_assoc($result)){
				//! get category view
				$xml_view .= $this->XML->createCategoryInfo($row['catId'], $row['catName'], $row['catDescription']);
			}
			$xml_view .= "\n		</subcategory-list>";
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);
		
		//! query application data by category ID
		$query = "SELECT appId, appName, description FROM {$appdb_base}.appFamily WHERE state='accepted' and catId={$catid}";
		$result = mysql_query($query, $this->appdb_dblink);
		if (!$result) {
			die("serachCategoryById: Invalid query");
		}

		if (mysql_num_rows($result) > 0){
			$xml_view .= "\n		<app-list>";
			while ($row = mysql_fetch_assoc($result)){
				//! get appinfo view
				$xml_view .= $this->XML->createCategoryAppInfo($row['appId'], $row['appName'], $row['description']);
			}
			$xml_view .= "\n		</app-list>";
		} 

		//! Close XML view header
		$xml_view .= $this->XML->closeHeader();
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

private:
	//! Function for getting app versions info form database
	/*!
	  This function will get app versions info data by appid

	  \param appid		application ID

	  \return xml_view	string containg full XML view
	*/
	function getVersion($appid){
		//! Convert into int type
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
			
		global $appdb_base;

		//! query for application data by app ID
		$query = "SELECT versionId, versionName, rating, ratingRelease FROM {$appdb_base}.appVersion WHERE appId={$appid} AND state='accepted'";

		$result = mysql_query($query, $this->appdb_dblink);
		if (!$result) {
			die("getVersion: Invalid query");
		}
		
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_assoc($result)) {
				//! create app verion XML view
				$xml_view .= $this->XML->createAppVersionInfo($row['versionId'], $row['versionName'], $row['rating'], $row['ratingRelease']);
			}
		}
		
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting app info form database
	/*!
	  This function will get app nfo data by appid

	  \param appid		application ID
	  \param verid		application version ID, default 0
	  \param verid		If not eq 0, we will get version info
	  \param testid		application test ID, default 0
	  \param testid		If not eq 0, we will get test info

	  \return xml_view	string containg full XML view
	*/
	function getApp($appid, $verid=0, $testid=0){
		//! Convert into int type
		$appid = (int)$appid;
		if ($appid <= 0)
			return;

		//! Convert into int type
		$verid = (int)$verid;
		$testid = (int)$testid;

		global $appdb_base;

		//! query for app info by appid
		$query = "SELECT appId, appName, description, catId, webPage FROM {$appdb_base}.appFamily WHERE appId={$appid} AND state='accepted' LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getApp: Invalid app info query");
			}
				
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			//! creates category XML view
			$category_list = $this->XML->createCategoryInfo(0, "Main", "");
			//! getting category info
			//! \sa getCategory()
			$category_list .= $this->getCategory($row['catId']);

			if ($verid<=0){
				//! if verid <=0, then we just get app version info
				//! \sa getVersion()
				$versions_list = $this->getVersion($row['appId']);
			} else {
				//!else, we get full test results info includeing notes, comments e.t.c.
				//! \sa getTestResults()
				//! \sa getNotes()
				//! \sa getComments()
				//! \sa getBugs()
				$test_results = $this->getTestResults($verid, $testid);
				$comment_list = $this->getNotes($appid, $verid);
				$comment_list .= $this->getComments($verid);
				$bugs_list = $this->getBugs($verid);
			}
			//! Just create XML app info view
			$xml_view .= $this->XML->createAppInfo($row['appId'], $verid, $row['appName'], $row['description'], $row['catId'], $row['webPage'], $versions_list, $category_list, $test_results, $comment_list, $verinfo, $bugs_list);
		}

		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting app test results info form database
	  /*!
	  This function will get app test results info data by verid and testid

	  \param verid		application version ID
	  \param testid		application test ID

	  \return xml_view	string containg full XML view
	*/
	function getTestResults($verid, $testid){
		//! Convert into int type
		$verid = (int)$verid;
		$testid = (int)$testid;
	  
		if (($testid <= 0) and ($verid <= 0))
			return;

		global $appdb_base;

		//!If testid <= 0, then we get test results by app version ID, else by app test ID
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
			//! query for app version details
			$query = "SELECT versionName, license FROM {$appdb_base}.appVersion WHERE versionId={$row[7]}";
			$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getTestResults: Invalid app info query");
			}
			if (mysql_num_rows($result) > 0){
				$verrow = mysql_fetch_array($result);
			}

			//! create app test info XML view
			$xml_view = $this->XML->createAppTestingInfo($row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $verrow[0], $verrow[1]);
			//! Getting top 5 test resilts
			//! \sa getTestResults_Top5()
			$xml_view .= $this->getTestResults_Top5($verid, $row[0]);	
		}

		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting category info form database
	/*!
	  This function will get category info data by catid

	  \param catid		category ID

	  \return xml_view	string containg full XML view
	*/
	function getCategory($catid){
		//! Convert into int type
		$catid = (int)$catid;
		if ($catid < 0)
			return;
	
		global $appdb_base;

		//! query for category info by catid
		$query = "SELECT catId, catName, catDescription, catParent FROM {$appdb_base}.appCategory WHERE catId={$catid} LIMIT 0, 1";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getCategory: Invalid category info query");
			}
		
		if (mysql_num_rows($result) > 0){
			$row = mysql_fetch_assoc($result);
			if ($row['catParent']>0){
				//! get subcategories data
				$xml_view .= $this->getCategory($row['catParent']);
			}
			//! just create category XML view
			$xml_view .= $this->XML->createCategoryInfo($row['catId'], $row['catName'], $row['catDescription']);
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);	
		return $xml_view;
	}

	//! Function for getting top5 test results info form database
	/*!
	  This function will get top5 test results info data by catid

	  \param verid		app version ID
	  \param curtest	current test ID

	  \return xml_view	string containg full XML view
	*/
	function getTestResults_Top5($verid, $curtest){
		//! Convert into int type
		$verid = (int)$verid;
		if ($verid < 0)
			return;
			
		$curtest = (int)$curtest;
		if ($curtest < 0)
			return;

		global $appdb_base;
		//! query for 5 test results
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
				//! just create test list XML view
				$xml_view .= $this->XML->createTop5TestResults($row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $curr);
			}
			$xml_view .= "\n		</test-list>";
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting test comments info form database
	/*!
	  This function will get test comments info data by verid

	  \param verid		app version ID
	  \param parent		parent ID, default 0

	  \return xml_view	string containg full XML view
	*/
	function getComments($verid, $parent=0){
		$verid = (int)$verid;
		if ($verid <= 0)
			return;
			
		$parent = (int)$parent;
		if ($parent < 0)
			return;
			
		global $appdb_base;
		//! query for comment data
		$query = "SELECT commentId, parentId, (SELECT realname FROM {$appdb_base}.user_list WHERE userid={$appdb_base}.appComments.userId), subject, body, DATE_FORMAT(time, '%e %b %Y %H:%i:%s') FROM {$appdb_base}.appComments WHERE versionId={$verid} AND parentId={$parent} ORDER BY time DESC";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getComments: Invalid comments info query");
			}
				
		if (mysql_num_rows($result) > 0){
			//$xml_view = "\n		<test-list>";
			while ($row = mysql_fetch_array($result)){
				//! just create comment view
				$xml_view .= $this->XML->createComment($row[0], $row[1], $row[2], $row[3], $row[4], $row[5]);
				//! Get parent comments
				$xml_view .= $this->getComments($verid, $row[0]);
			}
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting test notes info form database
	/*!
	  This function will get test notes info data by verid
	  
	  \param appid		app ID
	  \param verid		app version ID

	  \return xml_view	string containg full XML view
	*/
	function getNotes($appid, $verid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
		
		$verid = (int)$verid;
		if ($verid <= 0)
			return;
			
		global $appdb_base;
		//! query for notes data
		$query = "SELECT noteTitle, noteDesc, DATE_FORMAT(submitTime, '%e %b %Y %H:%i:%s'), (SELECT realname FROM {$appdb_base}.user_list WHERE userid={$appdb_base}.appNotes.submitterId) FROM {$appdb_base}.appNotes WHERE versionId={$verid} OR (appId={$appid} AND versionId=0) ORDER BY noteId";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getComments: Invalid comments info query " . $query);
			}
				
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_array($result)){
				//! just creates XML view
				$xml_view .= $this->XML->createComment(0, 0, $row[5], $row[0], $row[1], $row[2]);
			}
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}

	//! Function for getting buglist info form database
	/*!
	  This function will get buglist info data by appid

	  \param appid		app ID

	  \return xml_view	string containg full XML view
	*/
	function getBugs($appid){
		$appid = (int)$appid;
		if ($appid <= 0)
			return;
			
		global $bugzilla_base;
		global $appdb_base;

		//! query forapplication  buglist
		$query = "SELECT bug_id FROM {$appdb_base}.buglinks WHERE versionId={$appid} AND state='accepted' ORDER BY bug_id";
		$result = mysql_query($query, $this->appdb_dblink);
			if (!$result) {
				die("getBugs: Invalid comments info query");
			}
				
		if (mysql_num_rows($result) > 0){
			while ($row = mysql_fetch_array($result)){
				//! query for bug info
				$bug_query = "SELECT bug_id, bug_status, resolution, short_desc FROM {$bugzilla_base}.bugs WHERE bugs.bug_id={$row[0]} AND (bugs.bug_status='NEW' OR bugs.bug_status='UNCONFIRMED') LIMIT 0, 1";
				$bug_result = mysql_query($bug_query, $this->bugzilla_dblink);
				if (!$bug_result) {
					die("bug getBugs: Invalid comments info query");
				}
				
				if (mysql_num_rows($bug_result) > 0){
					$bug_row = mysql_fetch_array($bug_result);
					$xml_view .= $this->XML->createBugList($bug_row[0], $bug_row[1], $bug_row[2], $bug_row[3]);
				}
				//! Free the resources associated with the result set
				mysql_free_result($bug_result);
			}
		}
		//! Free the resources associated with the result set
		mysql_free_result($result);
		return $xml_view;
	}
	
};

?>
