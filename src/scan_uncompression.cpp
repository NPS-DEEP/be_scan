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

namespace uncompression {

  // ************************************************************
  // general support
  // ************************************************************

  //
  // subpatterns
  //

  // none

  //
  // patterns
  //
  const std::string ZIP("\\z50\\z4B\\z03\\z04");
  const std::string GZIP("\\z1F\\z8B\\z08.{5}[\\z00\\z02\\z04]");

  //
  // helper functions
  //

  // none

  // ************************************************************
  // callback functions
  // ************************************************************

  void zipHandler(const uint64_t start, const uint64_t size,
                  void* p_scanner_data) {

    // typecast void* into scanner_data
    be_scan::scanner_data_t* scanner_data(
                   static_cast<be_scan::scanner_data_t*>(p_scanner_data));

    // artifact
    std::string artifact = "zip artifact";

    // context
    std::string context = "zip context";

    scanner_data->artifacts.put(be_scan::artifact_t(
                   "zip",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }

  void gzipHandler(const uint64_t start, const uint64_t size,
                   void* p_scanner_data) {

    // typecast void* into scanner_data
    be_scan::scanner_data_t* scanner_data(
                   static_cast<be_scan::scanner_data_t*>(p_scanner_data));

    // artifact
    std::string artifact = "gzip artifact";

    // context
    std::string context = "gzip context";

    scanner_data->artifacts.put(be_scan::artifact_t(
                   "gzip",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }


  // ************************************************************
  // regex
  // ************************************************************

  std::string add_uncompression_regex(
                   lw::lw_scanner_program_t& lw_scanner_program) {
    std::string status;
    status = lw_scanner_program.add_regex(
                   ZIP, "UTF-8", false, false, &zipHandler);
    if (status != "") return status;
    status = lw_scanner_program.add_regex(
                   GZIP, "UTF-8", false, false, &gzipHandler);
    if (status != "") return status;
    return "";
  }
}

