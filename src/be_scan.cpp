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
#include "be_scan.hpp"
#include "scanners.hpp"
#include "db.hpp"

namespace be_scan {

  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

  // available scanners
  std::string available_scanners() {
    return "email"; // zz "email exif ..."
  }

  // constructor
  be_scan_t::be_scan_t(const std::string& settings) :
              db(new db_t(settings)),
              initialization_status(db->initialization_status) {
  }

  // destructor
  be_scan_t::~be_scan_t() {
    delete db;
  }

  // scan
  std::string be_scan_t::scan(const std::string& filename,
                              const size_t file_offset,
                              const std::string& recursion_path,
                              const char* const buffer,
                              size_t buffer_size) {

    // return error if bad
    if (initialization_status != "") {
      return initialization_status;
    }

    // conditionally call each scanner, for now just call scan_email
    return scan_email(filename, file_offset, recursion_path,
                      buffer, buffer_size, db);
  }
}

