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

struct CassSession; // Cassandra

namespace be_scan {

  /**
   * Version of the be_scan library.
   */
  const char* version();

  /**
   * The scanner class.
   * Although static and singleton, open() is required to connect
   * to the DB.
   */
  class be_scan_t {

    private:

    bool is_open;
    CassSession* cassandra_session;

    be_scan_t(const std::string& contact_point);

    write(const std::string& filename,
          const std::string& file_offset,
          const std::string& recursion_path,
          const std::string& artifact_class,
          const std::string& artifact);

    public:
    /**
     * Open be_scan connection.
     *
     * Parameters:
     *   contact_point - The Cassandra node to connect to.
     *
     * Returns:
     *   "" else error message if open failed.
     */
    std::string open(const std::string& contact_point);

    /**
     * Close be_scan connection.  Does nothing if not connected.
     */
    close();

    /**
     * Scan for email.
     *
     * Parameters:
     *   filename - The name of the file to attribute found artifacts to.
     *   file_offset - The offset into the file where the buffer starts.
     *   recursion_path - The recursive path to the artifact, or "" for
     *                    no recursion.
     *   buffer - The buffer of bytes to scan.
     *   buffer_size - The number of bytes in the buffer to scan.
     *
     * Returns:
     *   "" else error message if open failed.
     */
    std::string scan_email(const std::string& filename,
                           const std::string& file_offset,
                           const std::string& recursion_path,
                           const char* buffer,
                           size_t buffer_size);

    // NOTE: future: scan_zip(params) will exist.  Output may consist
    // of an iterator that dereferences to (params).  The caller will
    // be responsible for deallocating the returned buffer.
  };
}

#endif

