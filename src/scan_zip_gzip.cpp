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
#include <iostream>
#include <sstream>
#include <set>
#include "be_scan.hpp"
#include "scan_zip_gzip.hpp"

namespace be_scan {

  scan_zip_gzip_t::scan_zip_gzip_t(const char* const p_buffer,
                                   const size_t p_buffer_size) :
              buffer(reinterpret_cast<const unsigned char*>(p_buffer)),
              buffer_size(p_buffer_size),
              index(0) {
  }

  scan_zip_gzip_t::~scan_zip_gzip_t() {
  }

  artifact_t scan_zip_gzip_t::next() {

    // progress forward scanning for compression signatures
    for (; index<buffer_size; ++index) {

      // zip
      if (index + 30 < buffer_size &&
          buffer[index+0]==0x50 && buffer[index+1]==0x4B &&
          buffer[index+2]==0x03 && buffer[index+3]==0x04) {

        artifact_t artifact = uncompress_zip(buffer, buffer_size, index);
        if (artifact.artifact_class != "") {
          // we have an artifact
          index += 4;
          return artifact;
        }
      }

      // gzip
      if (index + 18 < buffer_size &&
          buffer[index+0]==0x1f && buffer[index+1]==0x8b &&
          buffer[index+2]==0x08 && (
          buffer[index+8]==0x00 || buffer[index+8]==0x02 || 
          buffer[index+8]==0x04)) {

        artifact_t artifact = uncompress_gzip(buffer, buffer_size, index);
        if (artifact.artifact_class != "") {
          // we have an artifact
          index += 3;
          return artifact;
        }
      }
    }

    // no more artifacts
    return artifact_t();
  }
}

extern "C"
be_scan::scanner_t* scan_zip_gzip(const char* const buffer,
                             const size_t buffer_size) {
  return new be_scan::scan_zip_gzip_t(buffer, buffer_size);
}

