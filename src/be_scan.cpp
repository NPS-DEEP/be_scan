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
#include <sstream>
#include <cstring>
#include <set>
#include <iostream>
#include "be_scan.hpp"
#include "scan_email.hpp"
#include "scan_zip_gzip.hpp"
#ifdef HAVE_DEVEL
#include "scan_names.hpp"
#endif

const char* be_scan_version() {
  return PACKAGE_VERSION;
}

// ************************************************************
// private support functions
// ************************************************************
// split()
// adapted from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
static std::set<std::string> split(const std::string &s, char delim) {
  std::set<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) {
    elems.insert(item);
  }
  return elems;
}

namespace be_scan {

// ************************************************************
// general support
// ************************************************************
  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

  // loopback test
  void be_scan_t::test_loopback(std::string& s) const {
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

// ************************************************************
// scanner
// ************************************************************
  // available scanners
  std::string available_scanners() {
#ifdef HAVE_DEVEL
    return "email zip_gzip names";
#else
    return "email zip_gzip";
#endif
  }

  // constructor
  be_scan_t::be_scan_t(const std::string& p_requested_scanners,
                       const std::string& p_avro_filename) :
                scanners(split(p_requested_scanners, ' '),
                avro_filename(p_avro_filename) {
  }

  std::string be_scan_t::scan(const char* const buffer,
                              size_t buffer_size) {

    // run each requested scanner
    for (auto it = scanners.begin(); it != scanners.end(); ++it) {

      if (scanner == "email") {
        opened_scanner = scan_email(buffer, buffer_size);
        break;
#ifdef HAVE_DEVEL
      } else if (scanner == "names") {
        opened_scanner = scan_names(buffer, buffer_size);
        break;
#endif
      } else if (scanner == "zip_gzip") {
        opened_scanner = scan_zip_gzip(buffer, buffer_size);
        break;
      } else {
        std::cerr << "be_scan_error: unrecognized scanner type '"
                  << scanner << "'\n";
      }






  be_scan_t::be_scan_t(const std::string& p_requested_scanners,
                       const artifact_t& artifact) :
                buffer(copy_buffer(artifact.new_buffer,
                                   artifact.new_buffer_size)),
                buffer_size(artifact.new_buffer_size),
                scanners(std::set<std::string>()),
                scanner_it(),
                opened_scanner(NULL),
                bad_alloc(buffer == NULL) {

    if (bad_alloc) {
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
#ifdef HAVE_DEVEL
      } else if (scanner == "names") {
        opened_scanner = scan_names(buffer, buffer_size);
        break;
#endif
      } else if (scanner == "zip_gzip") {
        opened_scanner = scan_zip_gzip(buffer, buffer_size);
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
    if (bad_alloc) {
      // return a bad_alloc artifact
      std::cerr << "be_scan error: not initialized, unable to allocate buffer resources\n";
      return artifact_t("", 0, "", "", NULL, 0, true);
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
    if (buffer != NULL) {
      delete[] buffer;
    }
  }
}

