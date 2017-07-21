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
#include "read_random.hpp"

namespace be_scan {

  /*
   * Read bytes from buffer, possibly reading from previous_buffer.
   * All requested bytes are included unless reading would cross outside
   * the bounds of the two buffers provided.
   */
  std::string read_buffer(const size_t buffer_offset,
                          const char* const previous_buffer,
                          const size_t previous_buffer_size,
                          const char* const buffer,
                          const size_t buffer_size,
                          const size_t offset,
                          const size_t length,
                          const size_t padding) {

    // fail on invalid input
    if (buffer_size == 0 || offset >= buffer_size) {
      std::cerr << "internal error in extract_content, buffer_size: "
                << buffer_size << ", offset: " << offset << "\n";
      assert(0);
    }
    if (previous_buffer_size > buffer_offset) {
      std::cerr << "internal error in extract_content, previous_buffer_size: "
                << previous_buffer_size << ", offset: " << offset << "\n";
      assert(0);
    }

    // start offset of the previous buffer
    const size_t previous_buffer_offset = buffer_offset - previous_buffer_size;

    // start offset of the padded artifact, no underflow
    const size_t start_offset = (offset < prevous_buffer_offset + padding)
                                ? previous_buffer_offset : offset - padding;

    // end offset of the padded artifact, no overflow
    const size_t end_offset =
                 (offset + length + padding > buffer_offset + buffer_size) ?
                  buffer_offset + buffer_size : offset + length + padding;

    // build the return string
    std::stringstream ss;

    // add any part from previous_buffer
    if (start_offset < buffer_offset) {
      const size_t end_offset1 = (offset + length + padding >= start_offset
                       ? start_offset - 1 : offset + length + padding;

      ss << std::string(&prevous_buffer[start_offset - previous_buffer_offset],
                        end_offset1 - start_offset));
    }

    // add any part from buffer
    if (end_offset >= buffer_offset) {

      const size_t start_offset2 = (start_offset >= buffer_offset)
                                 ? buffer_offset : start_offset;
      ss << std::string(&buffer[start_offset2 - buffer_offset],
                        end_offset - start_offset2);
    }

    return ss.str();
  }
}

