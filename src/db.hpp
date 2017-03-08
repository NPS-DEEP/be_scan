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

#ifndef CASSANDRA_DB_HPP
#define CASSANDRA_DB_HPP

#include <config.h>
#include "be_scan.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <climits>
#ifndef HAVE_CXX11
#include <cassert>
#endif

/*
 * A DB implementation for storing to Cassandra.
 * See A Simple Example, https://github.com/datastax/cpp-driver.
 */
namespace be_scan {

class db_t {

  private:
  bool is_open;
#ifdef HAVE_CASSANDRA
  CassSession* session;
#endif

  public:
  db_t();

  // open
  std::string open(const std::string& contact_point);

  // close
  std::string close();

  // write
  std::string be_scan_t::write(const std::string& filename,
                               const uint64_t file_offset,
                               const std::string& recursion_path,
                               const std::string& artifact_class,
                               const std::string& artifact);
};
}

#endif

