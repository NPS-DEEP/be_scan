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
	be_scan.cpp \
	be_scan.hpp \
	cassandra_db.cpp \
	db.hpp \
	flex_scan_exception.hpp \
	flex_scan_parameters.hpp \
	scanners.hpp \
	stdout_db.cpp

# scanners based on GNUflex
FLEX_INCS = \
	scan_email.flex

lib_LTLIBRARIES = libbe_scan.la

libbe_scan_la_CPPFLAGS = \
	-I${top_srcdir}/datastax

libbe_scan_la_LDFLAGS = -PIC -avoid-version -static -no-undefined

libbe_scan_la_SOURCES = $(FLEX_INCS) $(BE_SCAN_INCS)

include_HEADERS = be_scan.hpp

.flex.cpp:
	$(LEX) -R -t $< > $@

