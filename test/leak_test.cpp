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
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <locale>
#include <sys/stat.h>
#include "be_scan.hpp"

static std::locale loc;

void show(be_scan::artifact_t artifact, std::string path) {
  std::cout << artifact.artifact_class << " "
            << path << "\t"
            << be_scan::escape(artifact.artifact) << "\t"
            << be_scan::escape(artifact.context) << "\n";
}

void recurse(be_scan::artifact_t artifact_in, std::string path_in, int depth) {
  be_scan::be_scan_t scanner(be_scan::available_scanners(), artifact_in);
  while (true) {
    be_scan::artifact_t artifact = scanner.next();
    if (artifact.artifact_class == "") {
      break;
    }

    // compose path
    std::stringstream ss;
    ss << path_in << "-";

    // artifact_class in upper case
    std::string::iterator it;
    for (it = artifact_in.artifact_class.begin();
         it < artifact_in.artifact_class.end(); ++it) {
      ss << std::toupper(*it, loc);
    }
    ss << "-" << artifact.buffer_offset;
    std::string path = ss.str();

    // show this artifact
    show(artifact, path);

    // manage recursion
    if (artifact.has_new_buffer()) {

      // show any unpacked artifacts
      if (depth < 7) {
        recurse(artifact, path, depth + 1);
      }

      // release this buffer
      artifact.delete_new_buffer();
    }
  }
}

static const size_t BUFFER_SIZE = 134217728; // 2^27 = 128MiB

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./leak_test <testfile>\n";
    exit(0);
  }

  // off_t must align for pread
  if (sizeof(off_t) != 8) {
    std::cerr << "Configuration error, invalid off_t.  Aborting\n";
    exit(1);
  }

  // open file
  const char* const filename = argv[1];
  int fd = ::open(filename, O_RDONLY);

  // get file size
  /* We can use fstat if sizeof(st_size)==8 and st_size>0 */
  struct stat st;
  size_t file_size;
  memset(&st, 0, sizeof(st));
  if(sizeof(st.st_size)!=8 || fstat(fd, &st)!=0){
    std::cerr << "Error detecting file size.  Aborting.\n";
    exit(1);
  }
  file_size = st.st_size;

  // malloc
  char* buffer = new (std::nothrow) char[BUFFER_SIZE];
  if (buffer == NULL) {
    std::cerr << "Error allocating buffer for file.  Aborting.\n";
    exit(1);
  }

  // iterate through slices of the file
  size_t offset = 0;
  while (offset < file_size) {

    // read into buffer
    ssize_t count = ::pread64(fd, buffer, BUFFER_SIZE, offset);

    if (count < 0) {
      std::cerr << "Error reading file: " << count << ".  Aborting.\n";
      exit(1);
    }

    // runtime status
    std::cout << "File " << filename << " start " << offset
              << " count " << count << "\n";

    // open scanner
    be_scan::be_scan_t scanner(be_scan::available_scanners(), buffer, count);
    while (true) {
      be_scan::artifact_t artifact = scanner.next();
      if (artifact.artifact_class == "") {
        break;
      }

      // compose path
      std::stringstream ss;
      ss << (offset + artifact.buffer_offset);
      std::string path = ss.str();

      // show this artifact
      show(artifact, path);

      // manage recursion
      if (artifact.has_new_buffer()) {

        // show any unpacked artifacts
        recurse(artifact, path, 1);

        // release this buffer
        artifact.delete_new_buffer();
      }
    }

    // move to next offset
    offset += count;
  }

  // release primary buffer
  delete[] buffer;

  std::cout << "Done.\n";
  return 0;
}

