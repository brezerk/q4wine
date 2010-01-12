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

	/** @name Wine Appdb database connection settings */
	///@{
	//! Wine Appdb MySQL host
	$appdb_host="localhost";
	//! Wine Appdb MySQL base
	$appdb_base="apidb";
	//! Wine Appdb MySQL user
	//! \warning Please: <b>DO NOT</b> give write priveleges to this script!
	$appdb_user="";
	//! Wine Appdb MySQL passord
	//! \warning Please: <b>DO NOT</b> give write priveleges to this script!
	$appdb_pass="";
	///@}
	
	/** @name Wine Bugzilla database connection settings */
	///@{
	//! Wine Bugzilla MySQL host
	$bugzilla_host="localhost";
	//! Wine Bugzilla MySQL base
	$bugzilla_base="bugs";
	//! Wine Bugzilla MySQL user
	//! \warning Please: <b>DO NOT</b> give write priveleges to this script!
	$bugzilla_user="";
	//! Wine Bugzilla MySQL user
	//! \warning Please: <b>DO NOT</b> give write priveleges to this script!
	$bugzilla_pass="";
	///@}

	/*!
	  We can filter for client header.
	  It will make ddos a little bit harder ;)
	*/
	$white_agents = array();

	/*!
	  Default filter is set to q4wine user-agent
	  You can make it blank to disable this feature
	*/
	$white_agents[0] = "^q4wine/[0-1].[1-9]{3}([ ]|-[0-9a-zA-Z]+[ ])\([0-9a-zA-Z]+; [0-9a-zA-Z]+ [0-9a-zA-Z]+) xmlparser/[0-9].[0-9]$";

	//! define xmlexport version
	$xmlexport_version = "0.1";

?>
