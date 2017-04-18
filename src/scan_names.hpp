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

#ifndef SCAN_NAMES_HPP
#define SCAN_NAMES_HPP

#include <config.h>

#ifdef HAVE_DEVEL

#include <string>
#include <stdint.h>
#include <set>
#include "be_scan.hpp"
#include "scanner.hpp"
#include "artifact_context.hpp"
namespace be_scan {

class scan_names_t : public scanner_t {

  private:
  const char* const buffer;
  const size_t buffer_size;
  size_t index;
  size_t flex_start;
  size_t flex_stop;

  size_t find_start(const size_t at);
  size_t find_stop(const size_t at);
  size_t find_start16(const size_t at);
  size_t find_stop16(const size_t at);

  // do not allow copy or assignment
  scan_names_t(const scan_names_t&) = delete;
  scan_names_t& operator=(const scan_names_t&) = delete;

  public:
  static const std::string name;
  scan_names_t(const char* const p_buffer,
               const size_t p_buffer_size);
  ~scan_names_t();

  artifact_t next();
};

}

extern "C"
be_scan::scanner_t* scan_names(const char* const buffer,
                               const size_t buffer_size);


#endif // HAVE_DEVEL

#endif

