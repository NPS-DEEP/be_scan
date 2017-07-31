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

  const std::string U_ALNUM("[a-zA-Z0-9]\\z00");
  const std::string U_TLD("(A\\z00C\\z00|A\\z00D\\z00|A\\z00E\\z00|A\\z00E\\z00R\\z00O\\z00|A\\z00F\\z00|A\\z00G\\z00|A\\z00I\\z00|A\\z00L\\z00|A\\z00M\\z00|A\\z00N\\z00|A\\z00O\\z00|A\\z00Q\\z00|A\\z00R\\z00|A\\z00R\\z00P\\z00A\\z00|A\\z00S\\z00|A\\z00S\\z00I\\z00A\\z00|A\\z00T\\z00|A\\z00U\\z00|A\\z00W\\z00|A\\z00X\\z00|A\\z00Z\\z00|B\\z00A\\z00|B\\z00B\\z00|B\\z00D\\z00|B\\z00E\\z00|B\\z00F\\z00|B\\z00G\\z00|B\\z00H\\z00|B\\z00I\\z00|B\\z00I\\z00Z\\z00|B\\z00J\\z00|B\\z00L\\z00|B\\z00M\\z00|B\\z00N\\z00|B\\z00O\\z00|B\\z00R\\z00|B\\z00S\\z00|B\\z00T\\z00|B\\z00V\\z00|B\\z00W\\z00|B\\z00Y\\z00|B\\z00Z\\z00|C\\z00A\\z00|C\\z00A\\z00T\\z00|C\\z00C\\z00|C\\z00D\\z00|C\\z00F\\z00|C\\z00G\\z00|C\\z00H\\z00|C\\z00I\\z00|C\\z00K\\z00|C\\z00L\\z00|C\\z00M\\z00|C\\z00N\\z00|C\\z00O\\z00|C\\z00O\\z00M\\z00|C\\z00O\\z00O\\z00P\\z00|C\\z00R\\z00|C\\z00U\\z00|C\\z00V\\z00|C\\z00X\\z00|C\\z00Y\\z00|C\\z00Z\\z00|D\\z00E\\z00|D\\z00J\\z00|D\\z00K\\z00|D\\z00M\\z00|D\\z00O\\z00|D\\z00Z\\z00|E\\z00C\\z00|E\\z00D\\z00U\\z00|E\\z00E\\z00|E\\z00G\\z00|E\\z00H\\z00|E\\z00R\\z00|E\\z00S\\z00|E\\z00T\\z00|E\\z00U\\z00|F\\z00I\\z00|F\\z00J\\z00|F\\z00K\\z00|F\\z00M\\z00|F\\z00O\\z00|F\\z00R\\z00|G\\z00A\\z00|G\\z00B\\z00|G\\z00D\\z00|G\\z00E\\z00|G\\z00F\\z00|G\\z00G\\z00|G\\z00H\\z00|G\\z00I\\z00|G\\z00L\\z00|G\\z00M\\z00|G\\z00N\\z00|G\\z00O\\z00V\\z00|G\\z00P\\z00|G\\z00Q\\z00|G\\z00R\\z00|G\\z00S\\z00|G\\z00T\\z00|G\\z00U\\z00|G\\z00W\\z00|G\\z00Y\\z00|H\\z00K\\z00|H\\z00M\\z00|H\\z00N\\z00|H\\z00R\\z00|H\\z00T\\z00|H\\z00U\\z00|I\\z00D\\z00|I\\z00E\\z00|I\\z00L\\z00|I\\z00M\\z00|I\\z00N\\z00|I\\z00N\\z00F\\z00O\\z00|I\\z00N\\z00T\\z00|I\\z00O\\z00|I\\z00Q\\z00|I\\z00R\\z00|I\\z00S\\z00|I\\z00T\\z00|J\\z00E\\z00|J\\z00M\\z00|J\\z00O\\z00|J\\z00O\\z00B\\z00S\\z00|J\\z00P\\z00|K\\z00E\\z00|K\\z00G\\z00|K\\z00H\\z00|K\\z00I\\z00|K\\z00M\\z00|K\\z00N\\z00|K\\z00P\\z00|K\\z00R\\z00|K\\z00W\\z00|K\\z00Y\\z00|K\\z00Z\\z00|L\\z00A\\z00|L\\z00B\\z00|L\\z00C\\z00|L\\z00I\\z00|L\\z00K\\z00|L\\z00R\\z00|L\\z00S\\z00|L\\z00T\\z00|L\\z00U\\z00|L\\z00V\\z00|L\\z00Y\\z00|M\\z00A\\z00|M\\z00C\\z00|M\\z00D\\z00|M\\z00E\\z00|M\\z00F\\z00|M\\z00G\\z00|M\\z00H\\z00|M\\z00I\\z00L\\z00|M\\z00K\\z00|M\\z00L\\z00|M\\z00M\\z00|M\\z00N\\z00|M\\z00O\\z00|M\\z00O\\z00B\\z00I\\z00|M\\z00P\\z00|M\\z00Q\\z00|M\\z00R\\z00|M\\z00S\\z00|M\\z00T\\z00|M\\z00U\\z00|M\\z00U\\z00S\\z00E\\z00U\\z00M\\z00|M\\z00V\\z00|M\\z00W\\z00|M\\z00X\\z00|M\\z00Y\\z00|M\\z00Z\\z00|N\\z00A\\z00|N\\z00A\\z00M\\z00E\\z00|N\\z00C\\z00|N\\z00E\\z00|N\\z00E\\z00T\\z00|N\\z00F\\z00|N\\z00G\\z00|N\\z00I\\z00|N\\z00L\\z00|N\\z00O\\z00|N\\z00P\\z00|N\\z00R\\z00|N\\z00U\\z00|N\\z00Z\\z00|O\\z00M\\z00|O\\z00R\\z00G\\z00|P\\z00A\\z00|P\\z00E\\z00|P\\z00F\\z00|P\\z00G\\z00|P\\z00H\\z00|P\\z00K\\z00|P\\z00L\\z00|P\\z00M\\z00|P\\z00N\\z00|P\\z00R\\z00|P\\z00R\\z00O\\z00|P\\z00S\\z00|P\\z00T\\z00|P\\z00W\\z00|P\\z00Y\\z00|Q\\z00A\\z00|R\\z00E\\z00|R\\z00O\\z00|R\\z00S\\z00|R\\z00U\\z00|R\\z00W\\z00|S\\z00A\\z00|S\\z00B\\z00|S\\z00C\\z00|S\\z00D\\z00|S\\z00E\\z00|S\\z00G\\z00|S\\z00H\\z00|S\\z00I\\z00|S\\z00J\\z00|S\\z00K\\z00|S\\z00L\\z00|S\\z00M\\z00|S\\z00N\\z00|S\\z00O\\z00|S\\z00R\\z00|S\\z00T\\z00|S\\z00U\\z00|S\\z00V\\z00|S\\z00Y\\z00|S\\z00Z\\z00|T\\z00C\\z00|T\\z00D\\z00|T\\z00E\\z00L\\z00|T\\z00F\\z00|T\\z00G\\z00|T\\z00H\\z00|T\\z00J\\z00|T\\z00K\\z00|T\\z00L\\z00|T\\z00M\\z00|T\\z00N\\z00|T\\z00O\\z00|T\\z00P\\z00|T\\z00R\\z00|T\\z00R\\z00A\\z00V\\z00E\\z00L\\z00|T\\z00T\\z00|T\\z00V\\z00|T\\z00W\\z00|T\\z00Z\\z00|U\\z00A\\z00|U\\z00G\\z00|U\\z00K\\z00|U\\z00M\\z00|U\\z00S\\z00|U\\z00Y\\z00|U\\z00Z\\z00|V\\z00A\\z00|V\\z00C\\z00|V\\z00E\\z00|V\\z00G\\z00|V\\z00I\\z00|V\\z00N\\z00|V\\z00U\\z00|W\\z00F\\z00|W\\z00S\\z00|Y\\z00E\\z00|Y\\z00T\\z00|Y\\z00U\\z00|Z\\z00A\\z00|Z\\z00M\\z00|Z\\z00W\\z00)");
