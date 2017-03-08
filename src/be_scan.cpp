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

#ifdef HAVE_CASSANDRA
#include "be_cassandra.hpp" // for db_t implementation
#else
#error DB driver not selected
#endif


namespace be_scan {

  /**
   * Version of the be_scan library.
   */
  const char* version() {
    return PACKAGE_VERSION;
  }

  be_scan_t::be_scan_t() : db(0) {
  }

  std::string be_scan_t::open(const std::string& connection) {
    db = new db_t();
    return db->open(connection);
  }

  void be_scan_t::close() {
    db->close();
  }

  std::string available_scanners() {
    return "TBD";
  }

  std::string scan(const std::string& filename,
                     const std::string& file_offset,
                     const std::string& recursion_path,
                     const char* buffer,
                     size_t buffer_size) {


}

