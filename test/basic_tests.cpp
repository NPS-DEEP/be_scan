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
#include "unit_test.h"
#include "be_scan.hpp"

void test_version() {
  std::cout << "be_scan version: '" << be_scan::version() << "'\n";
  std::string version = PACKAGE_VERSION;
  TEST_EQ(be_scan::version(), version);
}

void test_available_scanners() {
  std::string available_scanners(be_scan::available_scanners());
  std::cout << "Available scanners: '" << available_scanners << "'\n";
  TEST_EQ(available_scanners, be_scan::available_scanners());
}

void test_buffer1() {
  std::string string1 = "someone@somewhere.com\tsomeone2@somewhere2.com\n";
  std::cout << "string1 size: " << string1.size() << "\n";
  const char* const bytes1 = string1.c_str();

  be_scan::be_scan_t scanner("email", bytes1, string1.size());

  be_scan::artifact_t artifact1 = scanner.next_artifact();
  TEST_EQ(artifact1.artifact_class, "email");
  TEST_EQ(artifact1.artifact, "someone@somewhere.com");
  TEST_EQ(artifact1.context, "someone@somewhere.com\tsomeone2@somewh");

  be_scan::artifact_t artifact2 = scanner.next_artifact();
  TEST_EQ(artifact2.artifact_class, "email");
  TEST_EQ(artifact2.artifact, "someone2@somewhere2.com");
  TEST_EQ(artifact2.context, "e@somewhere.com\tsomeone2@somewhere2.com\n");

  be_scan::artifact_t artifact3 = scanner.next_artifact();
  TEST_EQ(artifact3.artifact_class, "");
  TEST_EQ(artifact3.artifact, "");
  TEST_EQ(artifact3.context, "");
}


// ************************************************************
// main
// ************************************************************
int main(int argc, char* argv[]) {

  // tests
  test_version();
  test_available_scanners();
  test_buffer1();

  // done
  std::cout << "api_test Done.\n";
  return 0;
}

