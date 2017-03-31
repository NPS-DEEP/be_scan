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

namespace be_scan {

  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

  // available scanners
  std::string available_scanners() {
    return "email"; // zz "email exif ..."
  }

  // new copy, return NULL if malloc fails
  static char* new_buffer(const char* const buffer, size_t buffer_size) {
    char* b = new (std::nothrow) char[buffer_size];
    if (b == NULL) {
      // malloc failed
      return NULL;
    }
    ::memcpy(b, buffer, buffer_size);
    return b;
  }

  // constructor
  be_scan_t::be_scan_t(const std::string& p_selected_scanners,
                       const char* const p_buffer,
                       size_t p_buffer_size) :
                selected_scanners(p_selected_scanners),
                buffer(new_buffer(p_buffer, p_buffer_size)),
                buffer_size(p_buffer_size),
                scan_email(0),
                is_initialized(buffer != NULL) {

    // start the email scanner
    scan_email = new scan_email_t(buffer, buffer_size);
  }

  artifact_t be_scan_t::next_artifact() {
    if (!is_initialized) {
      return artifact_t("","","");
    }
    return scan_email->next();
  }

  // destructor
  be_scan_t::~be_scan_t() {
    // delete scanner
    delete scan_email;

    // delete buffer
    delete[] buffer;
  }
}

