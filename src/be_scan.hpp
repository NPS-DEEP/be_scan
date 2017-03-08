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

  class db_t;

  /**
   * Version of the be_scan library.
   */
  const char* version();

  /**
   * The scanner class.
   */
  class be_scan_t {

    private:
    db_t* db;

    public:
    /**
     * A connection to the database.
     */
    be_scan_t();

    /**
     * Open a connection to the database.
     *
     * Parameters:
     *   connection - The DB connection setting.  For example for
     *                Cassandra, this is the contact point.
     *
     * Returns:
     *   "" else error message if open failed.
     */
    std::string open(const std::string& connection);

    /**
     * Close be_scan connection.  Does nothing if not connected.
     */
    void close();

    /**
     * Scanner list.
     *
     * Returns:
     *   List of available scanners
     */
    std::string available_scaners();

    /**
     * Scan.
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
     *   "" else error message if scan failed.
     */
    std::string scan(const std::string& filename,
                     const std::string& file_offset,
                     const std::string& recursion_path,
                     const char* buffer,
                     size_t buffer_size);

  };
}

#endif

