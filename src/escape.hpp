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
 * Header file for the be_scan library.
 */

#ifndef ESCAPE_HPP
#define ESCAPE_HPP

#include <string>
#include <stdint.h>

namespace be_scan {

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   input - The binary input string.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const std::string& input);

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   p_buffer - The buffer of bytes to format.
   *   p_buffer_size - The number of bytes in the buffer to format.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const char* const p_buffer, size_t p_buffer_size);
}

#endif

