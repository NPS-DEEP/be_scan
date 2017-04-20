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
#include <stdint.h>
#include <iostream>
#include "extract_context.hpp"

namespace be_scan {

  /*
   * Return the context for the point within the buffer
   */
  std::string extract_context(const char* const buffer,
                               const size_t buffer_size,
                               const size_t artifact_offset,
                               const size_t artifact_length,
                               const size_t artifact_padding) {

    // handle empty input
    if (buffer_size == 0) {
      return "";
    }

    // find valid bounds for the context
    const size_t start = artifact_offset < artifact_padding ?
                            0 : artifact_offset - artifact_padding;
    const size_t stop = artifact_offset + artifact_length + artifact_padding >=
                    buffer_size ? buffer_size - 1 :
                    artifact_offset + artifact_length + artifact_padding - 1;

    // return the context
    return std::string(&buffer[start], stop - start + 1);
  }
}

