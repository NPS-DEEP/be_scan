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
 * Update this file when adding new scanners:
 *    - Add scanner to the available_scanners list.
 *    - Add regex for your scanner in the add_regexes function.
 */

#include <config.h>
#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include "scanners.hpp"

// split()
// adapted from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
static std::set<std::string> split(const std::string &s, char delim) {
  std::set<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) {
    elems.insert(item);
  }
  return elems;
}

namespace be_scan {

  // available scanners
  std::string available_scanners() {
    return "email uncompression custom_regex";
  }

  // add regex for all requested scanners
  std::string add_regexes(lw::lw_scanner_program_t& lw_scanner_program,
                          const std::string& requested_scanners) {
    std::string status;
    std::set<std::string> scanners = split(requested_scanners, ' ');
    if (scanners.size() == 0) {
      return "No scanners requested.";
    }
    for (auto it=scanners.begin(); it != scanners.end(); ++it) {

      // email
      if (*it == "email") {
        status = email::add_email_regex(lw_scanner_program);
        if (status != "") {
          return status;
        }

      // uncompression
      } else if (*it == "uncompression") {
        status = uncompression::add_uncompression_regex(lw_scanner_program);
        if (status != "") {
          return status;
        }

      // custom_regex
      } else if (*it == "custom_regex") {
        status = custom_regex::add_custom_regex(lw_scanner_program);
        if (status != "") {
          return status;
        }

      // not valid
      } else {
        // scanner name not identified
        return "Scanner '" + *it + "' not identified.";
      }
    }
    return "";
  }
}

