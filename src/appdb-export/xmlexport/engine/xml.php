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

/*!
  \class XMLExport
  \brief General XML export calss

  This class provides general xml view generation
  functions

  \author Malakhov Alexey
  \file xml.php
*/

class XMLExport {
	/*!
		\brief Function wich open XML header
		This function will open XML header
		and set header information

		\param action		XML export action description
		\param action		1 -- search app by name or toggle page
		\param action		3 -- view app by id
		\param action		4 -- view app test results by id
		\param action		5 -- view category info

		\return ret		string containg full XML view
	*/
	function openHeader($action){
		global $xmlexport_version;
		$ret = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<appdb_export version=\"{$xmlexport_version}\" action=\"{$action}\">";
		return $ret;
	}

	//! Function wich creates page list
	/*!
	  \param page		current page
	  \param page_count	total page count

	  \return ret		string containg full XML view
	*/
	function createPages($page, $page_count){
		$ret = "
	<page>
		<current>{$page}</current>
		<count>{$page_count}</count>
	</page>";
		return $ret;
	}
	
	//! Function to close XML export header
	function closeHeader(){
		$ret = "
</appdb_export>";
		return $ret;
	}

	//! Function wich creates application XML info
	/*!
	  \param id				application id
	  \param verid			application version id
	  \param name			application name
	  \param desc			application desc
	  \param category		application category
	  \param url			application web page url
	  \param versions_list	application versions list, defalut empty
	  \param category_list	application category list, defalut empty
	  \param test_results	application test results list list, defalut empty
	  \param comment_list	application comment list, defalut empty
	  \param verinfo		application version info, defalut empty
	  \param bugs_list		application bugs list, defalut empty

	  \return ret		string containg full XML view
	*/
	function createAppInfo($id, $verid, $name, $desc, $category, $url, $versions_list="", $category_list="", $test_results="", $comment_list="", $verinfo="", $bugs_list=""){
		$ret = "
	<app id=\"{$id}\"";
		if ($verid>0)
			$ret .= " verid=\"{$verid}\" ";
		$ret .= ">
		<name>" . $this->prepareString($name) . "</name>";
		
		if ($verinfo)
				$ret .= $verinfo;
		
		$ret .= "\n		<desc>";
		if ($test_results){
			$ret .= $this->prepareString($desc, -1, 0);
		} else {
			$ret .= $this->prepareString($desc, 255);
		}
		$ret .= "</desc>
		<category>{$category}</category>
		<url>" . $this->prepareString($url) . "</url>";
		if ($versions_list){
			$ret .= "\n		<version-list>";
			$ret .= $versions_list;
			$ret .= "\n		</version-list>";
		}
		if ($category_list){
			$ret .= "\n		<category-list>";
			$ret .= $category_list;
			$ret .= "\n		</category-list>";
		}
				
		if ($test_results)
			$ret .= $test_results;
		
		if ($bugs_list){
			$ret .= "\n		<bug-list>";
			$ret .= $bugs_list;
			$ret .= "\n		</bug-list>";
		}
		
		if ($comment_list){
			$ret .= "\n		<comment-list>";
			$ret .= $comment_list;
			$ret .= "\n		</comment-list>";
		}
			
		$ret .= "\n	</app>";
		return $ret;
	}

	//! Function wich creates app version XML view
	/*!
	  \param verid			application version id
	  \param appver			application id
	  \param rating			application test rating
	  \param winever		wine version

	  \return ret		string containg full XML view
	*/
	function createAppVersionInfo($verid, $appver, $rating, $winever){
		$ret = "
			<version id=\"{$verid}\">
				<app-ver>" . $this->prepareString($appver) . "</app-ver>
				<rating>{$rating}</rating>
				<wine-ver>" . $this->prepareString($winever) . "</wine-ver>
			</version>";
		return $ret;
	}

	//! Function wich creates app version XML view
	/*!
	  \param testid			application test id
	  \param rating			application test rating
	  \param winever		wine test version
	  \param works			what works
	  \param notworks		what not works
	  \param nottested		what not tested
	  \param comment		comment
	  \param vername		version name
	  \param lic			license

	  \return ret		string containg full XML view
	*/
	function createAppTestingInfo($testid, $rating, $winever, $works, $notworks, $nottested, $comment, $vername, $lic){
		$ret = "
		<app-ver>" . $this->prepareString($vername) . "</app-ver>
		<license>" . $this->prepareString($lic) . "</license>
		<test-result id=\"{$testid}\">
			<rating>{$rating}</rating>
			<wine-ver>" . $this->prepareString($winever) . "</wine-ver>
			<works>" . $this->prepareString($works, -1, 0) . "</works>;
			<not-works>" . $this->prepareString($notworks, -1, 0) . "</not-works>
			<not-tested>" . $this->prepareString($nottested, -1, 0) . "</not-tested>
			<comment>" . $this->prepareString($comment, -1, 0) . "</comment>
		</test-result>";
		return $ret;
	}

