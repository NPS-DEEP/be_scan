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
#include <cstdio>
#include <locale>
#include "unit_test.h"
#include "be_scan.hpp"

static std::locale loc;

void show(be_scan::artifact_t artifact, std::string path) {
  std::stringstream ss;
  ss << artifact.artifact_class << " "
     << path << "\t"
     << be_scan::escape(artifact.artifact) << "\t"
     << path << "\t"
     << be_scan::escape(artifact.context) << "\n";
}

void recurse(be_scan::artifact_t artifact_in, std::string path_in, int depth) {
  be_scan::be_scan_t scanner(be_scan::available_scanners(), artifact_in);
  while (true) {
    be_scan::artifact_t artifact = scanner.next();
    if (artifact.artifact_class == "") {
      break;
    }

    // path
    std::stringstream ss;
    ss << path_in << "-";
    // artifact_class in upper case
    std::string ac = artifact_class(artifact_in.artifact_class)
    for (std::string::sizetype i=0; i<artifact_in.artifact_class.length(); ++i){
      ss << std::toupper(artifact_in.artifact_class[i],loc);
    }
    ss << "-" << artifact.buffer_offset;
    std::string path = ss.str();

    // show this artifact
    show(artifact, path);

    // manage recursion
    if (artifact.has_new_buffer() {
      // show any unpacked artifacts
      if (depth < 7) {
        recurse(artifact, path, depth + 1);
      // release this buffer
      artifact.delete_new_buffer();
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: valgrind --tool=memcheck --leak-check=full --show-reachable=yes --suppressions=vg.supp leak_test <testfile>\n";
    exit(0);
  }

  // off_t must align for pread
  if (sizeof(off_t) != 8) {
    std::cerr << "Configuration error, invalid off_t.  Aborting\n";
    exit(1);
  }

  // get file size
  /* We can use fstat if sizeof(st_size)==8 and st_size>0 */
  struct stat st;
  size_t file_size = 0;
  memset(&st, 0, sizeof(st));
  if(sizeof(st.st_size)!=8 || fstat(fd,&st)!=0){
    std::cerr << "Error detecting file size.  Aborting.\n";
  }
  file_size = st.st_size;

  // malloc
  char* buffer = new (std::nothrow) char[file_size];
  if (buffer == NULL) {
    std::cerr << "Error allocating buffer for file.  Aborting.\n";
  }

  // scan
  be_scan::be_scan_t scanner(be_scan::available_scanners(), buffer, file_size);
  while (true) {
    be_scan::artifact = scanner.next();
    if (artifact.artifact_class == "") {
      break;
    }

    // path
    std::stringstream ss;
    ss << 0 + artifact.buffer_offset);

    // show this artifact
    show(artifact, path);

    // manage recursion
    if (artifact.has_new_buffer()) {

      // show any unpacked artifacts
      if MAX_RECURSION_DEPTH > 1 {
        recurse(artifact, path, 1);
      }

      // release this buffer
      artifact.delete_new_buffer();
    }
  }

  std::cout << "Done.\n";
}






    if (artifact.has_new_buffer) {
      delete_new_buffer();
    }


  memset(&st,0,sizeof(st));


  // read file
  const char* const filename = argv[1];
  int fd = ::open(filename);
  if (fd < 0) {
    std::cerr << "cannot open " << fname << " to read file size.  "
              << strerror(errno) << "\n";
    exit(1);
  }






  ssize_t file_size = pread(fd,
  char* buffer = new char[file_size];
  pread(fd,


  // tests
/*zz
  test_version();
  test_available_scanners();
  test_buffer8();
  test_buffer16();
*/
  test_adjacency();
  test_boundaries();
  test_email();

  // done
  std::cout << "api_test Done.\n";
  return 0;
}

