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

#ifndef ARTIFACT_HPP
#define ARTIFACT_HPP

#include <string>
#include <stdint.h>

namespace be_scan {

  /**
   * Artifacts are returned using this structure.
   */
// We intentionally have a pointer, so suppress the compiler warning.
#ifdef HAVE_DIAGNOSTIC_EFFCPP
#pragma GCC diagnostic ignored "-Weffc++"
#endif
  class artifact_t {
    public:
    std::string media_filename;
    uint64_t slice_offset;
    std::string recursion_prefix;
    std::string artifact_class;
    size_t buffer_offset;
    std::string artifact;
    std::string context;
    
    artifact_t();
    artifact_t(const std::string& p_media_filename,
               const uint64_t p_slice_offset,
               const std::string& p_recursion_prefix,
               const std::string& p_artifact_class,
               const size_t p_buffer_offset,
               const std::string& p_artifact,
               const std::string& p_context);

    void artifact_t::to_avro();
    void artifact_t::to_stdout();
  };
}

#endif

