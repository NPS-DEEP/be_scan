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
#include <stdint.h>
#include <iostream>
#include <cassert>
#include "read.hpp"

namespace be_scan {

  /*
   * Read bytes from buffer, including optional padding, into a string.
   */
  std::string read_random(const char* const buffer,
                          const size_t buffer_size,
                          const size_t offset,
                          const size_t length,
                          const size_t padding) {

    // fail on invalid input
    if (buffer_size == 0 || offset >= buffer_size) {
      std::cerr << "internal error in extract_content\n";
      assert(0);
    }

    // find valid bounds for the context
    const size_t start = offset < padding ? 0 : offset - padding;
    const size_t stop = offset + length + padding >= buffer_size ?
                        buffer_size - 1 : offset + length + padding - 1;

    // return the bytes as string
    return std::string(&buffer[start], stop - start + 1);
  }
}

