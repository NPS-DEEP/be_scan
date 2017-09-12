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

// adapted from https://github.com/simsong/bulk_extractor.

#include <config.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <set>
#include "scanners.hpp"
#include "scanner_data.hpp"
#include "lightgrep_wrapper.hpp"

namespace email {

  // ************************************************************
  // general support
  // ************************************************************

  //
  // subpatterns
  //

  const std::string INUM("(1?[0-9]{1,2}|2([0-4][0-9]|5[0-5]))");
  const std::string HEX("[0-9a-f]");
  const std::string ALNUM("[a-zA-Z0-9]");

  const std::string PC("[\\x20-\\x7E]");

  const std::string TLD("(AC|AD|AE|AERO|AF|AG|AI|AL|AM|AN|AO|AQ|AR|ARPA|AS|ASIA|AT|AU|AW|AX|AZ|BA|BB|BD|BE|BF|BG|BH|BI|BIZ|BJ|BL|BM|BN|BO|BR|BS|BT|BV|BW|BY|BZ|CA|CAT|CC|CD|CF|CG|CH|CI|CK|CL|CM|CN|CO|COM|COOP|CR|CU|CV|CX|CY|CZ|DE|DJ|DK|DM|DO|DZ|EC|EDU|EE|EG|EH|ER|ES|ET|EU|FI|FJ|FK|FM|FO|FR|GA|GB|GD|GE|GF|GG|GH|GI|GL|GM|GN|GOV|GP|GQ|GR|GS|GT|GU|GW|GY|HK|HM|HN|HR|HT|HU|ID|IE|IL|IM|IN|INFO|INT|IO|IQ|IR|IS|IT|JE|JM|JO|JOBS|JP|KE|KG|KH|KI|KM|KN|KP|KR|KW|KY|KZ|LA|LB|LC|LI|LK|LR|LS|LT|LU|LV|LY|MA|MC|MD|ME|MF|MG|MH|MIL|MK|ML|MM|MN|MO|MOBI|MP|MQ|MR|MS|MT|MU|MUSEUM|MV|MW|MX|MY|MZ|NA|NAME|NC|NE|NET|NF|NG|NI|NL|NO|NP|NR|NU|NZ|OM|ORG|PA|PE|PF|PG|PH|PK|PL|PM|PN|PR|PRO|PS|PT|PW|PY|QA|RE|RO|RS|RU|RW|SA|SB|SC|SD|SE|SG|SH|SI|SJ|SK|SL|SM|SN|SO|SR|ST|SU|SV|SY|SZ|TC|TD|TEL|TF|TG|TH|TJ|TK|TL|TM|TN|TO|TP|TR|TRAVEL|TT|TV|TW|TZ|UA|UG|UK|UM|US|UY|UZ|VA|VC|VE|VG|VI|VN|VU|WF|WS|YE|YT|YU|ZA|ZM|ZW)");

  const std::string YEAR("(19[6-9][0-9]|20[0-1][0-9])");
  const std::string DAYOFWEEK("(Mon|Tue|Wed|Thu|Fri|Sat|Sun)");
  const std::string MONTH("(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)");
  const std::string ABBREV("(UTC?|GMT|EST|EDT|CST|CDT|MST|MDT|PST|PDT|[ZAMNY])");

  //
  // patterns
  //

//  const std::string EMAIL(ALNUM + "(\\.?[a-zA-Z0-9_%\\-+])+\\.?" + ALNUM + "@" + ALNUM + "(\\.?[a-zA-Z0-9_%\\-])+\\." + TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

