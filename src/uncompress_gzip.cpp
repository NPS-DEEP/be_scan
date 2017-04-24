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

// Based on GZIP file format specification v4.3,
// http://www.zlib.org/rfc-gzip.html

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

  // return artifact, which may be blank
  artifact_t uncompress_gzip(unsigned char* const scratch_buf,
                             const size_t scratch_buf_size,
                             const unsigned char* const in_buf,
                             const size_t in_size,
                             const size_t in_offset) {

    // validate the buffer range
    if (in_size < in_offset + 18) {
      // nothing to do
      return artifact_t();
    }

    // calculate maximum input size
    const size_t max_in_size = in_size - in_offset;

    // set up the decompression structure
    z_stream zs;
    memset (&zs, 0, sizeof(zs));

    zs.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(
                                                        in_buf + in_offset));
    zs.avail_in = max_in_size;
    zs.next_out = scratch_buf;
    zs.avail_out = scratch_buf_size;

    int r = inflateInit2(&zs,16+MAX_WBITS);
    if(r==0){
      // ignore any error code and accept total_out
      r = inflate(&zs,Z_SYNC_FLUSH);
      const size_t out_size = zs.total_out;
      r = inflateEnd(&zs);

      // compose the artifact field
      std::stringstream ss;
      ss << "size=" << out_size;

      // no data
      if (out_size == 0) {
        return artifact_t("gzip", in_offset, ss.str(), "", NULL, 0, false);
      }

      // data
      unsigned char* out_buf = new (std::nothrow) unsigned char[out_size]();
      if (out_buf == NULL) {
        // bad_alloc
        return artifact_t("gzip", in_offset, ss.str(),
                          "bad_alloc", NULL, 0, true);
      } else {
        ::memcpy(out_buf, scratch_buf, out_size);
        return artifact_t("gzip", in_offset, ss.str(), "",
                          reinterpret_cast<const char*>(out_buf),
                          out_size, false);

      }
    } else {

      // inflate failed
      return artifact_t();
    }
  }
}

