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
#include <stdint.h>
#include <string>
#include "scanner_data.hpp"
#include "be_scan.hpp"       // for escape()

namespace be_scan {

  // path may be a simple offset or a recursion path, see Recursion Path
  // in hashdb_um.
  static std::string path(const scanner_data_t& scanner_data,
                          const size_t buffer_offset) {

    stringstream ss;
    if (scanner_data.recursion_prefix == "") {
      // simple offset
      ss << scanner_data.stream_offset + buffer_offset;
    } else {
      // recursion path
      ss << scanner_data.recursion_prefix << "-" << buffer_offset;
    }
    return ss.str();
  }


  // write artifact to Avro
  void write_avro(const scanner_data_t& scanner_data,
                  const std::string& artifact_class,
                  const size_t buffer_offset,
                  const std::string& artifact,
                  const std::string& context) {
  // zz TBD
  }

  // write artifact to stdout, for diagnostics only
  void write_stdout(const scanner_data_t& scanner_data,
                    const std::string& artifact_class,
                    const size_t buffer_offset,
                    const std::string& artifact,
                    const std::string& context) {

    // prepare artifact similar to bulk_extractor feature,
    // but with steam filename and artifact class in front.
    std::stringstream;
    ss << scanner_data.stream_filename << " "
       << artifact_class << " "
       << path(scanner_data, buffer_offset)
       << escape(artifact) << "\t"
       << escape(context) << "\n";
    std::cout << ss.str();
  }
}

