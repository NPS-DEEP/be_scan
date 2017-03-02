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

EXTRA_DIST = \
	liblmdb/COPYRIGHT \
	liblmdb/LICENSE

BE_SCAN_INCS = \
	be_scan.h

libbe_scan_la_LDFLAGS = -PIC -avoid-version -static -no-undefined

libbe_scan_la_SOURCES = $(BE_SCAN_INCS)
