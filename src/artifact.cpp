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
    artifact_t::artifact_t(const std::string& p_artifact_class,
                           const size_t p_buffer_offset,
                           const std::string& p_artifact,
                           const std::string& p_context,
                           const char* const p_new_buffer,
                           const size_t p_new_buffer_size,
                           const bool p_bad_alloc) :
                      artifact_class(p_artifact_class),
                      buffer_offset(p_buffer_offset),
                      artifact(p_artifact),
                      context(p_context),
                      new_buffer(p_new_buffer),
                      new_buffer_size(p_new_buffer_size),
                      bad_alloc(p_bad_alloc) {
    }

    // blank
    artifact_t::artifact_t() :
                      artifact_class(""),
                      buffer_offset(0),
                      artifact(""),
                      context(""),
                      new_buffer(NULL),
                      new_buffer_size(0),
                      bad_alloc(false) {
    }

    bool artifact_t::has_new_data() const {
      return (new_buffer != NULL);
    }

    void artifact_t::delete_new_buffer() {
      if(new_buffer != NULL) {
        delete[] new_buffer;
        new_buffer = NULL;
        new_buffer_size = 0;
      }
    }

    // get around Java \0 limitation, see be_scan.i
    void artifact_t::javaArtifact(std::string& a) const {
      a = artifact;
    }

    // get around Java \0 limitation, see be_scan.i
    void artifact_t::javaContext(std::string& c) const {
      c = context;
    }
}

