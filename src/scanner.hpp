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
 * Manages scanning through multiple scanners as though there were one
 * scanner.  Add new scanners here.
 */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <stdint.h>
#include <queue>
#include <sstream>
#include <cassert>
#include "scan_email.hpp"

// the names of all available scanners, separated by spaces
static const char* scanner_names = "email";

namespace be_scan {

  // Scanner types
  enum scan_state_t{
          SCAN_BEGIN,
          SCAN_EMAIL,
          SCAN_DONE
  };

  // Manage available scanners as one scanner
  class scanner_t {
    private:
    // buffer
    const char* const buffer;
    const size_t buffer_size;

    // scanner state
    scan_state_t scan_state;
    std::queue<scan_state_t> scanners;

    // scanners
    scan_email_t* scan_email;

    // helper
    void parse_scanners(const std::string& p_names) {

      // add spaces for easy parsing
      std::stringstream ss;
      ss << " " << p_names << " ";
      std::string names = ss.str();

      // add requested scanners
      if (names.find(" email ") != std::string::npos) {
        scanners.push(SCAN_EMAIL);
      }
    }

    // get the next available scanner else SCAN_DONE
    scan_state_t pop_next_scanner() {
      if (scanners.empty()) {
        return SCAN_DONE;
      } else {
        scan_state_t state = scanners.front();
        scanners.pop();
        return state;
      }
    }

    // open this scanner
    void open(scan_state_t scanner) {
      switch (scanner) {
        case SCAN_EMAIL: {
          scan_email = new scan_email_t(buffer, buffer_size);
          break;
        }

        case SCAN_DONE: {
          std::cerr << "be_scan error: no scanner to open\n";
          break;
        }

        default:
          assert(0);
      }
    }

    // close this scanner
    void close (scan_state_t scanner) {
      switch (scanner) {
        case SCAN_EMAIL: {
          delete scan_email;
          break;
        }

        case SCAN_DONE: {
          std::cerr << "be_scan error: no scanner to close\n";
        }

        default:
          assert(0);
      }
    }
 
    public:

    // constructor
    scanner_t(const std::string& names,
              const char* const p_buffer,
              size_t p_buffer_size) :
                         buffer(p_buffer),
                         buffer_size(p_buffer_size), 
                         scan_state(SCAN_BEGIN),
                         scanners() {

      // parse the scanenr names into scanners[]
      parse_scanners(names);

      // get the next scanner
      scan_state = pop_next_scanner();

      // open this scanner
      open(scan_state);
    }

    // next
    artifact_t next() {
      artifact_t artifact;
      while (true) {
        switch (scan_state) {
          case SCAN_EMAIL: {
            artifact = scan_email->next();
            break;
          }

          case SCAN_DONE: {
            return artifact_t();
          }

          default:
            assert(0);
        }

        // return if this scanner is providing an artifact
        if (artifact.artifact_class != "") {
          return artifact;
        }

        // this scanner is used up
        close(scan_state);

        // get the next scanner
        scan_state = pop_next_scanner();

        // open the next scanner
        open(scan_state);
      }
    }

    static std::string available_scanners() {
      return std::string(scanner_names);
    }
  };
}

#endif

