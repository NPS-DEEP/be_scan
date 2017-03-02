# Author:  Bruce Allen bdallen nps edu
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

# This file assists in building the be_scan Python bindings

# http://realmike.org/blog/2010/07/18/building-swig-python-extensions-on-gnulinux/
# https://github.com/roryyorke/swig-python-autotools-example/blob/master/Makefile.am

## the be_scan_wrap.cpp source and also be_scan.py that will be generated
BUILT_SOURCES = be_scan_wrap.cpp

# recipe for be_scan_wrap.c
# note that SWIG creates be_scan_wrap.c and be_scan.py
# SWIG needs to find be_scan.hpp to prepare Python interfaces
be_scan_wrap.cpp: be_scan.i \
               ${top_srcdir}/src_libbe_scan/be_scan.hpp \
               ../src_libbe_scan/libbe_scan.la
	rm -f be_scan.hpp
	ln -s ${top_srcdir}/src_libbe_scan/be_scan.hpp .
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -threads -o $@ $<
	rm be_scan.hpp

# install be_scan.py
python_PYTHON = $(builddir)/be_scan.py

# module, so no lib prefix
_be_scan_la_LDFLAGS = -module ${PYTHON_LDFLAGS}

# install _be_scan.la
pyexec_LTLIBRARIES = _be_scan.la

# the files that _be_scan.la depends on
_be_scan_la_SOURCES = be_scan_wrap.cpp be_scan.i

# _be_scan.la requires relocatable be_scan library libbe_scan.so
_be_scan_la_LIBADD = ../src_libbe_scan/libbe_scan.la

# c++ flags for compiling _be_scan.la
_be_scan_la_CPPFLAGS = ${PYTHON_CPPFLAGS} -Wno-deprecated-register -I${top_srcdir}/src_libbe_scan


# for testing
AM_TESTS_ENVIRONMENT = export PYTHONPATH='.:.libs';
testfile = test_be_scan.py
EXTRA_DIST = $(testfile)
TESTS = $(testfile)
clean-local:
	rm -f be_scan.py be_scan_wrap.cpp be_scan.hpp
	rm -rf temp_*

