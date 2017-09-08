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
 * Add your uncompressor template to this set of uncompressors and also
 * call it from uncompressor.cpp.
 */

#ifndef UNCOMPRESSORS_HPP
#define UNCOMPRESSORS_HPP

#include <config.h>

#include <string>
#include <stdint.h>

namespace be_scan {

uncompressed_t uncompress_zip(const unsigned char* const in_buf,
                              const size_t in_size,
                              const size_t in_offset,
                              unsigned char* const scratch_buf,
                              const size_t scratch_buf_size);

uncompressed_t uncompress_gzip(const unsigned char* const in_buf,
                               const size_t in_size,
                               const size_t in_offset,
                               unsigned char* const scratch_buf,
                               const size_t scratch_buf_size);
}

#endif

