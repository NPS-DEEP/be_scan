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
 * Generate context for artifact within buffer
 */

#ifndef SCAN_ZIP_HPP
#define SCAN_ZIP_HPP

#include <config.h>

#include <string>
#include <stdint.h>
#include "be_scan.hpp"
#include "scanner.hpp"
namespace be_scan {

class scan_zip_gzip_t : public scanner_t {

  private:
  const unsigned char* const buffer;
  const size_t buffer_size;
  size_t index;

  // do not allow copy or assignment
  scan_zip_gzip_t(const scan_zip_gzip_t&) = delete;
  scan_zip_gzip_t& operator=(const scan_zip_gzip_t&) = delete;

  public:
  static const std::string name;
  scan_zip_gzip_t(const char* const p_buffer,
             const size_t p_buffer_size);
  ~scan_zip_gzip_t();

  artifact_t next();
};

artifact_t uncompress_zip(const unsigned char* const in_buf,
                          const size_t in_size,
                          const size_t in_offset);

artifact_t uncompress_gzip(const unsigned char* const in_buf,
                           const size_t in_size,
                           const size_t in_offset);
}

extern "C"
be_scan::scanner_t* scan_zip_gzip(const char* const buffer,
                                  const size_t buffer_size);

#endif

