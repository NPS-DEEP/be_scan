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

#ifdef HAVE_DEVEL

#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include "be_scan.hpp"
#include "scan_names.hpp"
#include "extract_context.hpp"

namespace be_scan {

  // from https://en.wikipedia.org/wiki/Email_address
  // Use RFC 5322 except do not recognize backslash or quoted string.
  // Specifically: local part <= 64 characters, domain <= 255 characters and
  // not space or "(),:;<>@[\]
  const static size_t MAX_LOCAL_PART_SIZE = 129;
  const static size_t MAX_DOMAIN_SIZE = 138;
  inline bool not_local_part(const unsigned char c) {
    return (c<0x20 || c >0x7f || c=='\"' || c=='(' || c==')' || c==',' ||
            c==':' || c==';' || c=='<' || c=='>' || c=='@' || c=='[' ||
            c=='\\' || c==']');
  }
  inline bool not_domain(const unsigned char c) {
    return (c<0x20 || c >0x7f || c=='\"' || c=='(' || c==')' || c==',' ||
            c==':' || c==';' || c=='<' || c=='>' || c=='@' || c=='[' ||
            c=='\\' || c==']');
  }

  // find optimistic start of local part
  size_t scan_names_t::find_start(const size_t at) {
    size_t start = at;
    while (true) {
      // done if at beginning
      if (start == 0) {
        return start;
      }

      // done if adjacent to previous end
      if (flex_stop != 0 && start == flex_stop + 1) {
        return start;
      }

      // done if local part is too long
      if (at - start > MAX_LOCAL_PART_SIZE) {
        return start;
      }

      // done if next char is invalid
      unsigned char c = buffer[start-1];
      if (not_local_part(c)) {
        return start;
      }

      // char is valid so step backwards to it
      --start;
    }
  }

  // find optimistic end of domain
  size_t scan_names_t::find_stop(const size_t at) {
    size_t stop = at;
    while (true) {
      // done if at EOF
      if (stop + 1 == buffer_size) {
        return stop;
      }

      // invalid if domain is too long
      if (stop - at > MAX_DOMAIN_SIZE) {
        return at;
      }

      // done if next char is invalid
      unsigned char c = buffer[stop+1];
      if (not_domain(c)) {
        return stop;
      }

      // char is valid so step forward to it
      ++stop;
    }
  }

  // find optimistic start of local part pair
  size_t scan_names_t::find_start16(const size_t at) {
    size_t start = at;
    while (true) {
      // done if at beginning, specifically, start is at 0 or 1
      if (start < 2) {
        return start;
      }

      // done if adjacent to previous end, specifically, at stop+1 or stop+2
      if (flex_stop != 0 && start <= flex_stop + 2) {
        return start;
      }

      // done if local part is too long
      if (at - start > MAX_LOCAL_PART_SIZE * 2) {
        return start;
      }

      // done if next char pair is invalid
      if ((buffer[start-1] != '\0') || not_local_part(buffer[start-2])) {
        return start;
      }

      // char pair is valid so step backwards to it
      start -= 2;
    }
  }

  // find optimistic end of domain pair, where second byte of pair is at end + 1
  size_t scan_names_t::find_stop16(const size_t at) {
    size_t stop = at;
    while (true) {
      // done if byte from pair plus next pair is EOF
      if (stop +3 >= buffer_size) {
        return stop;
      }

      // done if domain is too long
      if (stop - at > MAX_DOMAIN_SIZE * 2) {
        return stop;
      }

      // done if next char pair is invalid
      if ((buffer[stop+3] != '\0') || not_local_part(buffer[stop+2])) {
        return stop;
      }

      // char pair is valid so step forward to it
      stop += 2;
    }
  }

  //  public:
  scan_names_t::scan_names_t(const char* const p_buffer,
                             const size_t p_buffer_size) :
                  buffer(p_buffer),
                  buffer_size(p_buffer_size),
                  index(0),
                  flex_start(0),
                  flex_stop(0) {
  }

  scan_names_t::~scan_names_t() {
  }

