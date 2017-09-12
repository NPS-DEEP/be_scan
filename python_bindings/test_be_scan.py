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

def test_buffer():
    # set up and scan
    buf = "someone@somewhere.com\0someone2@somewhere2.com\0"
    scan_engine = be_scan.scan_engine_t("email")
    str_equals(scan_engine.status, "")
    scanner = be_scan.scanner_t(scan_engine)
    scanner.scan_setup("stream name", "recursion prefix")
    status = scanner.scan_stream(0, "", buf)
    str_equals(status, "")
    status = scanner.scan_final(0, "", "")
    str_equals(status, "")
    status = scanner.scan_fence_final(0, "", "")
    str_equals(status, "")

    # validate artifacts
    artifact = scanner.get()
    str_equals(artifact.artifact_class, "email")
    str_equals(artifact.stream_name, "stream name")
    str_equals(artifact.recursion_prefix, "recursion prefix")
    str_equals(artifact.offset, 0)
    str_equals(artifact.artifact, "someone@somewhere.com")
    str_equals(artifact.context, "someone@somewhere.com\0someone2@somewh")
    artifact = scanner.get()
    str_equals(artifact.artifact_class, "email")
    str_equals(artifact.stream_name, "stream name")
    str_equals(artifact.recursion_prefix, "recursion prefix")
    str_equals(artifact.offset, 22)
    str_equals(artifact.artifact, "someone2@somewhere2.com")
    str_equals(artifact.context, "e@somewhere.com\0someone2@somewhere2.com\0")
    artifact = scanner.get()
    bool_equals(artifact.blank(), True)
    str_equals(artifact.artifact_class, "")
    str_equals(artifact.stream_name, "")
    str_equals(artifact.recursion_prefix, "")
    str_equals(artifact.offset, 0)
    str_equals(artifact.artifact, "")
    str_equals(artifact.context, "")

def test_escape():
    str_equals(be_scan.escape("a\0b"), "a\\x00b")

def test_uncompressor():
    uncompressor = be_scan.uncompressor_t()
    uncompressed = uncompressor.uncompress("", 1)
    str_equals(uncompressed.buffer, "")
    str_equals(uncompressed.status,
               "ERROR: uncompressor signature not recognized")
    uncompressor.close()
    uncompressed = uncompressor.uncompress("", 0)
    str_equals(uncompressed.status, "ERROR: uncompressor scratch buffer has been closed")

# main
if __name__=="__main__":
    test_environment()
    test_version()
    test_buffer()
    test_escape()
    test_uncompressor()

    print("Done.")

