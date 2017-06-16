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

namespace lw {
  class lw_t;
  class lw_scanner_t;
}

namespace be_scan {

  class scanner_data_t;

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
   * The scan engine class.
   */
  class scan_engine_t {

    // scanner_t accesses lw_t in scan_engine_t
    friend class scanner_t;

    private:
    lw::lw_t* lightgrep_wrapper;

#ifndef SWIG
    // do not allow copy or assignment
    scan_engine_t(const scan_engine_t&) = delete;
    scan_engine_t& operator=(const scan_engine_t&) = delete;
#endif

    public:

    /**
     * Status is "" if valid, else error message.
     */
    const std::string status;

    /**
     * Get a regex scan engine.
     *
     * Parameters:
     *   requested_scanners - The scanners to enable.
     */
    scan_engine_t(const std::string& requested_scanners);

    /**
     * Destructor.
     */
    ~scan_engine_t();
  };

  /**
   * The scanner class.
   */
  class scanner_t {
    private:
    scanner_data_t* scanner_data;
    lw::lw_scanner_t* lw_scanner;
    const std::string status;

#ifndef SWIG
    // do not allow copy or assignment
    scanner_t(const scanner_t&) = delete;
    scanner_t& operator=(const scanner_t&) = delete;
#endif

    public:

    /**
     * Get a regex scanner.
     *
     * Parameters:
     *   scan_engine - The scan engine to use for the scanning
     *   avro_output_filename - The filename to write found artifacts to.
     */
    scanner_t(scan_engine_t& scan_engine,
              const std::string& avro_output_filename);

    /**
     * Scan.
     *
     * Parameters:
     *   stream_name - The name where the buffer stream came from,
     *                 typically a media image filename.
     *   stream_offset - The stream start offset of this buffer.
     *   recursion_prefix - The recursion prefix, or "" for no recursion.
     *   buffer - The stream's buffer to scan.
     *   buffer_size - The number of bytes in the buffer to scan.
     * Returns:
     *   "" else failure message.
     */
    std::string scan(const std::string& stream_name,
                     const uint64_t stream_offset,
                     const std::string& recursion_prefix,
                     const char* const buffer,
                     size_t buffer_size);

    /**
     * Destructor.
     */
    ~scanner_t();
  };
}

#endif

