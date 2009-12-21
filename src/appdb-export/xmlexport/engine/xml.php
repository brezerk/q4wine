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

Class XMLExport { 
	function createHeader($action){
		$ret = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>       
<appdb_export version=\"0.1\" action=\"{$action}\">";
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
	
	function createAppInfo($id, $name, $desc, $category, $url, $versions_list="", $category_list="", $test_results=""){
		$ret = "
	<app id=\"{$id}\">
		<name>{$name}</name>
		<desc>{$desc}</desc>
		<category>{$category}</category>
		<url>{$url}</url>";
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
$ret .= "\n	</app>";
		return $ret;
	}
	
	function createAppVersionInfo($verid, $appver, $rating, $winever){
		$ret = "
			<version id=\"{$verid}\">
				<app-ver>{$appver}</app-ver>
				<rating>{$rating}</rating>
				<wine-ver>{$winever}</wine-ver>
			</version>";
		return $ret;
	}
	
	function createAppTestingInfo($testid, $rating, $winever, $works, $notworks, $nottested, $comment){
		$ret = "
		<test-result id=\"{$testid}\">
			<rating>{$rating}</rating>
			<wine-ver>{$winever}</wine-ver>
			<works>" . $this->prepareString($works, -1, 0) . "</works>;
			<not-works>" . $this->prepareString($notworks, -1, 0) . "</not-works>
			<not-tested>" . $this->prepareString($nottested, -1, 0) . "</not-tested>
			<comment>" . $this->prepareString($comment, -1, 0) . "</comment>
		</test-result>";
		return $ret;
	}
	
	function createCategoryInfo($id, $name, $desc){
		$ret = "
			<category id=\"{$id}\">
				<name>{$name}</name>
				<desc>{$desc}</desc>
			</category>";
		return $ret;
	}
	
	function createCategoryAppInfo($id, $name, $desc){
		$ret = "
			<app id=\"{$id}\">
				<name>{$name}</name>
				<desc>{$desc}</desc>
			</app>";
		return $ret;
	}
	
	function prepareString($string, $len = 0, $strip_html = 1){
		/* This function will prepare string for xml format
		 */
				
		//Remove stupid whitespaces
		$string = trim(preg_replace("/\s+/", " ", $string));
		
		if ($strip_html==1){
			$string = strip_tags($string);
		} else {
			$string = strip_tags($string, "<p><a><br><pre><ul><li><b><i>");
		}
		
		if ($len > 0)
			$string = substr($string, 0, $len);
		
		$string = str_replace("#", "", $string);
		$string = str_replace("<br />", "", $string);
		$string = str_replace("&", "&amp;", $string);
		$string = str_replace(">", "&gt;", $string);
		$string = str_replace("<", "&lt;", $string);	
		
		//Remove for <br /> e.t.c.
		
		
		return $string;
	}

};

?>
