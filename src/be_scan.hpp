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

#ifndef BE_SCAN_HPP
#define BE_SCAN_HPP

#include <string>
#include <stdint.h>

namespace be_scan {

  class scan_email_t;

//  /**
//   * Data to be scanned is contained within this structure.
//   */
//  class be_buffer_t {
//    public:
//    const char* const buffer;
//    const size_t buffer_size;
//    be_buffer_t(const char* const p_buffer, const size_t p_buffer_size);
//    ~be_buffer_t();
//  };

  /**
   * Artifacts are returned using this structure.
   */
  class artifact_t {
    public:
    const std::string artifact_class;
    const std::string artifact;
    const std::string context;
    artifact_t(const std::string& p_artifact_class,
               const std::string& p_artifact,
               const std::string& p_context);
  };

  /**
   * Version of the be_scan library.
   */
  const char* version();

  /**
   * Get a list of available scanners.
   *
   * Returns:
   *   List of available scanners separated by a space chaacter.
   */
  std::string available_scanners();

  /**
   * The scanner class.
   */
  class be_scan_t {

    private:
    const std::string selected_scanners;
    const char* const buffer;
    const size_t buffer_size;
    scan_email_t* scan_email;

    public:
    /**
     * False if the scanner failed to allocate memory for the buffer.
     */
    const bool is_initialized;

    /**
     * Create a scan instance given scanners to use and a buffer to scan.
     *
     * Parameters:
     *   p_selected_scanners - The scanners to use during the scan.
     *   p_buffer - The buffer of bytes to scan.
     *   p_buffer_size - The number of bytes in the buffer to scan.
     */
    be_scan_t(const std::string& p_selected_scanners,
              const char* const p_buffer,
              size_t p_buffer_size);

    /*
     * Scan to the next artifact.  If no artifact is available, return a
     * blank artifact containing "" and 0 values for all fields.
     *
     * Returns:
     *   artifact_t - The artifact or an artifact with blank data if done.
     */
    artifact_t next_artifact();

    /**
     * Destructor.
     */
    ~be_scan_t();

  };
}

#endif

