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

#ifdef HAVE_CASSANDRA

#include "cassandra.h"
#include "db.hpp"
#include <string>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <cassert>

/*
 * A DB implementation for storing to Cassandra.
 * https://github.com/datastax/cpp-driver/blob/master/examples/basic/basic.c
 */
namespace be_scan {

  // "" is good, text indicates error
  static std::string get_future_error(CassFuture* future) {
    const CassError rc = cass_future_error_code(future);
    std::string error;
    if (rc == CASS_OK) {
      error = "";
    } else {
      const char* message;
      size_t message_length;
      cass_future_error_message(future, &message, &message_length);
      error = std::string(message, message_length);
    }
    return error;
  }

  static CassCluster* new_cluster(const std::string& hosts) {
    // warn if hosts is empty
    if (hosts == "") {
      std::cerr << "cassandra_db new_cluster: Contact point required but none provided.";
    }

    // create cluster either way
    CassCluster* cluster = cass_cluster_new();
    cass_cluster_set_contact_points(cluster, hosts.c_str());
    return cluster;
  }

  static std::string connect(CassSession* session, const CassCluster* cluster) {
    CassFuture* future = cass_session_connect(session, cluster);
    cass_future_wait(future);
    const std::string error = get_future_error(future);
    cass_future_free(future);
    return error;
  }

  db_t::db_t(const std::string& contact_point) :
                  cluster(new_cluster(contact_point)),
                  session(cass_session_new()),
                  initialization_status(connect(session, cluster)),
                  is_open(initialization_status == ""){
  }

  // close
  db_t::~db_t() {
    // free cluster and session
    cass_cluster_free(cluster);
    cass_session_free(session);
  }

  // write
  // see A Simple Example, https://github.com/datastax/cpp-driver
  std::string db_t::write(const std::string& filename,
                          const uint64_t file_offset,
                          const std::string& recursion_path,
                          const std::string& artifact_class,
                          const std::string& artifact) {

    // must be open
    if (!is_open) {
      return ("cassandra_db write: Program error: Not open.");
    }

    // define the INSERT query
    std::stringstream ss;
    ss << "INSERT INTO " << artifact_class
       << " (artifact, filename, file_offset, recursion_path) "
       << "VALUES (?,?,?,?);";
    const std::string query = ss.str();

    // create the query statement
    CassStatement* const statement = cass_statement_new(query.c_str(), 4);
    cass_statement_bind_string(statement, 0, artifact.c_str());
    cass_statement_bind_string(statement, 1, filename.c_str());
    cass_statement_bind_int64(statement, 2, file_offset);
    cass_statement_bind_string(statement, 3, recursion_path.c_str());

    // run the query
    CassFuture* const future = cass_session_execute(session, statement);
    cass_future_wait(future);
    const std::string status = get_future_error(future);

    // free resources
    cass_future_free(future);
    cass_statement_free(statement);

    return status;
  }
} // end namespace

#endif

