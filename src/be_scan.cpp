// Author:  Bruce Allen
// Created: 3/2/2017
//
// The software provided here is released by the Naval Postgraduate
// School, an agency of the U.S. Department of Navy.  The software
// bears no warranty, either expressed or implied. NPS does not assume
// legal liability nor responsibility for a User's use of the software
// or the results of such use.
//
// Please note that within the United States, copyright protection,
// under Section 105 of the United States Code, Title 17, is not
// available for any work of the United States Government and/or for
// any works created by United States Government employees. User
// acknowledges that this software contains work which was created by
// NPS government employees and is therefore in the public domain and
// not subject to copyright.
//
// Released into the public domain on March 2, 2017 by Bruce Allen.

#include <config.h>
#include <string>
#include <cstring>
#include <set>
#include <iostream>
#include "be_scan.hpp"
#include "scan_email.hpp"

const char* be_scan_version() {
  return PACKAGE_VERSION;
}

// split()
// adapted from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
std::set<std::string> split(const std::string &s, char delim) {
  std::set<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) {
    elems.insert(item);
  }
  return elems;
}

namespace be_scan {

  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

  // available scanners
  std::string available_scanners() {
    return "email";
  }

  // new copy, return NULL if malloc fails
  static char* new_buffer(const char* const buffer, size_t buffer_size) {
    char* b = new (std::nothrow) char[buffer_size];
    if (b == NULL) {
      std::cerr << "be_scan error: unable to allocate buffer resources\n";
      // malloc failed
      return NULL;
    }
    ::memcpy(b, buffer, buffer_size);
    return b;
  }

  // constructor
  be_scan_t::be_scan_t(const std::string& p_requested_scanners,
                       const char* const p_buffer,
                       size_t p_buffer_size) :
                buffer(new_buffer(p_buffer, p_buffer_size)),
                buffer_size(p_buffer_size),
                scanners(std::set<std::string>()),
                scanner_it(),
                opened_scanner(NULL),
                is_initialized(buffer != NULL) {

    if (!is_initialized) {
      // the buffer was not allocated so stay closed
      return;
    }

    // identify the requested scanners
    scanners = split(p_requested_scanners, ' ');

    // start the scanner iterator
    scanner_it = scanners.begin();

    // open the next scanner
    open_next();
  }

  void be_scan_t::open_next() {
    while (scanner_it != scanners.end()) {
      std::string scanner = *scanner_it++;
      if (scanner == "email") {
        opened_scanner = scan_email(buffer, buffer_size);
        break;
      } else {
        std::cerr << "be_scan_error: unrecognized scanner type '"
                  << scanner << "'\n";
      }
    }
  }

  void be_scan_t::close_opened() {
    if (opened_scanner != NULL) {
      delete opened_scanner;
      opened_scanner = NULL;
    }
  }

  artifact_t be_scan_t::next() {
    if (!is_initialized) {
      std::cerr << "be_scan error: not initialized, unable to allocate buffer resources\n";
      return artifact_t();
    }
    // return the next artifact, progressing through scanners as they finish
    while (opened_scanner != NULL) {
      artifact_t artifact = opened_scanner->next();
      if(artifact.artifact_class != "") {
        // this scanner has an artifact to return
        return artifact;
      } else {
        // this scanner has no artifact so move on to the next
        close_opened();
        open_next();
      }
    }

    // no more scanners so done
    return artifact_t();
  }

  // destructor
  be_scan_t::~be_scan_t() {
    // close scanner if open
    close_opened();

    // delete the allocated buffer
    delete[] buffer;
  }

  // loopback test
  void be_scan_t::test_loopback(std::string& s) {
    s = std::string(buffer, buffer_size);
  }

  // escape
  static std::string hexesc(unsigned char ch) {
    char buf[10];
    snprintf(buf,sizeof(buf),"\\x%02X",ch);
    return std::string(buf);
  }
  std::string escape(const std::string &in) {
    std::stringstream ss;
    for(std::string::const_iterator it = in.begin(); it != in.end(); it++) {
      unsigned char c = *it;

      if (c < ' ' || c > '~' || c == '\\') {
        // show as \xXX
        ss << hexesc(c);
      } else {
        // show ascii character
        ss << c;
      }
    }
    return ss.str();
  }
  std::string escape(const char* const p_buffer, size_t p_buffer_size) {
    return escape(std::string(p_buffer, p_buffer_size));
  }
}

