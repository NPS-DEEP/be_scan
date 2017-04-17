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
#include "scanner.hpp"

const char* be_scan_version() {
  return PACKAGE_VERSION;
}

namespace be_scan {

  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

  // available scanners
  std::string available_scanners() {
    return scanner_t::available_scanners();
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
  be_scan_t::be_scan_t(const std::string& selected_scanners,
                       const char* const p_buffer,
                       size_t p_buffer_size) :
                buffer(new_buffer(p_buffer, p_buffer_size)),
                buffer_size(p_buffer_size),
                scanner(0),
                is_initialized(buffer != NULL) {

    // open the recursive scanner
    scanner = new scanner_t(selected_scanners, buffer, buffer_size);
  }

  artifact_t be_scan_t::next() {
    if (!is_initialized) {
      std::cerr << "be_scan error: not initialized, unable to allocate buffer resources\n";
      return artifact_t();
    }
    return scanner->next();
  }

  // destructor
  be_scan_t::~be_scan_t() {
    // delete scanner
    delete scanner;

    // delete buffer
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

