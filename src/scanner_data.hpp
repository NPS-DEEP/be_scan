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

/**
 * \file
 * Header file for the be_scan library.
 */

#ifndef SCANNER_DATA_T
#define SCANNER_DATA_T

#include <string>
#include <stdint.h>

namespace be_scan {

  class scanner_data_t {
    public:
    const std::string stream_filename;
    const uint64_t stream_offset;
    const std::string recursion_prefix;
    char* const buffer;
    size_t buffer_size;
    const std::string avro_output_filename;
  }:

} // namespace
#endif

