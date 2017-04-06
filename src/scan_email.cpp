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
#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include "be_scan.hpp"
#include "scan_email.hpp"
#include "artifact_context.hpp"
#include "escape.hpp"

namespace be_scan {

//  private:
//  const char* const scan_email_t::buffer;
//  const size_t scan_email_t::buffer_size;
//  size_t scan_email_t::index;

  const std::set<std::string> scan_email_t::domain_names {
"AC","AD","AE","AERO","AF","AG","AI","AL","AM","AN","AO","AQ","AR","ARPA","AS","ASIA","AT","AU","AW","AX","AZ","BA","BB","BD","BE","BF","BG","BH","BI","BIZ","BJ","BL","BM","BN","BO","BR","BS","BT","BV","BW","BY","BZ","CA","CAT","CC","CD","CF","CG","CH","CI","CK","CL","CM","CN","CO","COM","COOP","CR","CU","CV","CX","CY","CZ","DE","DJ","DK","DM","DO","DZ","EC","EDU","EE","EG","EH","ER","ES","ET","EU","FI","FJ","FK","FM","FO","FR","GA","GB","GD","GE","GF","GG","GH","GI","GL","GM","GN","GOV","GP","GQ","GR","GS","GT","GU","GW","GY","HK","HM","HN","HR","HT","HU","ID","IE","IL","IM","IN","INFO","INT","IO","IQ","IR","IS","IT","JE","JM","JO","JOBS","JP","KE","KG","KH","KI","KM","KN","KP","KR","KW","KY","KZ","LA","LB","LC","LI","LK","LR","LS","LT","LU","LV","LY","MA","MC","MD","ME","MF","MG","MH","MIL","MK","ML","MM","MN","MO","MOBI","MP","MQ","MR","MS","MT","MU","MUSEUM","MV","MW","MX","MY","MZ","NA","NAME","NC","NE","NET","NF","NG","NI","NL","NO","NP","NR","NU","NZ","OM","ORG","PA","PE","PF","PG","PH","PK","PL","PM","PN","PR","PRO","PS","PT","PW","PY","QA","RE","RO","RS","RU","RW","SA","SB","SC","SD","SE","SG","SH","SI","SJ","SK","SL","SM","SN","SO","SR","ST","SU","SV","SY","SZ","TC","TD","TEL","TF","TG","TH","TJ","TK","TL","TM","TN","TO","TP","TR","TRAVEL","TT","TV","TW","TZ","UA","UG","UK","UM","US","UY","UZ","VA","VC","VE","VG","VI","VN","VU","WF","WS","YE","YT","YU","ZA","ZM","ZW"
  };

  // from https://en.wikipedia.org/wiki/Email_address
  // Use RFC 5322 except do not recognize backslash or quoted string.
  // Specifically: local part <= 64 characters, domain <= 255 characters and
  // not space or "(),:;<>@[\]
  inline bool not_local_part(const unsigned char c) {
    return (c<0x20 || c >0x7f || c=='\"' || c=='(' || c==')' || c==',' ||
            c==':' || c==';' || c=='<' || c=='>' || c=='@' || c=='[' ||
            c=='\\' || c==']');
  }
  inline bool not_domain(const unsigned char c) {
    return (c<0x20 || c >0x7f || c=='\"' || c=='(' || c==')' || c==',' ||
            c==':' || c==';' || c=='<' || c=='>' || c=='@' || c=='[' ||
            c=='\\' || c==']');
  }

  // find local part of email address, return start else "at" point
  size_t scan_email_t::find_start(const size_t at) {
    size_t start = at;
    while (true) {
      // done if at beginning
      if (start == 0) {
        return start;
      }

      // invalid if local part > 64 bytes long
      if (at - start > 64) {
        return at;
      }

      // done if next char is invalid
      unsigned char c = buffer[start-1];
      if (not_local_part(c)) {
        return start;
      }

      // char is valid so step backwards to it
      --start;
    }
  }

  // find domain part of email address, return stop else "at" point
  size_t scan_email_t::find_stop(const size_t at) {
    size_t stop = at;
    while (true) {
      // done if at EOF
      if (stop == buffer_size - 1) {
        return stop;
      }

      // invalid if domain part > 256 bytes long
      if (stop - at > 256) {
        return at;
      }

      // done if next char is invalid
      unsigned char c = buffer[stop+1];
      if (not_domain(c)) {
        return stop;
      }

      // char is valid so step forward to it
      ++stop;
    }
  }

