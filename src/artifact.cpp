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
#include <sstream>
#include "be_scan.hpp"

namespace be_scan {

    artifact_t::artifact_t(const std::string& p_artifact_class,
                           const std::string& p_stream_name,
                           const std::string& p_recursion_prefix,
                           const size_t p_offset,
                           const std::string& p_artifact,
                           const std::string& p_context) :
                      artifact_class(p_artifact_class),
                      stream_name(p_stream_name),
                      recursion_prefix(p_recursion_prefix),
                      offset(p_offset),
                      artifact(p_artifact),
                      context(p_context) {
    }

    // blank
    artifact_t::artifact_t() :
                      artifact_class(""),
                      stream_name(""),
                      recursion_prefix(""),
                      offset(0),
                      artifact(""),
                      context("") {
    }

    // return as formatted escaped text
    std::string artifact_t::to_string() const {
      std::stringstream ss;
      ss << stream_name << " "
         << artifact_class << " ";
      if (recursion_prefix == "") {
        ss << offset;
      } else {
        ss << recursion_prefix << "-" << offset;
      }
      ss << "\t" << escape(artifact) << "\t" << escape(context);
      return ss.str();
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

