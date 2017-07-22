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
#include <queue>
#include "be_scan.hpp" // for artifact_t

namespace be_scan {

  class scanner_data_t {
    private:
    // do not allow copy or assignment
    scanner_data_t(const scanner_data_t&) = delete;
    scanner_data_t& operator=(const scanner_data_t&) = delete;

    public:
    std::queue<artifact_t> artifacts;
    std::string stream_name;
    uint64_t stream_offset;
    std::string recursion_prefix;
    const char* buffer;
    size_t buffer_size;

    // fields just for reading
    const char* previous_buffer;
    size_t previous_buffer_size;

    scanner_data_t() :
               artifacts(),
               stream_name(""), stream_offset(0), recursion_prefix(""),
               buffer(nullptr), buffer_size(0),
               previous_buffer(nullptr), previous_buffer_size(0) {
    }
  };

} // namespace
#endif

