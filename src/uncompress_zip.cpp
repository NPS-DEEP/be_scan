// Author:  Bruce Allen
// Created: 2/25/2013
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
// Released into the public domain on February 25, 2013 by Bruce Allen.

// Code adapted from bulk_extractor recursion scanners.

// NOTE: inline methods do not check for buffer overflow.  Do not call
//       them when near the end of data.

/**
 * \file
 * Provide uncompress routines for selected compression algorithms.
 */
#include <config.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <zlib.h>
#include "be_scan.hpp"

namespace be_scan {

  static const uint32_t zip_name_len_max = 1024;
  static const size_t uncompressed_size_min = 6;

  inline uint16_t u16(const unsigned char* const b) {
    return (uint16_t)(b[0]<<0) | (uint16_t)(b[1]<<8);
  }

  inline uint32_t u32(const unsigned char* const b) {
    return (uint32_t)(b[0]<<0) | (uint32_t)(b[1]<<8) |
           (uint32_t)(b[2]<<16) | (uint32_t)(b[3]<<24);
  }

  // return artifact, which may be blank
  artifact_t uncompress_zip(unsigned char* const scratch_buf,
                            const size_t scratch_buf_size,
                            const unsigned char* const in_buf,
                            const size_t in_size,
                            const size_t in_offset) {

    // validate the buffer range
    if (in_size < in_offset + 30) {
      // nothing to do
      return artifact_t();
    }

    const unsigned char* const b = in_buf + in_offset;

    const uint32_t compr_size=u32(b+18);
    const uint32_t uncompr_size=u32(b+22);
    const uint16_t name_len=u16(b+26);
    const uint16_t extra_field_len=u16(b+28);

    // validate name length
    if (name_len == 0 || name_len > zip_name_len_max) {
      // invalid zip metadata, nothing to do
      return artifact_t();
    }

    // calculate offset to compressed data
    uint32_t compressed_offset = in_offset + 30 + name_len + extra_field_len;

    // offset must be inside the buffer
    if (compressed_offset >= in_size) {
      // zip read request outside data range, nothing to do
      return artifact_t();
    }

    // size of compressed data
    const uint32_t compressed_size = (compr_size == 0 ||
               compressed_offset + compr_size > in_size) 
                          ? in_size - compressed_offset : compr_size;

    // size of uncompressed data
    const uint32_t potential_uncompressed_size =
               (compr_size == 0 || compr_size > scratch_buf_size)
                                  ? scratch_buf_size : uncompr_size;
    
    // skip if uncompressed size is too small
    if (potential_uncompressed_size < uncompressed_size_min) {
      // zip uncompress size too small, disregard it
      return artifact_t();
    }

    // set up zlib data
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    zs.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(
                                         in_buf + compressed_offset));
    zs.avail_in = compressed_size;
    zs.next_out = scratch_buf;
    zs.avail_out = potential_uncompressed_size;

    // initialize zlib for this decompression
    int r = inflateInit2(&zs, -15);
    if (r == 0) {

      // inflate
      inflate(&zs, Z_SYNC_FLUSH);

      // set out_size
      const size_t out_size = zs.total_out;

      // close zlib
      inflateEnd(&zs);

      // compose the artifact field
      std::stringstream ss;
      ss << "size=" << out_size;

      // no data
      if (out_size == 0) {
        return artifact_t("zip", in_offset, ss.str(), "", NULL, 0, false);
      }

      // data
      unsigned char* out_buf = new (std::nothrow) unsigned char[out_size]();
      if (out_buf == NULL) {
        // bad_alloc
        return artifact_t("zip", in_offset, ss.str(),
                          "bad_alloc", NULL, 0, true);
      } else {
        ::memcpy(out_buf, scratch_buf, out_size);
        return artifact_t("zip", in_offset, ss.str(), "",
                          reinterpret_cast<const char*>(out_buf),
                          out_size, false);
      }
    } else {

      // inflate failed
      return artifact_t();
    }
  }
}

