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
#include <stdint.h>
#include <string>
#include "be_scan.hpp"

namespace be_scan {
    artifact_t::artifact_t(const std::string& p_media_filename,
                           const uint64_t p_slice_offset,
                           const std::string& p_recursion_prefix,
                           const std::string& p_artifact_class,
                           const size_t p_buffer_offset,
                           const std::string& p_artifact,
                           const std::string& p_context) :
                      media_filename(p_media_filename),
                      slice_offset(p_slice_offset),
                      recursion_prefix(p_recursion_prefix),
                      artifact_class(p_artifact_class),
                      buffer_offset(p_buffer_offset),
                      artifact(p_artifact),
                      context(p_context) {
    }

    // blank
    artifact_t::artifact_t() :
                      media_filename(""),
                      slice_offset(0),
                      recursion_prefix(""),
                      artifact_class(""),
                      buffer_offset(0),
                      artifact(""),
                      context("") {
    }

    // forward artifact to Avro
    void artifact_t::to_avro() {
      // TBD
      std::cout << "to_avro TBD\n";
    }

    // forward artifact to stdout
    void artifact_t::to_stdout() {
      std::stringstream;
      ss << media_filename << " "
         << artifact_class << " "
         << slice_offset + buffer_offset << "\t"
         << escape(artifact) << "\t"
         << escape(context) << "\n";
      std::cout << ss.str();
    }
}

