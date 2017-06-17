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
#include <sstream>
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

void test_buffer8() {
  std::string string8 = "someone@somewhere.com\tsomeone2@somewhere2.com\n";
  const char* const bytes8 = string8.c_str();

  be_scan::scan_engine_t scan_engine("email");

  be_scan::scanner_t scanner(scan_engine, "unused output filename");

  TEST_EQ(scanner.scan("stream_name_test_buffer8", 0, "",
                       bytes8, string8.size()), "");

/*


  be_scan::be_scan_t scanner("email", bytes8, string8.size());

  TEST_EQ(scanner.bad_alloc, false);

  be_scan::artifact_t artifact;
  artifact = scanner.next();
  std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact_class, "email");
  TEST_EQ(artifact.buffer_offset, 0);
  TEST_EQ(artifact.artifact, "someone@somewhere.com");
  TEST_EQ(artifact.context, "someone@somewhere.com\tsomeone2@somewh");

  artifact = scanner.next();
  TEST_EQ(artifact.artifact_class, "email");
  TEST_EQ(artifact.buffer_offset, 22);
  TEST_EQ(artifact.artifact, "someone2@somewhere2.com");
  TEST_EQ(artifact.context, "e@somewhere.com\tsomeone2@somewhere2.com\n");

  artifact = scanner.next();
  TEST_EQ(artifact.artifact_class, "");
  TEST_EQ(artifact.buffer_offset, 0);
  TEST_EQ(artifact.artifact, "");
  TEST_EQ(artifact.context, "");
*/
}

void test_buffer16() {
  std::string string16(" \0a\0a\0a\0@\0b\0b\0.\0z\0w\0 \0", 22);
  const char* const bytes16 = string16.c_str();

  be_scan::scan_engine_t scan_engine("email");

  be_scan::scanner_t scanner(scan_engine, "unused output filename");

  scanner.scan("stream_name_test_buffer16", 0, "", bytes16, string16.size());

/*
  be_scan::be_scan_t scanner("email", bytes16, string16.size());

  be_scan::artifact_t artifact;
  artifact = scanner.next();
  TEST_EQ(artifact.artifact_class, "email");
  TEST_EQ(artifact.buffer_offset, 2);
  TEST_EQ(artifact.artifact, std::string("a\0a\0a\0@\0b\0b\0.\0z\0w\0", 18));
  TEST_EQ(artifact.context, std::string(" \0a\0a\0a\0@\0b\0b\0.\0z\0w\0 \0", 22));

  artifact = scanner.next();
  TEST_EQ(artifact.artifact_class, "");
  TEST_EQ(artifact.buffer_offset, 0);
  TEST_EQ(artifact.artifact, "");
  TEST_EQ(artifact.context, "");
*/
}

