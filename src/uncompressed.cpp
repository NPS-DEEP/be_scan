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
#include <stdexcept>
#include "be_scan.hpp"       // for uncompressor_t, uncompressed_t
#include "uncompressors.hpp" // for the various uncompressor templates

namespace be_scan {

  // The uncompressed string can be really big so return "" on malloc failure
  static std::string failsafe_buffer(const unsigned char* const buf,
                                     size_t size) {
    // try to create the output string
    std::string out;
    try {
      out = std::string(reinterpret_cast<const char*>(buf), size);
    } catch (std::bad_alloc& e) {
      out = "";
    }
    return out;
  }

  // nothing found so nothing uncompressed
  uncompressed_t::uncompressed_t() :
                       buffer(""), status("") {
  }

  // something failed earlier
  uncompressed_t::uncompressed_t(const std::string& failure) :
                       buffer(""), status(failure) {
  }

  // good, something uncompressed
  uncompressed_t::uncompressed_t(const unsigned char* const scratch_buf,
                                 size_t out_size) :
    buffer(failsafe_buffer(scratch_buf, out_size)),
    status((buffer.size() == out_size) ? "" : "buffer malloc failure") {
  }

  // javaBuffer
  void uncompressed_t::javaBuffer(std::string& a) const {
    a = buffer;
  }
}

