# Author:  Bruce Allen
# Created: 2/25/2013
#
# The software provided here is released by the Naval Postgraduate
# School, an agency of the U.S. Department of Navy.  The software
# bears no warranty, either expressed or implied. NPS does not assume
# legal liability nor responsibility for a User's use of the software
# or the results of such use.
#
# Please note that within the United States, copyright protection,
# under Section 105 of the United States Code, Title 17, is not
# available for any work of the United States Government and/or for
# any works created by United States Government employees. User
# acknowledges that this software contains work which was created by
# NPS government employees and is therefore in the public domain and
# not subject to copyright.
#
# Released into the public domain on March 2, 2017 by Bruce Allen.

# This file assists in building libbe_scan.so

BE_SCAN_INCS = \
	be_scan.cpp \
	be_scan.hpp \
	escape.cpp \
	read_buffer.cpp \
	read_buffer.hpp \
	scan_email.cpp \
	scanner_data.hpp \
	scanners.cpp \
	scanners.hpp \
	write_artifact.cpp \
	write_artifact.hpp

lib_LTLIBRARIES = libbe_scan.la

AM_CFLAGS = $(BE_SCAN_CFLAGS)
AM_CXXFLAGS = $(BE_SCAN_CXXFLAGS)

libbe_scan_la_LDFLAGS = -PIC

libbe_scan_la_SOURCES = $(BE_SCAN_INCS)

include_HEADERS = be_scan.hpp

