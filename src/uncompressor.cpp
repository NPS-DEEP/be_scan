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

  const size_t max_uncompressed_size = 256*1024*1024;

  static unsigned char* allocate_scratch_buffer() {
    unsigned char* c = new (std::nothrow) unsigned char[max_uncompressed_size];
    return c;
  }

  // uncompressor_t
  uncompressor_t::uncompressor_t() :
                       scratch_buffer(allocate_scratch_buffer()),
                       bad_alloc(scratch_buffer == nullptr) {
  }

  // uncompress
  uncompressed_t uncompressor_t::uncompress(const char* const p_buffer,
                                            size_t buffer_size,
                                            size_t buffer_offset) {

    // use unsigned char in uncompressors
    const unsigned char* const buffer =
                      reinterpret_cast<const unsigned char*>(p_buffer);

    // scratch_buffer must be available
    if (scratch_buffer == nullptr) {
      return uncompressed_t(bad_alloc?
               "ERROR: uncompressor scratch buffer could not be allocated" :
               "ERROR: uncompressor scratch buffer has been closed");
    }

    // try uncompressors at buffer_offset until one works

    // zip
    if (buffer_offset + 30 < buffer_size &&
        buffer[buffer_offset+0]==0x50 && buffer[buffer_offset+1]==0x4B &&
        buffer[buffer_offset+2]==0x03 && buffer[buffer_offset+3]==0x04) {

      return uncompress_zip(scratch_buffer, max_uncompressed_size,
                            buffer, buffer_size, buffer_offset);
    }

    // gzip
    if (buffer_offset + 18 < buffer_size &&
        buffer[buffer_offset+0]==0x1f && buffer[buffer_offset+1]==0x8b &&
        buffer[buffer_offset+2]==0x08 && (
        buffer[buffer_offset+8]==0x00 || buffer[buffer_offset+8]==0x02 ||
        buffer[buffer_offset+8]==0x04)) {

      return uncompress_gzip(scratch_buffer, max_uncompressed_size,
                            buffer, buffer_size, buffer_offset);
    }

    // user error because the uncompressor signature was not found
    return uncompressed_t("ERROR: uncompressor signature not recognized");
  }

  // close
  void uncompressor_t::close() {
    if (scratch_buffer != nullptr) {
      delete[] scratch_buffer;
      scratch_buffer = nullptr;
    }
  }
}