  const std::string EMAIL(ALNUM + "(\\.?[a-zA-Z0-9_%\\-+]){1,64}\\.?" + ALNUM + "@" + ALNUM + "(\\.?[a-zA-Z0-9_%\\-]){1,64}\\." + TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

  //
  // helper functions
  //

  std::string find_domain_in_email(const std::string& email) {
    size_t i = email.find("@");
    if (i==std::string::npos) {
      return "";
    } else {
      return email.substr(i+1);
    }
  }

  template <typename T>
  inline size_t find_domain_in_url(const T* buf, size_t buflen, size_t& domain_len) {
    const T* dbeg = search_n(buf, buf + buflen, 2, '/') + 2;
    if (dbeg < buf + buflen) {
      const T stop[] = { '/', ':' };
      const T* dend = find_first_of(dbeg, buf + buflen, stop, stop + 2);
      domain_len = dend - dbeg;
      return dbeg - buf;
    }

    return buflen;
  }

  bool valid_ether_addr(const uint8_t* buf) {
    if (std::memcmp((const uint8_t *)"00:00:00:00:00:00", buf, 17) == 0) {
      return false;
    }

    if (std::memcmp((const uint8_t *)"00:11:22:33:44:55", buf, 17) == 0) {
      return false;
    }

    /* Perform a quick histogram analysis.
     * For each group of characters, create a value based on the two digits.
     * There is no need to convert them to their 'actual' value.
     * Don't accept a histogram that has 3 values. That could be
     * 11:11:11:11:22:33
     * Require 4, 5 or 6.
     * If we have 4 or more distinct values, then treat it good.
     * Otherwise its is some pattern we don't want.
     */
    std::set<uint16_t> ctr;
    for (uint32_t i = 0; i < 6; ++i) {  // loop over each group
      // create a unique value of the two characters
      ctr.insert((buf[i*3] << 8) + buf[i*3+1]);
    }

    return ctr.size() >= 4;
  }

  template <typename T>
  bool valid_ipaddr(const T* leftguard, const T* hit) {
    // copy up to 'window' preceding Ts into context array
    static const ssize_t window = 8;
    T context[window] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    const ssize_t diff = min(hit - leftguard, window);
    copy(hit - diff, hit, context + window - diff);

    if (
      isalnum(context[7]) ||
      context[7] == '.' ||
      context[7] == '-' ||
      context[7] == '+' ||
      (ishexnumber(context[4]) && ishexnumber(context[5]) &&
       ishexnumber(context[6]) && context[7] == '}') ||
      (*hit == '0' && *(hit + 1) == '.'))
    {
      // ignore
      return false;
    }

    static const struct {
      size_t pos;
      const char* str;
    } checks[] = {
      { 5, "v."     },
      { 5, "v "     },
      { 5, "rv:"    },  // rv:1.9.2.8 as in Mozilla
      { 4, ">="     },  // >= 1.8.0.10
      { 4, "<="     },  // <= 1.8.0.10
      { 4, "<<"     },  // << 1.8.0.10
      { 4, "ver"    },
      { 4, "Ver"    },
      { 4, "VER"    },
      { 0, "rsion"  },
      { 0, "ion="   },
      { 0, "PSW/"   },  // PWS/1.5.19.3 ...
      { 0, "flash=" },  // flash=
      { 0, "stone=" },  // Milestone=
      { 4, "NSS"    },
      { 0, "/2001," },  // /2001,3.60.50.8
      { 0, "TI_SZ"  }   // %REG_MULTI_SZ%,
    };

    for (size_t i = 0; i < sizeof(checks)/sizeof(checks[0]); ++i) {
      if (search(
        context + checks[i].pos,
        context + 8, checks[i].str,
        checks[i].str + strlen(checks[i].str)
      ) != context + 8) {
        return false;
      }
    }

    return true;
  }

  // ************************************************************
  // callback functions
  // ************************************************************

  void emailHitHandler(const uint64_t start, const uint64_t size,
                       void* p_scanner_data) {

    // typecast void* into scanner_data
    be_scan::scanner_data_t* scanner_data(
                   static_cast<be_scan::scanner_data_t*>(p_scanner_data));

    // artifact
    std::string artifact = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size-1, 0);

    // context
    std::string context = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size-1, 16);

    scanner_data->artifacts.put(be_scan::artifact_t(
                   "email",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }

  void emailUTF16LEHitHandler(const uint64_t start, const uint64_t size,
                              void* p_scanner_data) {

    // typecast void* into scanner_data
    be_scan::scanner_data_t* scanner_data(
                   static_cast<be_scan::scanner_data_t*>(p_scanner_data));

    // artifact
    std::string artifact = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size-2, 0);

    // context
    std::string context = lw::read_buffer(
                   scanner_data->stream_offset,
                   scanner_data->previous_buffer,
                   scanner_data->previous_buffer_size,
                   scanner_data->buffer,
                   scanner_data->buffer_size,
                   start, size-2, 16);

    scanner_data->artifacts.put(be_scan::artifact_t(
                   "email",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }

  // ************************************************************
  // regex
  // ************************************************************

  std::string add_email_regex(lw::lw_scanner_program_t& lw_scanner_program) {
    std::string status;
    status = lw_scanner_program.add_regex(
                       EMAIL, "UTF-8", true, false, &emailHitHandler);
    if (status != "") return status;
    status = lw_scanner_program.add_regex(
                       EMAIL, "UTF-16LE", true, false, &emailHitHandler);
    if (status != "") return status;
    return "";
  }
}

