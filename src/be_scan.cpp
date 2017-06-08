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

const char* be_scan_version() {
  return PACKAGE_VERSION;
}

namespace be_scan {

// ************************************************************
// general support
// ************************************************************
  // Version of the be_scan library.
  const char* version() {
    return PACKAGE_VERSION;
  }

// ************************************************************
// scan engine
// ************************************************************

  scan_engine_t::scan_engine_t(const std::string& requested_scanners) :
                lightgrep_wrapper(new lw::lw_t),
                status(add_regex(*lightgrep_wrapper, requested_scanners)) {
    if (status == "") {
      lightgrep_wrapper->finalize_regex();
    }
  }

  scan_engine_t::~scan_engine_t() {
    delete lightgrep_wrapper;
  }

// ************************************************************
// scanner
// ************************************************************

  static const size_t max_backtrack_size = 1000;

  static std::string open_avro(const std::string& scan_engine_status,
                               const std::string& avro_filename) {
    if (scan_engine_status != "") {
      return "Scan engine not initialized: " + scan_engine_status;
    }

    // set up avro
    // zz TBD
  }

  scanner_t::scanner_t(scan_engine_t& scan_engine,
                       const std::string& avro_filename) :
             scanner_data(new scanner_data_t),
             lw_scanner_t(scan_engine.lightgrep_wrapper->new_lw_scanner(
                          scanner_data, max_backtrack_size)),
             status(open_avro(scan_engine->status, avro_filename)) {
  }

  scanner_t::scan(const std::string& stream_name,
                  const uint64_t stream_offset,
                  const std::string& recursion_prefix,
                  const char* const buffer,
                  size_t buffer_size) {

    // set up scanner_data fields for this scan
    scanner_data->stream_name = stream_name;
    scanner_data->stream_offset = stream_offset;
    scanner_data->recursion_prefix = recursion_prefix;

    // perform the scan
    scan_engine->scan(buffer, buffer_size);
    scan_engine->scan_finalize();
  }

  scanner_t::~scanner_t() {
    delete scanner_data;
    // do not delete lw_scanner because lw_t does this.
  }
}

