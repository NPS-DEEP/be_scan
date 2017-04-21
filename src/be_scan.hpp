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
#include <set>

/**
 * Version of the be_scan library.
 */
const char* be_scan_version();

namespace be_scan {

  class scanner_t;

  /**
   * Artifacts are returned using this structure.
   *
   * Some artifacts can include decompressed data which you must delete.
   * bad_alloc is true if there was not enough memory to fully retrieve
   * the artifact.
   */
// We intentionally have a pointer, so suppress the compiler warning.
#ifdef HAVE_DIAGNOSTIC_EFFCPP
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif
  class artifact_t {
    public:
    std::string artifact_class;
    size_t buffer_offset;
    std::string artifact;
    std::string context;
    const char* new_buffer;
    size_t new_buffer_size;
    bool bad_alloc;
    
    artifact_t();
    artifact_t(const std::string& p_artifact_class,
               const size_t p_buffer_offset,
               const std::string& p_artifact,
               const std::string& p_context,
               const char* const p_new_buffer,
               const size_t p_new_buffer_size,
               const bool p_bad_alloc);
    /**
     * Returns true if this artifact includes decompressed data that
     * can be scanned.  It is your responsibility to delete this new
     * buffer else you will have a memory leak.
     */
    bool has_new_buffer() const;

    /**
     * Artifacts from decompression scanners include a buffer containing
     * new decompressed data.  It is your responsibility to delete this
     * new data else you will have a memory leak.
     */
    void delete_new_buffer();

    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaArtifact(std::string& java_artifact) const;

    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaContext(std::string& java_context) const;

  };
#ifdef HAVE_DIAGNOSTIC_EFFCPP
#pragma GCC diagnostic pop
#endif

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
    std::set<std::string> scanners;
    std::set<std::string>::const_iterator scanner_it;
    scanner_t* opened_scanner;
    void open_next();
    void close_opened();

#ifndef SWIG
    // do not allow copy or assignment
    be_scan_t(const be_scan_t&) = delete;
    be_scan_t& operator=(const be_scan_t&) = delete;
#endif

    public:
    /**
     * False if C++ failed to allocate memory for the buffer.
     */
    const bool bad_alloc;

    /**
     * Create a scan instance given scanners to use and a buffer to scan.
     *
     * Parameters:
     *   requested_scanners - The scanners to use during the scan.
     *   p_buffer - The buffer of bytes to scan.  The buffer is copied.
     *   p_buffer_size - The number of bytes in the buffer to scan.
     */
    be_scan_t(const std::string& requested_scanners,
              const char* const p_buffer,
              size_t p_buffer_size);

    /**
     * Create a scan instance given scanners to use and an artifact
     * containing a buffer to scan.
     *
     * Parameters:
     *   requested_scanners - The scanners to use during the scan.
     *   artifact - The artifact containing the buffer of bytes to scan.
     *              The buffer is copied.
     *   p_buffer_size - The number of bytes in the buffer to scan.
     */
    be_scan_t(const std::string& requested_scanners,
              const artifact_t& artifact);

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
    void test_loopback(std::string& loopback_buffer) const;
  };

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   input - The binary input string.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const std::string& input);

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   p_buffer - The buffer of bytes to format.
   *   p_buffer_size - The number of bytes in the buffer to format.
   * Returns:
   *   Escaped output.
   */
  std::string escape(const char* const p_buffer, size_t p_buffer_size);

}

#endif

