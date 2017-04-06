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

# This file assists in building test programs.

EXTRA_DIST = vg.supp

check_PROGRAMS = basic_tests

TESTS = $(check_PROGRAMS)

AM_CFLAGS = $(BE_SCAN_CFLAGS)
AM_CXXFLAGS = $(BE_SCAN_CXXFLAGS)

AM_LDFLAGS = -static

LDADD = ../src/libbe_scan.la

BASIC_TESTS_INCS = \
	basic_tests.cpp \
	be_scan.hpp \
	escape.hpp \
	unit_test.h

run_tests_valgrind:
	for test in $(check_PROGRAMS); do \
	  echo Begin $$test ... ; \
	  valgrind --tool=memcheck --leak-check=full --show-reachable=yes --suppressions=vg.supp ./$$test; \
	done

# ############################################################
# tests
# ############################################################
basic_tests_SOURCES = $(BASIC_TESTS_INCS)

.PHONY: run_tests_valgrind

