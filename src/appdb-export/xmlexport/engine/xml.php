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

Class XMLExport { 
	function openHeader($action){
		global $xmlexport_version;
		$ret = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>       
<appdb_export version=\"{$xmlexport_version}\" action=\"{$action}\">";
		return $ret;
	}
	
	function createPages($page, $page_count){
		$ret = "
	<page>
		<current>{$page}</current>
		<count>{$page_count}</count>
	</page>";
		return $ret;
	}
	
	
	function closeHeader(){
		$ret = "
</appdb_export>";
		return $ret;
	}
	
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
	
	function createAppVersionInfo($verid, $appver, $rating, $winever){
		$ret = "
			<version id=\"{$verid}\">
				<app-ver>" . $this->prepareString($appver) . "</app-ver>
				<rating>{$rating}</rating>
				<wine-ver>" . $this->prepareString($winever) . "</wine-ver>
			</version>";
		return $ret;
	}
	
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
	
	function createTop5TestResults($testid, $rating, $winever, $testdate, $distrib, $installs, $runs, $curr){
		$ret = "
			<test id=\"{$testid}\">
				<current>{$curr}</current>
				<distrib>" . $this->prepareString($distrib) . "</distrib>
				<date>{$testdate}</date>
				<wine>" . $this->prepareString($winever) . "</wine>
				<instal>{$installs}</instal>
				<run>{$runs}</run>
				<rating>{$rating}</rating>
			</test>";
		return $ret;
	}
	
	function createCategoryInfo($id, $name, $desc){
		$ret = "
			<category id=\"{$id}\">
				<name>" . $this->prepareString($name) . "</name>
				<desc>" . $this->prepareString($desc, 60) . "</desc>
			</category>";
		return $ret;
	}
	
	function createCategoryAppInfo($id, $name, $desc){
		$ret = "
			<app id=\"{$id}\">
				<name>" . $this->prepareString($name) . "</name>
				<desc>" . $this->prepareString($desc, 60) . "</desc>
			</app>";
		return $ret;
	}
	
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
	
	function prepareString($string, $len = 0, $strip_html = 1){
		/* This function will prepare string for xml format
		 */
			
		$string = trim($string);
				
		if ($strip_html==1){
			$string = strip_tags($string);
			//Remove stupid whitespaces
			$string = preg_replace("/\s+/", " ", $string);
		} else {
			$string = strip_tags($string, "<ul><li><b><i>");
			$string = preg_replace('/(http:\/\/[^\s]+)/', '<a href="$1">$1</a>', $string);
			$string = nl2br ($string);
			$string = preg_replace("/(<br \/>\s+)+/", "<br /><br />", $string);
			$string = preg_replace("/<br \/><br \/>$/", "", $string);

		}

		if ($len > 0)
			$string = substr($string, 0, $len);
		

		$string = htmlentities($string);
		
		
		return $string;
	}

};

?>