/*
void test_adjacency() {
  std::string string16("a\0a\0a\0@\0b\0b\0.\0z\0w\0\0\0b\0b\0b\0@\0c\0c\0.\0z\0w\0", 38);
  std::cout << "test_adjacency (" << string16.size() << ") string16: " << be_scan::escape(string16) << std::endl;
  const char* const bytes16 = string16.c_str();

  be_scan::be_scan_t scanner("email", bytes16, string16.size());

  be_scan::artifact_t artifact;
  artifact = scanner.next();
  std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact_class, "email");
  TEST_EQ(artifact.buffer_offset, 0);
  TEST_EQ(artifact.artifact, std::string("a\0a\0a\0@\0b\0b\0.\0z\0w\0", 18));
  TEST_EQ(artifact.context, std::string("a\0a\0a\0@\0b\0b\0.\0z\0w\0\0\0b\0b\0b\0@\0c\0c\0.\0", 34));

  artifact = scanner.next();
  std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact_class, "email");
  TEST_EQ(artifact.buffer_offset, 20);
  TEST_EQ(artifact.artifact, std::string("b\0b\0b\0@\0c\0c\0.\0z\0w\0", 18));
  TEST_EQ(artifact.context, std::string("a\0@\0b\0b\0.\0z\0w\0\0\0b\0b\0b\0@\0c\0c\0.\0z\0w\0", 34));

  artifact = scanner.next();
  TEST_EQ(artifact.artifact_class, "");
  TEST_EQ(artifact.buffer_offset, 0);
  TEST_EQ(artifact.artifact, "");
  TEST_EQ(artifact.context, "");
}

void test_boundaries() {
  be_scan::artifact_t artifact;
  std::string string;
  string = std::string("");
  be_scan::be_scan_t scanner0("email", string.c_str(), 0);
  artifact = scanner0.next();
  string = std::string(" ");
  be_scan::be_scan_t scanner1("email", string.c_str(), 1);
  artifact = scanner1.next();
  string = std::string("  ");
  be_scan::be_scan_t scanner2("email", string.c_str(), 2);
  artifact = scanner2.next();
  string = std::string("@\0",2);
  be_scan::be_scan_t scanner3("email", string.c_str(), 2);
  artifact = scanner3.next();
  string = std::string("\0@",2);
  be_scan::be_scan_t scanner4("email", string.c_str(), 2);
  artifact = scanner4.next();
  string = std::string("\0@\0",3);
  be_scan::be_scan_t scanner5("email", string.c_str(), 3);
  artifact = scanner5.next();
  string = std::string("a\0@\0",4);
  be_scan::be_scan_t scanner6("email", string.c_str(), 4);
  artifact = scanner6.next();
  string = std::string("a\0@\0a",5);
  be_scan::be_scan_t scanner7("email", string.c_str(), 5);
  artifact = scanner7.next();
}

void test_email() {
// 009714344575\x01\x00\x00\x00\xFF\xFF\xFF\xFF\x05\x00\x17\x00Emailnorbert@emirates.net.ae\x04\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x04\x00Modified\x01\x00\x00\x00\x04\x00\x00\x00\xFF\xFF\xFF\xFF\x0B\x00\x04\x00Preferences\x00\x00\x00\x00)\x00\x00\x00\xD3\x00\x05\x00)\x00\x00\x00\x15\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x9E\x00Contact5rubyeapen@hotmail.com\x00r\x00u\x00b\x00y\x00e\x00a\x00p\x00e\x00n\x00@\x00h\x00o\x00t\x00m\x00a\x00i\x00l\x00.\x00c\x00o\x00m\x00\x00\x00M\x00S\x00N\x00 \x00M\x00e\x00s\x00s\x00e\x00n\x00g\x00e\x00r\x00 \x00S\x00e\x00r\x00v\x00i\x00c\x00e\x00\x00\x004\x002\x00\x00\x00P\x00H\x00H\x00=\x00\x00\x00P\x00H\x00W\x00=\x00\x00\x00P\x00H\x00M\x00=\x00\x00\x00M\x00O\x00B\x00=\x00N\x00\x00\x00\x03\x00\x00\x00\xFF\xFF\xFF\xFF\x06\x00\x9E\x00Allow5rubyeape8LI/EI/DH0DH2DI5HH3FH1DI0DK0DH6HH4HH3HH1EH,CH.CH0DI/DI0BK0BI1BH4EL0BI0CI1DI0CG-DI.EG/FF1BE1>E6EJ5FK5GL6HM3GK4FJ5FI5GG7DI5DH3DH5II7GI8HI9JJ6HL8DL2@H0AH.BH0DH3HH2EF2CE3AG/AC.AD.AE1DG5HJ5HJ5HJ5FI5BF5CE6D\x18?\x1B F\x1A#I\x19'L\x1D3T#9Z';]+=a*:c)=d(?c(Bb)>d%9b-8c.9b/:b.;b*9a,8c/8e1@k5Fo4Fn4Gm.Bi$:k!9j\x1E9j\x187i!2n$5m(9l+=o,?r,?t+?v+?y-F~2L\x894O\x8E7R\x94;V\x96?U\x99@V\x9BBW\x9EC\x5C\xA2?W\x9F@W\x9DAW\x9B?U\x98?S\x98CS\x93FU\x94IX\x95L_\x98Mc\x99I_\x97F[\x96DV\x95FW\x95HZ\x97K]\x9AMb\x9DTe\xA1Pd\x9FLd\x9E

  const std::string buffer(
"\x30\x30\x39\x37\x31\x34\x33\x34\x34\x35\x37\x35\x01\x00\x00\x00"
"\xff\xff\xff\xff\x05\x00\x17\x00\x45\x6d\x61\x69\x6c\x6e\x6f\x72"
"\x62\x65\x72\x74\x40\x65\x6d\x69\x72\x61\x74\x65\x73\x2e\x6e\x65"
"\x74\x2e\x61\x65\x04\x00\x00\x00\xff\xff\xff\xff\x08\x00\x04\x00"
"\x4d\x6f\x64\x69\x66\x69\x65\x64\x01\x00\x00\x00\x04\x00\x00\x00"
"\xff\xff\xff\xff\x0b\x00\x04\x00\x50\x72\x65\x66\x65\x72\x65\x6e"
"\x63\x65\x73\x00\x00\x00\x00\x29\x00\x00\x00\xd3\x00\x05\x00\x29"
"\x00\x00\x00\x15\x00\x00\x00\x00\xff\xff\xff\xff\x08\x00\x9e\x00"
"\x43\x6f\x6e\x74\x61\x63\x74\x35\x72\x75\x62\x79\x65\x61\x70\x65"
"\x6e\x40\x68\x6f\x74\x6d\x61\x69\x6c\x2e\x63\x6f\x6d\x00\x72\x00"
"\x75\x00\x62\x00\x79\x00\x65\x00\x61\x00\x70\x00\x65\x00\x6e\x00"
"\x40\x00\x68\x00\x6f\x00\x74\x00\x6d\x00\x61\x00\x69\x00\x6c\x00"
"\x2e\x00\x63\x00\x6f\x00\x6d\x00\x00\x00\x4d\x00\x53\x00\x4e\x00"
"\x20\x00\x4d\x00\x65\x00\x73\x00\x73\x00\x65\x00\x6e\x00\x67\x00"
"\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x72\x00\x76\x00\x69\x00"
"\x63\x00\x65\x00\x00\x00\x34\x00\x32\x00\x00\x00\x50\x00\x48\x00"
"\x48\x00\x3d\x00\x00\x00\x50\x00\x48\x00\x57\x00\x3d\x00\x00\x00"
"\x50\x00\x48\x00\x4d\x00\x3d\x00\x00\x00\x4d\x00\x4f\x00\x42\x00"
"\x3d\x00\x4e\x00\x00\x00\x03\x00\x00\x00\xff\xff\xff\xff\x06\x00"
"\x9e\x00\x41\x6c\x6c\x6f\x77\x35\x72\x75\x62\x79\x65\x61\x70\x65"
"\x38\x4c\x49\x2f\x45\x49\x2f\x44\x48\x30\x44\x48\x32\x44\x49\x35"
"\x48\x48\x33\x46\x48\x31\x44\x49\x30\x44\x4b\x30\x44\x48\x36\x48"
"\x48\x34\x48\x48\x33\x48\x48\x31\x45\x48\x2c\x43\x48\x2e\x43\x48"
"\x30\x44\x49\x2f\x44\x49\x30\x42\x4b\x30\x42\x49\x31\x42\x48\x34"
"\x45\x4c\x30\x42\x49\x30\x43\x49\x31\x44\x49\x30\x43\x47\x2d\x44"
"\x49\x2e\x45\x47\x2f\x46\x46\x31\x42\x45\x31\x3e\x45\x36\x45\x4a"
"\x35\x46\x4b\x35\x47\x4c\x36\x48\x4d\x33\x47\x4b\x34\x46\x4a\x35"
"\x46\x49\x35\x47\x47\x37\x44\x49\x35\x44\x48\x33\x44\x48\x35\x49"
"\x49\x37\x47\x49\x38\x48\x49\x39\x4a\x4a\x36\x48\x4c\x38\x44\x4c"
"\x32\x40\x48\x30\x41\x48\x2e\x42\x48\x30\x44\x48\x33\x48\x48\x32"
"\x45\x46\x32\x43\x45\x33\x41\x47\x2f\x41\x43\x2e\x41\x44\x2e\x41"
"\x45\x31\x44\x47\x35\x48\x4a\x35\x48\x4a\x35\x48\x4a\x35\x46\x49"
"\x35\x42\x46\x35\x43\x45\x36\x44\x18\x3f\x1b\x20\x46\x1a\x23\x49"
"\x19\x27\x4c\x1d\x33\x54\x23\x39\x5a\x27\x3b\x5d\x2b\x3d\x61\x2a"
"\x3a\x63\x29\x3d\x64\x28\x3f\x63\x28\x42\x62\x29\x3e\x64\x25\x39"
"\x62\x2d\x38\x63\x2e\x39\x62\x2f\x3a\x62\x2e\x3b\x62\x2a\x39\x61"
"\x2c\x38\x63\x2f\x38\x65\x31\x40\x6b\x35\x46\x6f\x34\x46\x6e\x34"
"\x47\x6d\x2e\x42\x69\x24\x3a\x6b\x21\x39\x6a\x1e\x39\x6a\x18\x37"
"\x69\x21\x32\x6e\x24\x35\x6d\x28\x39\x6c\x2b\x3d\x6f\x2c\x3f\x72"
"\x2c\x3f\x74\x2b\x3f\x76\x2b\x3f\x79\x2d\x46\x7e\x32\x4c\x89\x34"
"\x4f\x8e\x37\x52\x94\x3b\x56\x96\x3f\x55\x99\x40\x56\x9b\x42\x57"
"\x9e\x43\x5c\xa2\x3f\x57\x9f\x40\x57\x9d\x41\x57\x9b\x3f\x55\x98"
"\x3f\x53\x98\x43\x53\x93\x46\x55\x94\x49\x58\x95\x4c\x5f\x98\x4d"
"\x63\x99\x49\x5f\x97\x46\x5b\x96\x44\x56\x95\x46\x57\x95\x48\x5a"
"\x97\x4b\x5d\x9a\x4d\x62\x9d\x54\x65\xa1\x50\x64\x9f\x4c\x64\x9e", 45*16);

  be_scan::be_scan_t scanner("email", buffer.c_str(), buffer.size());
  be_scan::artifact_t artifact;
  artifact = scanner.next();
  //std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  //std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact, "Emailnorbert@emirates.net.ae");
  TEST_EQ(be_scan::escape(artifact.context), be_scan::escape(std::string("4575\x01\x00\x00\x00\xFF\xFF\xFF\xFF\x05\x00\x17\x00" "Emailnorbert@emirates.net.ae\x04\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x04\x00Modi", 60)));
  artifact = scanner.next();
  //std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  //std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact, "Contact5rubyeapen@hotmail.com");
  TEST_EQ(be_scan::escape(artifact.context), be_scan::escape(std::string("\x00\x00\x00\x15\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x00\x9E\x00" "Contact5rubyeapen@hotmail.com\x00r\x00u\x00" "b\x00y\x00" "e\x00" "a\x00p\x00" "e", 61)));
  artifact = scanner.next();
  //std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  //std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;

  TEST_EQ(be_scan::escape(artifact.artifact), be_scan::escape(std::string("r\x00u\x00" "b\x00y\x00" "e\x00" "a\x00p\x00" "e\x00n\x00@\x00h\x00o\x00t\x00m\x00" "a\x00i\x00l\x00.\x00" "c\x00o\x00m\x00", 42)));

  TEST_EQ(be_scan::escape(artifact.context), be_scan::escape(std::string("pen@hotmail.com\x00r\x00u\x00" "b\x00y\x00" "e\x00" "a\x00p\x00" "e\x00n\x00@\x00h\x00o\x00t\x00m\x00" "a\x00i\x00l\x00.\x00" "c\x00o\x00m\x00\x00\x00M\x00S\x00N\x00 \x00M\x00" "e\x00s\x00", 74)));

  artifact = scanner.next();
  //std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  //std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact, "E6EJ5FK5GL6HM3GK4FJ5FI5GG7DI5DH3DH5II7GI8HI9JJ6HL8DL2@H0AH.BH");
  TEST_EQ(artifact.context, "CG-DI.EG/FF1BE1>E6EJ5FK5GL6HM3GK4FJ5FI5GG7DI5DH3DH5II7GI8HI9JJ6HL8DL2@H0AH.BH0DH3HH2EF2CE3AG/");
  artifact = scanner.next();
  //std::cout << "artifact: '" << be_scan::escape(artifact.artifact) << "'" << std::endl;
  //std::cout << "context: '" << be_scan::escape(artifact.context) << "'" << std::endl;
  TEST_EQ(artifact.artifact, "8e1@k5Fo4Fn4Gm.Bi");
  artifact = scanner.next();
  TEST_EQ(artifact.artifact, "");
}
*/

// ************************************************************
// main
// ************************************************************
int main(int argc, char* argv[]) {

  // tests
  // NOTE: tests are made to work with write_stdout.
  test_version();
  test_available_scanners();
  test_buffer8();
  test_buffer16();
//  test_adjacency();
//  test_boundaries();
//  test_email();

  // done
  std::cout << "api_test Done.\n";
  return 0;
}

