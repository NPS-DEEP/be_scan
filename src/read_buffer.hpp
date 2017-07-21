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
 * Read bytes from buffer, possibly reading from previous_buffer.
 * All requested bytes are included unless reading would cross outside
 * the bounds of the two buffers provided.
 */

#ifndef READ_BUFFER_HPP
#define READ_BUFFER_HPP

#include <string>
#include <stdint.h>

namespace be_scan {

  std::string read_buffer(const size_t buffer_offset,
                          const char* const previous_buffer,
                          const size_t previous_buffer_size,
                          const char* const buffer,
                          const size_t buffer_size,
                          const size_t offset,
                          const size_t length,
                          const size_t padding);
}

#endif

