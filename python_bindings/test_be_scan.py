#!/usr/bin/env python

# This script tests the Python version and hashdb library functions.
# This script creates filenames starting with "temp_" in the local directory.

# ############################################################
# test Python version
# ############################################################
import sys
import be_scan
import shutil
import struct
import io
import os

# comparison tests
def str_equals(a,b):
    if a != b:
        print("a", ":".join("{:02x}".format(ord(c)) for c in a))
        print("b", ":".join("{:02x}".format(ord(c)) for c in b))
        raise ValueError("'%s' not equal to '%s'" % (a,b))
def bool_equals(a,b):
    if a != b:
        raise ValueError(a + " not equal to " + b)
def int_equals(a,b):
    if a != b:
        raise ValueError(str(a) + " not equal to " + str(b))

# be_scan interface tests
def test_environment():
    # require Python version 2.7
    if sys.version_info.major != 2 and sys.version_info.minor != 7:
        print("Error: Invalid Python version: 2.7 is required.")
        print(sys.version_info)
        sys.exit(1)

    # require 64-bit Python
    if sys.maxsize != 2**63 - 1:
        print("Error: 64-bit Python is required.")
        print("found %d but expected %d" % (sys.maxsize, 2**64))
        sys.exit(1)

def test_version():
    str_equals(be_scan.version()[:2], "0.")

def test_loopback():
    scanner = be_scan.be_scan_t("email", "\0a\0a\0", 5)
    str_equals(scanner.test_loopback(), "\0a\0a\0")
    print("len: %d" % len(scanner.test_loopback()))

def test_escape():
    str_equals(be_scan.escape("\0"), "\\x00")

# main
if __name__=="__main__":
    test_environment()
    test_version()
    test_loopback()
    test_escape()

    print("Done.")