  artifact_t scan_names_t::next() {
    // progress forward to artifact or to near end of buffer
    for (; index+2<buffer_size; ++index) {
      if (buffer[index] == '@') {
        if (buffer[index+1] != '\0') {
          // unicode 8
          const size_t start = find_start(index);
          if (start+1 > index) {   // require at least one byte
            continue;
          }
          const size_t stop = find_stop(index);
          if (stop < index+4) {    // require at least four bytes
            continue;
          }

          // build name from this
          const std::string feature = std::string(&buffer[start], stop-start+1);

          // validate simple
//          if (!valid_top_level_domain(feature)) {
//            continue;
//          }

/*
          // validate regex
          flex_scan(feature); // sets flex_extra_parameters
          if (flex_extra_parameters.flex_size == 0) {
            // not valid in flex
            continue;
          }

          // define start and stop based on flex
          flex_start = start + flex_extra_parameters.flex_offset;
          flex_stop = flex_start + flex_extra_parameters.flex_size - 1;
*/
          flex_start = start;
          flex_stop = stop;

          // advance past the artifact
          index = stop + 1;

          // accept the artifact
          size_t size = flex_stop - flex_start + 1;
          return artifact_t("names", flex_start,
                 std::string(&buffer[flex_start], size),
                 extract_context(buffer, buffer_size, flex_start, size, 16),
                 NULL, 0, false);

        } else {
          // unicode 16
          const size_t start = find_start16(index);
//std::cout << "next16.a start: " << start << "\n";
          if (start+2 > index) {   // require at least one pair
            continue;
          }
          const size_t stop = find_stop16(index);
//std::cout << "next16.b stop: " << stop << "\n";
          if (stop < index+8) {    // require at least four pairs
            continue;
          }

//std::cout << "next16.c\n";
/*
          // generate returned unicode 16 feature
          // zz NOTE: in future, return unicode 8 feature
          const std::string feature16 = std::string(&buffer[start],
                                                    stop-start+2);
*/


          // build name from this
          const std::string feature = std::string(&buffer[start], stop-start+2);

          // validate simple
//          if (!valid_top_level_domain(feature)) {
//            continue;
//          }

/*
          // validate regex
          flex_scan(feature); // sets flex_extra_parameters
          if (flex_extra_parameters.flex_size == 0) {
            // not valid in flex
            continue;
          }

          // define start and stop based on flex
          flex_start = start + flex_extra_parameters.flex_offset;
          flex_stop = flex_start + flex_extra_parameters.flex_size - 1;
*/
          flex_start = start;
          flex_stop = stop + 2;

          // advance past the artifact
          index = stop + 1;

          // accept the artifact
          size_t size = flex_stop - flex_start + 1;
          return artifact_t("names", flex_start,
                 std::string(&buffer[flex_start], size),
                 extract_context(buffer, buffer_size, flex_start, size, 16),
                 NULL, 0, false);

/*
          // build unicode 8 from this
          std::stringstream ss;
          for (size_t i=start; i <= stop; i+=2) {
std::cout << "next16.buffer[" << i << "]: '" << buffer[i] << "'\n";
            ss << buffer[i];
          }

          // get the feature as unicode 8
          const std::string feature8 = ss.str();

std::cout << "next16.d '" << escape(feature8) << "'\n";
          // validate simple
//          if (!valid_top_level_domain(feature8)) {
//            continue;
//          }

          // validate regex
          flex_scan(feature8); // sets flex_extra_parameters
          if (flex_extra_parameters.flex_size == 0) {
            // not valid in flex
            continue;
          }

          // define start and stop based on flex
          flex_start = start + flex_extra_parameters.flex_offset * 2;
          flex_stop = flex_start + (flex_extra_parameters.flex_size - 1) * 2;

          // advance past the artifact
          index = stop + 1;

          // accept the artifact
          size_t size = flex_stop - flex_start + 2;
std::cout << "next16.e '" << escape(std::string(&buffer[flex_start], size)) << "\n";
          return artifact_t("names", flex_start,
                 std::string(&buffer[flex_start], size),
                 extract_context(buffer, buffer_size, flex_start, size, 16),
                 NULL, 0, false);
*/
        }
      }
    }
    // no more artifacts for this buffer
    return artifact_t();
  }
}

extern "C"
be_scan::scanner_t* scan_names(const char* const buffer,
                               const size_t buffer_size) {
  return new be_scan::scan_names_t(buffer, buffer_size);
}

#endif // HAVE_DEVEL


