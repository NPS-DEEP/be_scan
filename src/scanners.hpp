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
 * Header file for all scanners.  Update this when adding scanners.
 */

#ifndef SCANNERS_HPP
#define SCANNERS_HPP

#include <config.h>
#include <string>
#include <stdint.h>
#include "lightgrep_wrapper.hpp"

// scanner support
namespace be_scan {

  // available scanners
  std::string available_scanners();

  // add regex for all scanners, return "" else error
  std::string add_regex(lw::lw_t& lw, const std::string& requested_scanners);
}

class be_scan::p_scanner_data;

// email
namespace email {
  // callback functions
  void emailHitHandler(const uint64_t start, const uint64_t size,
                       void* p_scanner_data);
  void emailUTF16LEHitHandler(const uint64_t start, const uint64_t size,
                              void* p_scanner_data);
  // regex
  std::string add_email_regex(lw::lw_t& lw);
}

#endif