	//! Function wich gets top 5 test results XML view
	/*!
	  \param testid			application test id
	  \param rating			application test rating
	  \param winever		wine test version
	  \param testdate		test date
	  \param distrib		distribution
	  \param installs		did installs?
	  \param runs			did runs?
	  \param curr			is current test?

	  \return ret		string containg full XML view
	*/
	function createTop5TestResults($testid, $rating, $winever, $testdate, $distrib, $installs, $runs, $curr){
		$ret = "
			<test id=\"{$testid}\">
				<current>{$curr}</current>
				<distrib>" . $this->prepareString($distrib) . "</distrib>
				<date>{$testdate}</date>
				<wine>" . $this->prepareString($winever) . "</wine>
				<install>";
				if ($installs=="Yes"){
					$ret .= "1";
				} else {
					$ret .= "0";
				}
		$ret .= "</install>
				<run>";
				if ($runs=="Yes"){
					$ret .= "1";
				} else {
					$ret .= "0";
				}
		$ret .= "</run>
				<rating>{$rating}</rating>
			</test>";
		return $ret;
	}

	//! Function wich gets category XML view
	/*!
	\param id			application id
	\param name			application name
	\param desc			application description

	\return ret		string containg full XML view
	*/
	function createCategoryInfo($id, $name, $desc){
		$ret = "
			<category id=\"{$id}\">
				<name>" . $this->prepareString($name) . "</name>
				<desc>" . $this->prepareString($desc, 60) . "</desc>
			</category>";
		return $ret;
	}

	//! Function wich gets category appinfo XML view
	/*!
	  \param id			application id
	  \param name		application name
	  \param desc		application description

	  \return ret		string containg full XML view
	*/
	function createCategoryAppInfo($id, $name, $desc){
		$ret = "
			<app id=\"{$id}\">
				<name>" . $this->prepareString($name) . "</name>
				<desc>" . $this->prepareString($desc, 60) . "</desc>
			</app>";
		return $ret;
	}

	//! Function wich gets comment XML view
	/*!
	  \param id			comment id
	  \param parent		comment parent id
	  \param user		commented user
	  \param subject	comment subject
	  \param body		comment body
	  \param time		comment time

	  \return ret		string containg full XML view
	*/
	function createComment($id, $parent, $user, $subject, $body, $time){
		$ret = "
			<comment id=\"{$id}\">
				<topic>" . $this->prepareString($subject) . "</topic>
				<date>{$time}</date>
				<autor>";
				
		if ($user){
			$ret .= $this->prepareString($user);
		} else {
			$ret .= "Anonymous";
		}

		$ret .= "</autor>
				<message>" . $this->prepareString($body, -1, 0) . "</message>
				<parent>{$parent}</parent>
			</comment>";
		return $ret;
	}

	//! Function wich gets bugs XML view
	/*!
	  \param bug_id			bug id
	  \param bug_status		bug status
	  \param resolution		bug resolution
	  \param short_desc		bug short description

	  \return ret		string containg full XML view
	*/
	function createBugList($bug_id, $bug_status, $resolution, $short_desc){
		$ret = "
			<bug id=\"{$bug_id}\">
				<desc>" . $this->prepareString($short_desc) . "</desc>
				<status>{$bug_status}</status>
				<resolution>{$resolution}</resolution>
			</bug>
		";
		return $ret;
	}

		//! Function wich prepare strings for XML view
		/*!
		  In fact, appdb site allows users to add html tegs into comments, appinfo, name e.t.c.
		  and this will breakes XML parser and formatting, so I need to process strings before
		  add them into xml view

		  \param string			income string
		  \param len			max output string len, default 0
		  \param strip_html		strip html or not?

		  \return string		formatted string
		*/
		function prepareString($string, $len = 0, $strip_html = 1){
			/* This function will prepare string for xml format
			*/
			$string = trim($string);

			if ($strip_html==1){
				//! remove html tags
				$string = strip_tags($string);
				//! remove stupid whitespaces
				$string = preg_replace("/\s+/", " ", $string);
			} else {
				//! strip html tags except <ul><li><b><i><strong>
				$string = strip_tags($string, "<ul><li><b><i><strong>");
				//! Fix for urls
				$string = preg_replace('/(http:\/\/[^\s]+)/', '<a href="$1">$1</a>', $string);
				//! replace new lines into br
				$string = nl2br ($string);
			}

			if ($len > 0)
				$string = substr($string, 0, $len);

			//! confert characters into html elements
			$string = htmlentities($string);
			return $string;
		}

};

?>
