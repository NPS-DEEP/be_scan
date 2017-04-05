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
 * Used by diagnostics to escape unprintable string characters.
 */

#ifndef BE_SCAN_HPP
#define BE_SCAN_HPP

#include <string>
#include <sstream>
#include <stdint.h>

namespace be_scan {

  static std::string hexesc(char ch) {
    char buf[10];
    snprintf(buf,sizeof(buf),"\\x%02X",ch);
    return std::string(buf);
  }

  inline std::string escape(const std::string &in) {
    std::stringstream ss;
    for(std::string::const_iterator it = in.begin(); it != in.end(); it++) {
      char c = *it;

      if (c < ' ' || c > '~' || c == '\\') {
        // show as \xXX
        ss << hexesc(c);
      } else {
        // show ascii character
        ss << c;
      }
    }
    return ss.str();
  }
}

#endif

