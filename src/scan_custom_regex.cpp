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
#include <iostream>
#include <sstream>
#include <cstring>
#include <set>
#include "scanners.hpp"
#include "scanner_data.hpp"
#include "lightgrep_wrapper.hpp"

/**
 * \file
 * Scan for your own custom regex patterns using this scanner.
 */

namespace be_scan {
  // patterns
  std::set<std::string> custom_regex_patterns;

  // add custom regex patterns
  void add_custom_regex_pattern(const std::string& pattern) {
    custom_regex_patterns.insert(pattern);
  }

  // clear custom regex patterns
  void clear_custom_regex_patterns() {
    custom_regex_patterns.clear();
  }
}

namespace custom_regex {


  // ************************************************************
  // callback functions
  // ************************************************************

  void custom_regexHitHandler(const uint64_t start, const uint64_t size,
                       void* p_scanner_data) {

    // typecast void* into scanner_data
    be_scan::scanner_data_t* scanner_data(
                   static_cast<be_scan::scanner_data_t*>(p_scanner_data));

    // artifact
    std::string artifact = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size, 0);

    // context
    std::string context = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size, 16);

    scanner_data->artifacts->put(be_scan::artifact_t(
                   "custom_regex",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }

  // ************************************************************
  // add custom_regex
  // ************************************************************

  std::string add_custom_regex(lw::lw_scanner_program_t& lw_scanner_program) {
    std::string status;
    for (auto it = be_scan::custom_regex_patterns.begin(); it !=
                       be_scan::custom_regex_patterns.end(); ++it) {
      status = lw_scanner_program.add_regex(
                    *it, "UTF-8", true, false, &custom_regexHitHandler);
      if (status != "") return status;
      status = lw_scanner_program.add_regex(
                    *it, "UTF-16LE", true, false, &custom_regexHitHandler);
      if (status != "") return status;
    }
    return "";
  }
}

