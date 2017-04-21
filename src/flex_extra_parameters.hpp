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

#ifndef FLEX_EXTRA_PARAMETERS_HPP
#define FLEX_EXTRA_PARAMETERS_HPP

#include <stdint.h>
#include <string>
#include <iostream>

typedef void* yyscan_t; // flex

namespace be_scan {

  class flex_extra_parameters_t {

    public:
    yyscan_t scanner;
    size_t flex_offset;
    size_t flex_size;
    size_t region_start;
    size_t region_stop;
  };
} // end namespace

#endif

