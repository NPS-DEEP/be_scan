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
  // regex for email
  // ************************************************************

  const std::string ALNUM("[a-zA-Z0-9]");

  const std::string TLD("(AC|AD|AE|AERO|AF|AG|AI|AL|AM|AN|AO|AQ|AR|ARPA|AS|ASIA|AT|AU|AW|AX|AZ|BA|BB|BD|BE|BF|BG|BH|BI|BIZ|BJ|BL|BM|BN|BO|BR|BS|BT|BV|BW|BY|BZ|CA|CAT|CC|CD|CF|CG|CH|CI|CK|CL|CM|CN|CO|COM|COOP|CR|CU|CV|CX|CY|CZ|DE|DJ|DK|DM|DO|DZ|EC|EDU|EE|EG|EH|ER|ES|ET|EU|FI|FJ|FK|FM|FO|FR|GA|GB|GD|GE|GF|GG|GH|GI|GL|GM|GN|GOV|GP|GQ|GR|GS|GT|GU|GW|GY|HK|HM|HN|HR|HT|HU|ID|IE|IL|IM|IN|INFO|INT|IO|IQ|IR|IS|IT|JE|JM|JO|JOBS|JP|KE|KG|KH|KI|KM|KN|KP|KR|KW|KY|KZ|LA|LB|LC|LI|LK|LR|LS|LT|LU|LV|LY|MA|MC|MD|ME|MF|MG|MH|MIL|MK|ML|MM|MN|MO|MOBI|MP|MQ|MR|MS|MT|MU|MUSEUM|MV|MW|MX|MY|MZ|NA|NAME|NC|NE|NET|NF|NG|NI|NL|NO|NP|NR|NU|NZ|OM|ORG|PA|PE|PF|PG|PH|PK|PL|PM|PN|PR|PRO|PS|PT|PW|PY|QA|RE|RO|RS|RU|RW|SA|SB|SC|SD|SE|SG|SH|SI|SJ|SK|SL|SM|SN|SO|SR|ST|SU|SV|SY|SZ|TC|TD|TEL|TF|TG|TH|TJ|TK|TL|TM|TN|TO|TP|TR|TRAVEL|TT|TV|TW|TZ|UA|UG|UK|UM|US|UY|UZ|VA|VC|VE|VG|VI|VN|VU|WF|WS|YE|YT|YU|ZA|ZM|ZW)");

// unbounded match length is inefficient, use bounded match length instead
//  const std::string EMAIL(ALNUM + "(\\.?[a-zA-Z0-9_%\\-+])+\\.?" + ALNUM + "@" + ALNUM + "(\\.?[a-zA-Z0-9_%\\-])+\\." + TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

  // bounded match length
  const std::string EMAIL(ALNUM + "(\\.?[a-zA-Z0-9_%\\-+]){1,64}\\.?" + ALNUM + "@" + ALNUM + "(\\.?[a-zA-Z0-9_%\\-]){1,64}\\." + TLD + "[^\\z41-\\z5A\\z61-\\z7A]");

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

    scanner_data->artifacts->put(be_scan::artifact_t(
                   "email",
                   scanner_data->stream_name,
                   scanner_data->recursion_prefix,
                   start,
                   artifact,
                   context));
  }

  // ************************************************************
  // add regex
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

