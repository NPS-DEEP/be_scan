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
#include "be_scan.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <climits>
#ifndef HAVE_CXX11
#include <cassert>
#endif

namespace be_scan {

  // constructor
  be_scan_t::be_scan_t(const std::string& p_contact_point) :
                         is_open(false),
                         contact_point(p_contact_point) {
  }

  // open
  // see A Simple Example, https://github.com/datastax/cpp-driver
  std::string be_scan_t::open(const std::string& contact_point) {
    CassFuture* connect_future = NULL;
    CassCluster* cluster = cass_cluster_new();
    session = cass_session_new();

    // add contact point
    cass_cluster_set_contact_points(cluster, contact_point);

    // connect 
    connect_future = cass_session_connect(session, cluster);


    if (cass_future_error_code(connect_future) == CASS_OK) {
      CassFuture* close_future = NULL;
      is_open = true;
      return "";

    } else {
      /* Handle error */
      const char* message;
      size_t message_length;
      cass_future_error_message(connect_future, &message, &message_length);
      std::string error = std::string(message, message_length);
      return error;
    }
  }

  // close
  // see A Simple Example, https://github.com/datastax/cpp-driver
  std::string be_scan_t::close() {
    if (is_open) {
      CassFuture* close_future = cass_session_close(session);
      cass_future_wait(close_future);
      cass_future_free(close_future);
      is_open = false;
      session = NULL;
    }
  }

  // write
  // see A Simple Example, https://github.com/datastax/cpp-driver
  std::string be_scan_t::write(const std::string& filename,
                               const uint64_t file_offset,
                               const std::string& recursion_path,
                               const std::string& artifact_class,
                               const std::string& artifact) {

    // must be open
    if (!is_open) {
      return ("Program error: Not open.");
    }

    // status
    std::string success = "";

    // insert
    // refer to insert_into_basic in 
    // https://github.com/datastax/cpp-driver/blob/1.0/examples/basic/basic.c
    CassError rc = CASS_OK;
    CassStatement* statement = NULL;
    CassFuture* future = NULL;
    CassString query = cassString_init("INSERT INTO " + artifact_class +
                   " (artifact, filename, file_offset, recursion_path);");
    statement = cass_statement_new(query, 4);
    cass_statement_bind_string(statement, 0, cass_string_init(artifact));
    cass_statement_bind_string(statement, 1, cass_string_init(filename));
    cass_statement_bind_int64(statement, 2, file_offset);
    cass_statement_bind_string(statement, 3, cass_string_init(recursion_path));

    future = cass_session_execute(session, statement);
    cass_future_wait(future);

    rc = cass_future_error_code(future);
    if (rc != CASS_OK) {
      CassString message = cass_future_error_message(future);
      success = std::string((int)message.length, message.data);
    }
    cass_future_free(future);
    cass_future_free(statement);

    return success;
  }
}

