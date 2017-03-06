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

#ifndef FLEX_COMMON_HPP
#define FLEX_COMMON_HPP

#include <stdint.h>

// ************************************************************
// Define be_scan::flex_buffer_reader_t for reading from char[]
// instead of from FILE*.
// ************************************************************
namespace be_scan {

  /* Glue so flex can read from a byte buffer. */
  class flex_buffer_reader_t {

    private:
    const char* const buffer;
    const size_t buffer_size;
    size_t buffer_index;

    public:
    flex_buffer_reader_t(const char* const p_buffer,
                         const size_t p_buffer_size) :
              buffer(p_buffer),
              buffer_size(p_buffer_size),
              buffer_index(0) {
    }

    /* flex uses get_input to read bytes from the buffer. */
    size_t get_input(char* buf, size_t max_size) {
      // this should not happen
      if((int)max_size < 0) {
        return 0;
      }

      // provide up to n bytes
      // adapted from Lex and Yacc 2'nd ed. p. 157
      int n = (buffer_size - buffer_index < max_size) ? buffer_size : max_size;
      if (n > 0) {
        ::memcpy(buf, buffer + buffer_index, n);
        buffer_index += n;
      }
    }
  }

// ************************************************************
// Ease compiler warnings
// ************************************************************
/*
zz leave warnings alone?
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wmissing-noreturn"

#ifdef HAVE_DIAGNOSTIC_EFFCPP
#pragma GCC diagnostic ignored "-Weffc++"
#endif

#ifdef HAVE_DIAGNOSTIC_DEPRECATED_REGISTER
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#endif
*/

// ************************************************************
// Define FLEX macros
// ************************************************************
#define YY_NO_INPUT
#define ECHO {}                   /* Never echo anything */
#define YY_SKIP_YYWRAP            /* Never wrap */
#define YY_NO_INPUT
#define YY_INPUT(buf,result,max_size) result = get_extra(yyscanner)->get_input(buf,max_size);

// note that scanner-specific macros are also defined in scan_*.flex.

} // end namespace




#endif

