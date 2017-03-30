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

#ifndef FLEX_SCAN_PARAMETERS_HPP
#define FLEX_SCAN_PARAMETERS_HPP

#include <stdint.h>
#include <string>
#include <iostream>

// ************************************************************
// Define be_scan::flex_buffer_reader_t for reading from char[]
// instead of from FILE*.
// ************************************************************
namespace be_scan {

  /* Glue so flex can read from a byte buffer. */
  class flex_scan_parameters_t {

    public:
    const std::string& filename;
    const size_t file_offset;
    const std::string& recursion_path;

    // buffer
    const char* const buffer;
    const size_t buffer_size;

//    private:
//    // buffer pointer
    // flex increments this as it scans along
    size_t buffer_index;

    public:
//    // flex increments this as it scans along
//    size_t artifact_index;

    flex_scan_parameters_t(const std::string& p_filename,
                           const size_t p_file_offset,
                           const std::string& p_recursion_path,
                           const char* const p_buffer,
                           const size_t p_buffer_size) :
              filename(p_filename),
              file_offset(p_file_offset),
              recursion_path(p_recursion_path),
              buffer(p_buffer),
              buffer_size(p_buffer_size),
              buffer_index(0) {
//zzstd::cout << "'" << buffer << "', buffer_size: " << buffer_size << "\n";
    }

/*
    // flex uses get_input to read bytes from the buffer
    size_t get_input(char* buf, size_t max_size) {
      // this should not happen
      if((int)max_size < 0) {
        return 0;
      }

      // provide up to n bytes
      // adapted from Lex and Yacc 2'nd ed. p. 157
      const int n = (buffer_size - buffer_index < max_size)
                              ? buffer_size - buffer_index : max_size;
std::cerr << "n: " << n << " buffer_size: " << buffer_size << ", buffer_index: " << buffer_index << ", max_size: " << max_size << "\n";
      if (n > 0) {
        ::memcpy(buf, buffer + buffer_index, n);
        buffer_index += n;
      }
      return n;
    }
*/

    // Offer context field used only in the bulk_extractor feature file,
    // to be discontinued.
    // Note that offset can be calculated bytut length is in yyleng and is
    // not visible.
    std::string current_context(size_t length) {
      const size_t offset = buffer_index;

      const size_t start = offset < 16 ? 0 : offset - 16;
      const size_t stop = offset + length + 16 > buffer_size
                        ? buffer_size : offset + length + 16;
      const std::string context(&buffer[start], stop - start);
//zzstd::cout << "current_context: length: " << length << ", start: " << start << ", stop: " << stop << "\n";
      return context;
    }
  };
} // end namespace

#endif

