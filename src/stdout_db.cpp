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

#ifdef HAVE_STDOUT_DB

#include "db.hpp"
#include <string>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <cassert>

static std::string hexesc(char ch)
{
    char buf[10];
    snprintf(buf,sizeof(buf),"\\x%02X",ch);
    return std::string(buf);
}

static std::string escape(const std::string& input) {
  std::stringstream ss;
  for (const char& c : input) {
    if (c < ' ' || c > '~' || c == '\\') {
      ss << hexesc(c);
    } else {
      ss << c;
    }
  }
  return ss.str();
}

/*
 * A diagnostic configuration that writes to stdout instead of to a DB.
 */
namespace be_scan {

  db_t::db_t(const std::string& settings) :
                  initialization_status(""),
                  is_open(true) {
    std::cout << "stdout_db db_t settings: "
              << settings << "\n";
  }

  // close
  db_t::~db_t() {
    // no action
  }

  // write
  std::string db_t::write(const std::string& filename,
                          const uint64_t file_offset,
                          const std::string& recursion_path,
                          const std::string& artifact_class,
                          const std::string& artifact,
                          const std::string& context) {

/*
    std::cout << "stdout_db db_t write: filename: '" << filename
              << "', file_offset: " << file_offset
              << ", recursion_path: '" << recursion_path
              << "', artifact_class: " << artifact_class
              << ", artifact: '" << escape(artifact)
              << ", context: '" << escape(context)
              << "'\n";
*/

    std::cout << "bulk_extractor " << artifact_class
              << " " << filename << " "
              << recursion_path << file_offset // feature path
              << "\t" << escape(artifact)      // feature
              << "\t" << escape(context)       // context
              << "\n";
    return "";
  }
} // end namespace

#endif

