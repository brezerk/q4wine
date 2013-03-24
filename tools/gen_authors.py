#!/usr/bin/python
#
# Copyright (C) 2008, 2009, 2010, 2011 by Malakhov Alexey
# brezerk@gmail.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import argparse
import os
import yaml

D_DATADIR = os.path.join(os.path.dirname(__file__), '../src/q4wine-gui')

def html_head(group):
    return u"""
#define T_%s \"<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' 'http://www.w3.org/TR/REC-html40/strict.dtd'>\" \\
\"<html>\" \\
    \"<head>\" \\
        \"<meta name='qrichtext' content='1' />\" \\
	\"<style type='text/css'>p, li { white-space: pre-wrap; }</style>\" \\
    \"</head>\" \\
    \"<body>\" \\""" % group.upper()

def html_footer():
    return u"""
     \"</body>\" \\
\"<html>\"
"""

def html_line(text=" "):
    return u"""
        \"<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>\" \\
	    \"%s\" \\
	\"</p>\" \\""" % text

def html_mail_to(mail, text='E-Mail'):
    return u"""
        \"<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>\" \\
	    \"<span style='color:#6495ed;'>%s</span>: <a href='mailto:%s' style='text-decoration: none; color:#5f9ea0;'>%s</a>\" \
	\"</p>\" \\""" % (text, mail, mail)

def html_web(web):
    return u"""
        \"<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;'>\" \\
	    \"<span style='color:#6495ed;'>Web</span>: <a href='%s' style='color:#5f9ea0;'>%s</a>\" \
	\"</p>\" \\""" % (web, web)

def generate_authors():
    filename = os.path.join(D_DATADIR, 'authors.yaml')
    yamlfile = open(filename, 'r')

    filename = os.path.join(D_DATADIR, 'authors.h')
    with open(filename, 'w') as h:

        data = yaml.safe_load(yamlfile)
        for group, persons in data.iteritems():
            h.write(html_head(group))
            for person in persons:
		nick = ""
                if person.has_key('nick'):
                    nick = u"(%s)" % person['nick']
		h.write(html_line(u"%s %s" % (person['name'], nick)))
		h.write(html_line(person['info']))
                if person.has_key('mail'):
                    h.write(html_mail_to(person['mail']))
                if person.has_key('jid'):
                    h.write(html_mail_to(person['jid'], "Jabber"))
                if person.has_key('web'):
                    h.write(html_web(person['web']))
                if person.has_key('location'):
                    h.write(html_line(person['location']))
                h.write(html_line())
            h.write(html_footer())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Parse authors.jaml file and create authors.h file.')
    parser.add_argument('--generate', dest='generate', action='store_true',
                        required=True,
                        help='Generate authors.h file')

    args = parser.parse_args()

    if args.generate:
        generate_authors()
