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
#include "cassandra.h"

#include <string>
#include <stdint.h>

/*
 * A DB implementation for storing to Cassandra.
 * https://github.com/datastax/cpp-driver/blob/master/examples/basic/basic.c
 */
namespace be_scan {

  class db_t {

    private:
#ifdef HAVE_CASSANDRA
    CassCluster* const cluster;
    CassSession* const session;
#endif

    public:
    const std::string initialization_status;
    const bool is_open;

    db_t(const std::string& contact_point);
    ~db_t();

  // write
    std::string write(const std::string& filename,
                      const size_t file_offset,
                      const std::string& recursion_path,
                      const std::string& artifact_class,
                      const std::string& artifact,
                      const std::string& context);
  };
}

#endif

