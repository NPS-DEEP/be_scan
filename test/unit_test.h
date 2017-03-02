// Author:  Bruce Allen
// Created: 2/25/2013
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
 * Support for simple unit testing.
 * WARNING: If LHS or RHS are functions and the test fails, the function
 *          will be run again when printing the error, which can return
 *          a differnet result than was actually tested.
 */

#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <cstdio>
#include <exception>

// fail if not equal
#define TEST_EQ( LHS, RHS )                                                  \
  if (!(LHS == RHS)) {                                                       \
    std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__               \
              << ": value " << LHS << " != " << RHS << "\n";                 \
    exit(1);                                                                 \
  }                                                                          \
  //

// fail if float not equal
#define TEST_FLOAT_EQ( LHS, RHS )                                            \
  if (!((int)(LHS*1000) == (int)(RHS*1000))) {                               \
    std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__               \
              << ": value " << LHS << " != " << RHS << "\n";                 \
    exit(1);                                                                 \
  }                                                                          \
  //

// fail if equal
#define TEST_NE( LHS, RHS )                                                  \
  if (LHS == RHS) {                                                          \
    std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__               \
              << ": value " << LHS << " == " << RHS << "\n";                 \
    exit(1);                                                                 \
  }                                                                          \
  //

// fail if expr does not throw correct excep
#define TEST_THROWS( EXPR, EXCEP )                                           \
  try {                                                                      \
    EXPR;                                                                    \
    std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__               \
              << ": Exception not thrown\n";                                 \
    exit(1);                                                                 \
  } catch(EXCEP const&) {                                                    \
  } catch(const std::exception& e) {                                         \
    std::cerr << "ERROR: " << __FILE__ << " line " << __LINE__               \
              << ": Wrong exception thrown.\n" << e.what() << "\n";          \
    exit(1);                                                                 \
  }                                                                          \
  //
#endif

