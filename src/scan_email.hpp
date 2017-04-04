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

#ifndef SCAN_EMAIL_HPP
#define SCAN_EMAIL_HPP

#include <config.h>
#include <string>
#include <stdint.h>
#include <set>
#include "be_scan.hpp"
#include "artifact_context.hpp"
#include "flex_extra_parameters.hpp"
namespace be_scan {

class scan_email_t {

  private:
  const char* const buffer;
  const size_t buffer_size;
  size_t index;
  flex_extra_parameters_t flex_extra_parameters;

  const static std::set<std::string> domain_names;

  size_t find_start(const size_t at);
  size_t find_stop(const size_t at);
  size_t find_start16(const size_t at);
  size_t find_stop16(const size_t at);
  bool valid_top_level_domain(const std::string& feature);

  // implemented in flex email scanner
  void flex_init();
  void flex_scan(const std::string& potential_feature);
  void flex_close();

  public:
  scan_email_t(const char* const p_buffer,
               const size_t p_buffer_size);
  ~scan_email_t();

  artifact_t next();
};

}

#endif

