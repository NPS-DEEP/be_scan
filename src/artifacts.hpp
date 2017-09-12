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
 * Threadsafe FIFO for artifacts.
 */

#ifndef ARTIFACTS_HPP
#define ARTIFACTS_HPP

#include <config.h>
#include <queue>
#include <pthread.h>
#include "be_scan.hpp"

// scanner support
namespace be_scan {

  class artifacts_t {
    private:
    std::queue<artifact_t> artifacts;
    mutable pthread_mutex_t M;

    // do not allow copy or assignment
    artifacts_t(const artifacts_t&) = delete;
    artifacts_t& operator=(const artifacts_t&) = delete;

    public:
    // constructor
    artifacts_t();

    // threadsafe enqueue, does not enqueue empty artifacts.
    void put(const artifact_t& artifact);

    // threadsafe dequeue, returns empty artifact when empty.
    artifact_t get();
  };

}

#endif

