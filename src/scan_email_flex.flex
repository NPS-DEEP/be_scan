/* Vim note: for C style language highlighting in Vim: ":setf c" */

/* The regex are from bulk_extractor.
 * For redefining YY_INPUT: Lex&yacc, 2'nd ed. p. 157.
 */

/* ******************** FLEX Definitions ******************** */
%{
#include "config.h"
#include <cstring>
#include "scan_email.hpp"
#include "flex_extra_parameters.hpp"
#include "flex_scan_exception.hpp"

#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-compare"
#ifdef HAVE_DIAGNOSTIC_EFFCPP
#pragma GCC diagnostic ignored "-Weffc++"
#endif
#ifdef HAVE_DIAGNOSTIC_DEPRECATED_REGISTER
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#endif

/* Needed for flex: */
#define ECHO {}                   /* Never echo anything */
#define YY_SKIP_YYWRAP            /* Never wrap */
#define YY_NO_INPUT

//#define YY_INPUT(buffer, result, max_size) (result = \
//            yyemail_get_extra(yyscanner)->get_input(buffer, max_size));

#define YY_FATAL_ERROR(msg) {throw be_scan::flex_scan_exception(msg);}

#define YY_EXTRA_TYPE be_scan::flex_extra_parameters_t*
YY_EXTRA_TYPE yyemail_get_extra(yyscan_t yyscanner);

/* pointer for buffer scanning */
YY_BUFFER_STATE bp;

%}

%option extra-type = "be_scan::flex_extra_parameters_t*"

%option noyywrap
%option 8bit
%option batch
%option case-insensitive
%option pointer
%option noyymore
%option prefix="yyemail_"

SPECIALS	[()<>@,;:\\".\[\]]
ATOMCHAR	([a-zA-Z0-9`~!#$%^&*\-_=+{}|?])
ATOM		({ATOMCHAR}{1,80})
INUM		(([0-9])|([0-9][0-9])|(1[0-9][0-9])|(2[0-4][0-9])|(25[0-5]))
HEX		([0-9a-f])
XPC		[ !#$%&'()*+,\-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\[\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~]
PC		[ !#$%&'()*+,\-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\[\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"]
ALNUM		[a-zA-Z0-9]
TLD		(AC|AD|AE|AERO|AF|AG|AI|AL|AM|AN|AO|AQ|AR|ARPA|AS|ASIA|AT|AU|AW|AX|AZ|BA|BB|BD|BE|BF|BG|BH|BI|BIZ|BJ|BL|BM|BN|BO|BR|BS|BT|BV|BW|BY|BZ|CA|CAT|CC|CD|CF|CG|CH|CI|CK|CL|CM|CN|CO|COM|COOP|CR|CU|CV|CX|CY|CZ|DE|DJ|DK|DM|DO|DZ|EC|EDU|EE|EG|EH|ER|ES|ET|EU|FI|FJ|FK|FM|FO|FR|GA|GB|GD|GE|GF|GG|GH|GI|GL|GM|GN|GOV|GP|GQ|GR|GS|GT|GU|GW|GY|HK|HM|HN|HR|HT|HU|ID|IE|IL|IM|IN|INFO|INT|IO|IQ|IR|IS|IT|JE|JM|JO|JOBS|JP|KE|KG|KH|KI|KM|KN|KP|KR|KW|KY|KZ|LA|LB|LC|LI|LK|LR|LS|LT|LU|LV|LY|MA|MC|MD|ME|MF|MG|MH|MIL|MK|ML|MM|MN|MO|MOBI|MP|MQ|MR|MS|MT|MU|MUSEUM|MV|MW|MX|MY|MZ|NA|NAME|NC|NE|NET|NF|NG|NI|NL|NO|NP|NR|NU|NZ|OM|ORG|PA|PE|PF|PG|PH|PK|PL|PM|PN|PR|PRO|PS|PT|PW|PY|QA|RE|RO|RS|RU|RW|SA|SB|SC|SD|SE|SG|SH|SI|SJ|SK|SL|SM|SN|SO|SR|ST|SU|SV|SY|SZ|TC|TD|TEL|TF|TG|TH|TJ|TK|TL|TM|TN|TO|TP|TR|TRAVEL|TT|TV|TW|TZ|UA|UG|UK|UM|US|UY|UZ|VA|VC|VE|VG|VI|VN|VU|WF|WS|YE|YT|YU|ZA|ZM|ZW)


DOMAINREF	{ATOM}
SUBDOMAIN	{DOMAINREF}
DOMAIN		({SUBDOMAIN}[.])*{SUBDOMAIN}
QTEXT		[a-zA-Z0-9`~!@#$%^&*()_\-+=\[\]{}\\|;:',.<>/?]
QUOTEDSTRING	["]{QTEXT}*["]
WORD		({ATOM})|({QUOTEDSTRING})
LOCALPART	{WORD}([.]{WORD})*
ADDRSPEC	{LOCALPART}@{DOMAIN}
MAILBOX		{ADDRSPEC}

EMAIL	{ALNUM}[a-zA-Z0-9._%\-+]{1,128}{ALNUM}@{ALNUM}[a-zA-Z0-9._%\-]{1,128}\.{TLD}

/* ******************** FLEX Rules ******************** */
%%


.|\n { 
    ++yyextra->flex_offset;
}

{EMAIL}/[^a-zA-Z]	{
    yyextra->flex_size = yyleng;
    return 0;
}

%%
/* ******************** FLEX User Code ******************** */
// allocate space for scanner, initialize it, and give it visibility
// to flex_extra_parameters
void be_scan::scan_email_t::flex_init() {
  yyscan_t scanner;
  flex_extra_parameters.scanner = &scanner;
  yylex_init(&scanner);
  yyset_extra(&flex_extra_parameters, &scanner);
}

// scan buffer, leaving any result in flex_extra_parameters
void be_scan::scan_email_t::flex_scan(const std::string& buffer) {
  // copy buffer into flex
  bp = yy_scan_bytes(buffer.c_str(), buffer.size(),
                     flex_extra_parameters.scanner);
  yy_switch_to_buffer(bp, flex_extra_parameters.scanner);

  // initialize the flex offset
  flex_extra_parameters.flex_offset = 0;
  flex_extra_parameters.flex_size = 0;

  // scan
  try {
    yylex(flex_extra_parameters.scanner);
  } catch (be_scan::flex_scan_exception *e) {
    // warn to stderr, return no match, and keep going
    std::cerr << "email flex scan error: " << std::string(e->what());
    delete e;
  }

  // delete the flex buffer
  yy_delete_buffer(bp, flex_extra_parameters.scanner);
}

void be_scan::scan_email_t::flex_close() {
  yylex_destroy(flex_extra_parameters.scanner);
  (void)yyunput;			// avoids defined but not used
}

