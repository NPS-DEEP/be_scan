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
 * Generate context for artifact within buffer
 */

#ifndef SCAN_EMAIL_HPP
#define SCAN_EMAIL_HPP

#include <config.h>
#include <string>
#include <stdint.h>
#include "artifact.hpp"
#include "lightgrep_wrapper.hpp"
#include "user_data.hpp"
#include "read.hpp"
//#include "be_scan.hpp"
//#include "scanner.hpp"
//#include "extract_context.hpp"

namespace be_scan {

void email_callback_function(const uint64_t start,
                             const uint64_t size,
                             void* p_user_data);

void add_email_regex(lw::lw_t& lw);

} // namespace

#endif

