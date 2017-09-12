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
#include <queue>
#include <iostream>
#include <pthread.h>
#include "be_scan.hpp"
#include "artifacts.hpp"

namespace be_scan {

  // artifacts_t
  artifacts_t::artifacts_t() : artifacts(std::queue<artifact_t>()), M() {
    pthread_mutex_init(&M, NULL);
  }

  // put, threadsafe, warn and do not put empty artifact
  void artifacts_t::put(const artifact_t& artifact) {
    if (artifact.blank()) {
      // warn
      std::cerr << "warning: artifacts_t put blank.\n";
      return;
    }
    pthread_mutex_lock(&M);
    artifacts.push(artifact);
    pthread_mutex_unlock(&M);
  }

  // get, threadsafe, return empty artifact when empty
  artifact_t artifacts_t::get() {
    pthread_mutex_lock(&M);

    if (artifacts.empty()) {
      pthread_mutex_unlock(&M);
      return artifact_t();
    } else {
      artifact_t artifact = artifacts.front();
      artifacts.pop();
      pthread_mutex_unlock(&M);
      return artifact;
    }
  }
}