  // find local part of email address, return start else "at" point
  size_t scan_email_t::find_start16(const size_t at) {
    size_t start = at;
    while (true) {
      // done if at beginning
      if (start <= 1) {
        return start;
      }

      // invalid if local part > 64 bytes long
      if (at - start > 64 * 2) {
        return at;
      }

      // done if next char pair is invalid
      if (buffer[start-1] != '\0') {
        return start;
      }
      unsigned char c = buffer[start-2];
      if (not_local_part(c)) {
        return start;
      }

      // char pair is valid so step backwards to it
      start -= 2;
    }
  }

  // find domain part of email address, return stop else "at" point
  // where stop is first byte of last pair
  size_t scan_email_t::find_stop16(const size_t at) {
    size_t stop = at;
    while (true) {
      // done if at EOF
      if (stop >= buffer_size - 2) {
        return stop;
      }

      // invalid if domain part > 256 bytes long
      if (stop - at > 256 * 2) {
        return at;
      }

      // done if next char is invalid
      if (buffer[stop+1] != '\0') {
        return stop;
      }
      unsigned char c = buffer[stop+2];
      if (not_domain(c)) {
        return stop;
      }

      // char is valid so step forward to it
      stop += 2;
    }
  }

  bool scan_email_t::valid_top_level_domain(const std::string& feature) {
    // find last dot, it will preceed the top-level domain name
    const size_t pos = feature.rfind('.');
    const size_t size = feature.size();
    if (pos == std::string::npos) {
      // not a valid feature
      return false;
    }

    // extract top-level domain name
    std::stringstream ss;
    for (size_t i=pos+1; i <= size-1; ++i) {
      char c = feature[i];
      if (c >= 0x61 && c <= 0x7a) {
        // to upper
        c -= 0x20;
      }
      ss << c;
    }

    // require to recognize the domain name
    return (domain_names.find(ss.str()) != domain_names.end());
  }

  //  public:
  scan_email_t::scan_email_t(const char* const p_buffer,
                             const size_t p_buffer_size) :
                  buffer(p_buffer),
                  buffer_size(p_buffer_size),
                  index(0),
                  flex_extra_parameters() {
    flex_init();
  }

  scan_email_t::~scan_email_t() {
    flex_close();
  }

  artifact_t scan_email_t::next() {
    // progress forward to artifact or end of buffer
    for (; index<buffer_size-1; ++index) {
      if (buffer[index] == '@') {
        if (buffer[index+1] != '\0') {
          // unicode 8
          const size_t start = find_start(index);
          if (start == index) {
            continue;
          }
          const size_t stop = find_stop(index);
          if (stop == index) {
            continue;
          }

          // build email address from this
          const std::string feature = std::string(&buffer[start], stop-start+1);

          // validate simple
//          if (!valid_top_level_domain(feature)) {
//            continue;
//          }

          // validate regex
          flex_scan(feature); // sets flex_extra_parameters
          if (flex_extra_parameters.flex_size == 0) {
            // not valid in flex
            continue;
          }

          // refine the location of the artifact based on regex
          const size_t offset = start + flex_extra_parameters.flex_offset;
          const size_t size = flex_extra_parameters.flex_size;

          // advance index past the @ character
          ++index;

          return artifact_t("email", offset, std::string(&buffer[offset], size),
                            artifact_context(buffer, buffer_size,
                            offset, size, 16));

        } else {
          // unicode 16
          size_t start = find_start16(index);
          if (start == index) {
            continue;
          }
          size_t stop = find_stop16(index);
          if (stop == index) {
            continue;
          }

          // generate returned unicode 16 feature
          // zz NOTE: in future, return unicode 8 feature
          const std::string feature16 = std::string(&buffer[start], stop-start+1);

          // build unicode 8 from this
          std::stringstream ss;
          for (size_t j=start; j <= stop+1; j+=2) {
            ss << buffer[j];
          }

          // get the feature as unicode 8
          const std::string feature8 = ss.str();

          // validate simple
//          if (!valid_top_level_domain(feature8)) {
//            continue;
//          }

          // validate regex
          flex_scan(feature8); // sets flex_extra_parameters
          if (flex_extra_parameters.flex_size == 0) {
            // not valid in flex
            continue;
          }

          // refine the location of the artifact based on regex
          const size_t offset = start + flex_extra_parameters.flex_offset;
          const size_t size = flex_extra_parameters.flex_size;

          // advance index past the @ character
          ++index;

          return artifact_t("email", offset,
                            std::string(&buffer[offset], size*2),
                            artifact_context(buffer, buffer_size,
                            offset, size*2, 16));
        }
      }
    }
    // no more artifacts for this buffer
    return artifact_t("", 0, "", "");
  }
}

