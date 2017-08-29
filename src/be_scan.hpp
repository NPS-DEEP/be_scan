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
   *   List of available scanners separated by space characters.
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
   * The artifact class.  Artifacts are returned using this structure.
   */
  class artifact_t {

    public:
    std::string artifact_class;
    std::string stream_name;
    std::string recursion_prefix;
#ifdef SWIG // make SWIG provide long instead of BigInteger for Java
    size_t offset;
#else
    uint64_t offset;
#endif
    std::string artifact;
    std::string context;
    
    artifact_t();
    artifact_t(const std::string& p_artifact_class,
               const std::string& p_stream_name,
               const std::string& p_recursion_prefix,
               const size_t p_offset,
               const std::string& p_artifact,
               const std::string& p_context);

    /**
     * Return the artifact as formatted escaped text, see artifact.cpp:
     * <stream filename><space><artifact class><space><path><tab>
     * <escaped artifact><tab><escaped context>
     */
    std::string to_string() const;

    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaArtifact(std::string& java_artifact) const;

    /**
     * For Java, return as byte[] which can include \0.
     */
    void javaContext(std::string& java_context) const;
  };

  /**
   * The scanner class.
   */
  class scanner_t {
    private:
    scanner_data_t* scanner_data;
    lw::lw_scanner_t* lw_scanner;

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
     */
    scanner_t(scan_engine_t& scan_engine);

    /**
     * Set up the scanner for scanning.
     *
     * Parameters:
     *   stream_name - The name where the buffer came from, typically a media
     *          image filename.
     *   recursion_prefix - The recursion prefix, or "" for no recursion.
     *
     * Returns:
     *   "" else failure message.
     */
    std::string scan_setup(const std::string& stream_name,
                           const std::string& recursion_prefix);

    /**
     * Scan buffer, caching found artifacts, see empty() and get().
     *
     * Parameters:
     *   stream_offset - The byte offset of where this buffer starts.
     *   previous_buffer - The previously scanned buffer, used for
     *          composing artifacts that start before this buffer.
     *   previous_buffer_size - The number of bytes in the previous
     *          buffer, or 0 for no previous buffer.
     *   buffer - The buffer to scan.
     *   buffer_size - The number of bytes in the buffer to scan.
     *
     * Returns:
     *   "" else failure message.
     */
    std::string scan(const size_t stream_offset,
                     const char* const previous_buffer,
                     size_t previous_buffer_size,
                     const char* const buffer,
                     size_t buffer_size);

    /**
     * Finalize the scan, caching found artifacts, see empty() and get().
     * Always finalize your stream to capture artifacts that might have
     * been longer if there were more data.
     *
     * Parameters:
     *   stream_offset - The byte offset of where this buffer starts.
     *   previous_buffer - The previously scanned buffer, used for
     *          composing artifacts that start before this buffer.
     *   previous_buffer_size - The number of bytes in the previous
     *          buffer, or 0 for no previous buffer.
     *   buffer - The buffer to scan.
     *   buffer_size - The number of bytes in the buffer to scan.
     *
     * Returns:
     *   "" else failure message.
     */
    std::string scan_finalize(const size_t stream_offset,
                              const char* const previous_buffer,
                              size_t previous_buffer_size,
                              const char* const buffer,
                              size_t buffer_size);

    /**
     * Scan up to fence byte, finalize the scan, caching found artifacts,
     * see empty() and get().  Always finalize your stream to capture
     * artifacts that might have been longer if there were more data.
     *
     * Parameters:
     *   stream_offset - The byte offset of where this buffer starts.
     *   previous_buffer - The previously scanned buffer, used for
     *          composing artifacts that start before this buffer.
     *   previous_buffer_size - The number of bytes in the previous
     *          buffer, or 0 for no previous buffer.
     *   buffer - The buffer to scan.
     *   buffer_size - The number of bytes in the buffer to scan.
     *
     * Returns:
     *   "" else failure message.
     */
    std::string scan_fence_finalize(const size_t stream_offset,
                                    const char* const previous_buffer,
                                    size_t previous_buffer_size,
                                    const char* const buffer,
                                    size_t buffer_size);

    /**
     * Identify whether there are cached artifacts.
     *
     * Returns:
     *   True if there are no cached artifacts to retrieve.
     */
    bool empty() const;

    /**
     * Obtain a cached artifact.
     *
     * Returns:
     *   artifact, or a blank artifact if the cache is empty.
     */
    artifact_t get();

    /**
     * Destructor.
     */
    ~scanner_t();
  };

  /**
   * The uncompressed buffer class.  The uncompressor creates instances
   * of this.
   */
  class uncompressed_t {
    public:

    /**
     * Uncompressed binary data, can be empty.
     *
     * Java users may want to clear this when done rather than waiting
     * for it to lose scope in the GC.
     */
    std::string buffer;

    /**
     * Empty else error text.
     */
    std::string status;


    /**
     * Let the uncompressor return uncompressed objects.
     */
    uncompressed_t();
    uncompressed_t(const std::string& status);
    uncompressed_t(const unsigned char* const scratch_buf, size_t out_size);

    /**
     * For Java, return buffer as byte[] which can include \0.
     */
    void javaBuffer(std::string& java_buffer) const;
  };

  /**
   * The uncompressor class.  Use this to uncompress content such as zip
   * data.
   */
  class uncompressor_t {
    private:
    unsigned char* scratch_buffer;

#ifndef SWIG
    // do not allow copy or assignment
    uncompressor_t(const uncompressor_t&) = delete;
    uncompressor_t& operator=(const uncompressor_t&) = delete;
#endif

    public:
    /**
     * True if the uncompressor failed to initialize because scratch
     * memory could not be allocated.
     */
    const bool bad_alloc;

    /**
     * Get an uncompressor instance.  It will include a large scratch
     * buffer.
     */
    uncompressor_t ();

    /**
     * Destructor closes resources.
     */
    ~uncompressor_t ();

    /**
     * Uncompress content at the given offset in the buffer.
     *
     * Parameters:
     *   buffer - The buffer containing content to uncompress.
     *   buffer_size - The size of the buffer, in bytes.
     *   buffer_offset - The offset in the buffer to the content to
     *          uncompress.
     *
     * Returns:
     *   Uncompressed content.
     */
    uncompressed_t uncompress(const char* const buffer,
                              size_t buffer_size,
                              size_t buffer_offset);

    /**
     * Close uncompressor resources.  Alternatively, wait for the destructor
     * to do it.
     *
     * Java users may wish to call this when done in order to clear
     * allocated buffer space rather than waiting for the Garbage
     * Collector to trigger its release.
     */
    void close();

  };

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   input - The binary input string.
   *
   * Returns:
   *   Escaped output.
   */
  std::string escape(const std::string& input);

  /**
   * A helper function for formatting binary data into a printable string
   * by escaping non-printable bytes.
   *
   * Parameters:
   *   in_buffer - The buffer of bytes to format.
   *   in_buffer_size - The number of bytes in the buffer to format.
   *
   * Returns:
   *   Escaped output.
   */
  std::string javaEscape(const char* const buffer, size_t buffer_size);
}

#endif

