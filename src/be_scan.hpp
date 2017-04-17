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

/**
 * Version of the be_scan library.
 */
const char* be_scan_version();

namespace be_scan {

  class scanner_t;

  /**
   * Artifacts are returned using this structure.
   */
  class artifact_t {
    public:
    std::string artifact_class;
    size_t buffer_offset;
    std::string artifact;
    std::string context;
    artifact_t();
    artifact_t(const std::string& p_artifact_class,
               const size_t p_buffer_offset,
               const std::string& p_artifact,
               const std::string& p_context);
    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaArtifact(std::string& java_artifact);

    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaContext(std::string& java_context);
  };

  /**
   * Version of the be_scan library.
   */
  const char* version();

  /**
   * The complete list of available scanners.
   *
   * Returns:
   *   List of available scanners separated by space charcters.
   */
  std::string available_scanners();

  /**
   * The scanner class.
   */
  class be_scan_t {

    private:
    const char* const buffer;
    const size_t buffer_size;
    scanner_t* scanner;

#ifndef SWIG
    // do not allow copy or assignment
    be_scan_t(const be_scan_t&) = delete;
    be_scan_t& operator=(const be_scan_t&) = delete;
#endif

    public:
    /**
     * False if C++ failed to allocate memory for the buffer.
     */
    const bool is_initialized;

    /**
     * Create a scan instance given scanners to use and a buffer to scan.
     *
     * Parameters:
     *   selected_scanners - The scanners to use during the scan.
     *   p_buffer - The buffer of bytes to scan.
     *   p_buffer_size - The number of bytes in the buffer to scan.
     */
    be_scan_t(const std::string& selected_scanners,
              const char* const p_buffer,
              size_t p_buffer_size);

    /*
     * Scan to the next artifact.  If no artifact is available, return a
     * blank artifact containing "" and 0 values for all fields.
     *
     * Returns:
     *   artifact_t - The artifact or an artifact with blank data if done.
     */
    artifact_t next();

    /**
     * Destructor.
     */
    ~be_scan_t();

    /**
     * For diagnostics only, test loopback of input buffer.
     */
    void test_loopback(std::string& loopback_buffer);
  };

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   in - The binary input string.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const std::string& in);

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   buffer - The buffer of bytes to format.
   *   buffer_size - The number of bytes in the buffer to format.
   *   selected_scanners - The scanners to use during the scan.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const char* const p_buffer, size_t p_buffer_size);
}

#endif

