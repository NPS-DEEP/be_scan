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

CLEANFILES = scan_email.cpp

BE_SCAN_INCS = \
	be_scan.hpp \
	version.cpp

# scanners based on GNUflex
FLEX_INCS = \
	flex_common.hpp \
	scan_email.flex

lib_LTLIBRARIES = libbe_scan.la

libbe_scan_la_CPPFLAGS = \
	-I${top_srcdir}/cassandra

libbe_scan_la_LDFLAGS = -PIC -avoid-version -static -no-undefined

#libbe_scan_la_SOURCES = $(BE_SCAN_INCS) $(FLEX_INCS)
libbe_scan_la_SOURCES = scan_email.flex version.cpp

include_HEADERS = be_scan.hpp

.flex.cpp:
	$(LEX) -R -t $<  > `basename $@ .o`.cpp 

#
# Rule to compile .flex into a .o
# FreeBSD NOTE: flex does not support -R and -o does not work properly
# | sed "s/static int yy_get_next_buffer/static int XXyy_get_next_buffer/" 

#SUFFIXES = .flex
#FLEX_CXXFLAGS=-Wno-shadow -Wno-effc++ -Wno-redundant-decls -Wno-missing-noreturn
#.flex.o:
#	$(LEX) -R -t $<  > `basename $@ .o`.cpp 
#	$(CXX) $(CPPFLAGS) $(AM_CPPFLAGS) $(CXXFLAGS) $(FLEX_CXXFLAGS) \
#		-I${top_srcdir} -I${srcdir} -I.. -I../../_build -c `basename $@ .o`.cpp 
#