//  const std::string U_TLD("(A\0C\0|A\0D\0|A\0E\0|A\0E\0R\0O\0|A\0F\0|A\0G\0|A\0I\0|A\0L\0|A\0M\0|A\0N\0|A\0O\0|A\0Q\0|A\0R\0|A\0R\0P\0A\0|A\0S\0|A\0S\0I\0A\0|A\0T\0|A\0U\0|A\0W\0|A\0X\0|A\0Z\0|B\0A\0|B\0B\0|B\0D\0|B\0E\0|B\0F\0|B\0G\0|B\0H\0|B\0I\0|B\0I\0Z\0|B\0J\0|B\0L\0|B\0M\0|B\0N\0|B\0O\0|B\0R\0|B\0S\0|B\0T\0|B\0V\0|B\0W\0|B\0Y\0|B\0Z\0|C\0A\0|C\0A\0T\0|C\0C\0|C\0D\0|C\0F\0|C\0G\0|C\0H\0|C\0I\0|C\0K\0|C\0L\0|C\0M\0|C\0N\0|C\0O\0|C\0O\0M\0|C\0O\0O\0P\0|C\0R\0|C\0U\0|C\0V\0|C\0X\0|C\0Y\0|C\0Z\0|D\0E\0|D\0J\0|D\0K\0|D\0M\0|D\0O\0|D\0Z\0|E\0C\0|E\0D\0U\0|E\0E\0|E\0G\0|E\0H\0|E\0R\0|E\0S\0|E\0T\0|E\0U\0|F\0I\0|F\0J\0|F\0K\0|F\0M\0|F\0O\0|F\0R\0|G\0A\0|G\0B\0|G\0D\0|G\0E\0|G\0F\0|G\0G\0|G\0H\0|G\0I\0|G\0L\0|G\0M\0|G\0N\0|G\0O\0V\0|G\0P\0|G\0Q\0|G\0R\0|G\0S\0|G\0T\0|G\0U\0|G\0W\0|G\0Y\0|H\0K\0|H\0M\0|H\0N\0|H\0R\0|H\0T\0|H\0U\0|I\0D\0|I\0E\0|I\0L\0|I\0M\0|I\0N\0|I\0N\0F\0O\0|I\0N\0T\0|I\0O\0|I\0Q\0|I\0R\0|I\0S\0|I\0T\0|J\0E\0|J\0M\0|J\0O\0|J\0O\0B\0S\0|J\0P\0|K\0E\0|K\0G\0|K\0H\0|K\0I\0|K\0M\0|K\0N\0|K\0P\0|K\0R\0|K\0W\0|K\0Y\0|K\0Z\0|L\0A\0|L\0B\0|L\0C\0|L\0I\0|L\0K\0|L\0R\0|L\0S\0|L\0T\0|L\0U\0|L\0V\0|L\0Y\0|M\0A\0|M\0C\0|M\0D\0|M\0E\0|M\0F\0|M\0G\0|M\0H\0|M\0I\0L\0|M\0K\0|M\0L\0|M\0M\0|M\0N\0|M\0O\0|M\0O\0B\0I\0|M\0P\0|M\0Q\0|M\0R\0|M\0S\0|M\0T\0|M\0U\0|M\0U\0S\0E\0U\0M\0|M\0V\0|M\0W\0|M\0X\0|M\0Y\0|M\0Z\0|N\0A\0|N\0A\0M\0E\0|N\0C\0|N\0E\0|N\0E\0T\0|N\0F\0|N\0G\0|N\0I\0|N\0L\0|N\0O\0|N\0P\0|N\0R\0|N\0U\0|N\0Z\0|O\0M\0|O\0R\0G\0|P\0A\0|P\0E\0|P\0F\0|P\0G\0|P\0H\0|P\0K\0|P\0L\0|P\0M\0|P\0N\0|P\0R\0|P\0R\0O\0|P\0S\0|P\0T\0|P\0W\0|P\0Y\0|Q\0A\0|R\0E\0|R\0O\0|R\0S\0|R\0U\0|R\0W\0|S\0A\0|S\0B\0|S\0C\0|S\0D\0|S\0E\0|S\0G\0|S\0H\0|S\0I\0|S\0J\0|S\0K\0|S\0L\0|S\0M\0|S\0N\0|S\0O\0|S\0R\0|S\0T\0|S\0U\0|S\0V\0|S\0Y\0|S\0Z\0|T\0C\0|T\0D\0|T\0E\0L\0|T\0F\0|T\0G\0|T\0H\0|T\0J\0|T\0K\0|T\0L\0|T\0M\0|T\0N\0|T\0O\0|T\0P\0|T\0R\0|T\0R\0A\0V\0E\0L\0|T\0T\0|T\0V\0|T\0W\0|T\0Z\0|U\0A\0|U\0G\0|U\0K\0|U\0M\0|U\0S\0|U\0Y\0|U\0Z\0|V\0A\0|V\0C\0|V\0E\0|V\0G\0|V\0I\0|V\0N\0|V\0U\0|W\0F\0|W\0S\0|Y\0E\0|Y\0T\0|Y\0U\0|Z\0A\0|Z\0M\0|Z\0W\0)");
  //
  // patterns
  //

  const std::string EMAIL(ALNUM + "(\\.?[a-zA-Z0-9_%\\-+])+\\.?" + ALNUM + "@" + ALNUM + "(\\.?[a-zA-Z0-9_%\\-])+\\." + TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

  const std::string U_EMAIL(U_ALNUM + "((\\.\\z00)?([a-zA-Z0-9_%\\-+]\\z00))+(\\.\\z00)?" + U_ALNUM + "@\\z00" + U_ALNUM + "((\\.\\z00)?([a-zA-Z0-9_%\\-]\\z00))+(\\.\\z00)" + U_TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

//  const std::string U_EMAIL(U_ALNUM + "((\\.\\z00)?([a-zA-Z0-9_%\\-+]\\z00))+(\\.\\z00)?" + U_ALNUM + "@\\z00" + U_ALNUM + "((\\.\\z00)?([a-zA-Z0-9_%\\-]\\z00))+(\\.\\z00)" + U_TLD + "[^\\z41-\\z5A\\z61-\\z7A]\\z00");

//  const std::string U_EMAIL(U_ALNUM + "@\\z00");

//  const std::string U_EMAIL("b\\z00b\\z00");


//  const std::string U_EMAIL(U_ALNUM + "([a-zA-Z0-9_%\\-+]\\z00)+");

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

be_scan::artifact_t a(
                   "email",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context);
std::cout << "emailHitHandler push " << a.to_string() << std::endl;

    scanner_data->artifacts.push(be_scan::artifact_t(
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

    scanner_data->artifacts.push(be_scan::artifact_t(
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

  std::string add_email_regex(lw::lw_t& lw) {
    std::string status;
    status = lw.add_regex(EMAIL, "UTF-8", true, false, &emailHitHandler);
    if (status != "") return status;
    status = lw.add_regex(U_EMAIL, "UTF-8", true, false, &emailHitHandler);
//    status = lw.add_regex(U_EMAIL, "UTF-8", true, false, &emailUTF16LEHitHandler);
//    status = lw.add_regex(EMAIL, "UTF-16LE", true, false, &emailHitHandler);
//    status = lw.add_regex(EMAIL, "UTF-16LE", true, false, &emailUTF16LEHitHandler);
//    status = lw.add_regex(U_EMAIL, "UTF-8", true, false, &emailHitHandler);
    if (status != "") return status;
    return "";
  }
}

