/* Vim note: for C style language highlighting in Vim: ":setf c" */

/* This code is adapted from bulk_extractor.
 * For extra_data: http://sector7.xray.aps.anl.gov/~dohnarms/programming/flex/html/Extra-Data.html
 * For redefining YY_INPUT: Lex&yacc, 2'nd ed. p. 157.
 */

/* ******************** FLEX Definitions ******************** */
%{
#include "config.h"
#include <cstring>
#include "flex_scan_parameters.hpp"
#include "flex_scan_exception.hpp"
#include "scanners.hpp"

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

#define YY_INPUT(buffer, result, max_size) (result = \
            yyemail_get_extra(yyscanner)->get_input(buffer, max_size));

#define YY_FATAL_ERROR(msg) {throw be_scan::flex_scan_exception(msg);}

#define YY_EXTRA_TYPE be_scan::flex_scan_parameters_t*
YY_EXTRA_TYPE yyemail_get_extra(yyscan_t yyscanner);


/* Address some common false positives in email scanner */
inline bool validate_email(const char *email)
{
    if(strstr(email,"..")) return false;
    return true;
}

%}

%option extra-type = "be_scan::flex_scan_parameters_t*"

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


U_TLD1		(A\0C\0|A\0D\0|A\0E\0|A\0E\0R\0O\0|A\0F\0|A\0G\0|A\0I\0|A\0L\0|A\0M\0|A\0N\0|A\0O\0|A\0Q\0|A\0R\0|A\0R\0P\0A\0|A\0S\0|A\0S\0I\0A\0|A\0T\0|A\0U\0|A\0W\0|A\0X\0|A\0Z\0|B\0A\0|B\0B\0|B\0D\0|B\0E\0|B\0F\0|B\0G\0|B\0H\0|B\0I\0|B\0I\0Z\0|B\0J\0|B\0L\0|B\0M\0|B\0N\0|B\0O\0|B\0R\0|B\0S\0|B\0T\0|B\0V\0|B\0W\0|B\0Y\0|B\0Z\0|C\0A\0|C\0A\0T\0|C\0C\0|C\0D\0|C\0F\0|C\0G\0|C\0H\0|C\0I\0|C\0K\0|C\0L\0|C\0M\0|C\0N\0|C\0O\0|C\0O\0M\0|C\0O\0O\0P\0|C\0R\0|C\0U\0|C\0V\0|C\0X\0|C\0Y\0|C\0Z\0)
U_TLD2		(D\0E\0|D\0J\0|D\0K\0|D\0M\0|D\0O\0|D\0Z\0|E\0C\0|E\0D\0U\0|E\0E\0|E\0G\0|E\0H\0|E\0R\0|E\0S\0|E\0T\0|E\0U\0|F\0I\0|F\0J\0|F\0K\0|F\0M\0|F\0O\0|F\0R\0|G\0A\0|G\0B\0|G\0D\0|G\0E\0|G\0F\0|G\0G\0|G\0H\0|G\0I\0|G\0L\0|G\0M\0|G\0N\0|G\0O\0V\0|G\0P\0|G\0Q\0|G\0R\0|G\0S\0|G\0T\0|G\0U\0|G\0W\0|G\0Y\0|H\0K\0|H\0M\0|H\0N\0|H\0R\0|H\0T\0|H\0U\0|I\0D\0|I\0E\0|I\0L\0|I\0M\0|I\0N\0|I\0N\0F\0O\0|I\0N\0T\0|I\0O\0|I\0Q\0|I\0R\0|I\0S\0|I\0T\0|J\0E\0|J\0M\0|J\0O\0|J\0O\0B\0S\0|J\0P\0|K\0E\0|K\0G\0|K\0H\0|K\0I\0|K\0M\0|K\0N\0|K\0P\0|K\0R\0|K\0W\0|K\0Y\0|K\0Z\0)
U_TLD3		(L\0A\0|L\0B\0|L\0C\0|L\0I\0|L\0K\0|L\0R\0|L\0S\0|L\0T\0|L\0U\0|L\0V\0|L\0Y\0|M\0A\0|M\0C\0|M\0D\0|M\0E\0|M\0F\0|M\0G\0|M\0H\0|M\0I\0L\0|M\0K\0|M\0L\0|M\0M\0|M\0N\0|M\0O\0|M\0O\0B\0I\0|M\0P\0|M\0Q\0|M\0R\0|M\0S\0|M\0T\0|M\0U\0|M\0U\0S\0E\0U\0M\0|M\0V\0|M\0W\0|M\0X\0|M\0Y\0|M\0Z\0|N\0A\0|N\0A\0M\0E\0|N\0C\0|N\0E\0|N\0E\0T\0|N\0F\0|N\0G\0|N\0I\0|N\0L\0|N\0O\0|N\0P\0|N\0R\0|N\0U\0|N\0Z\0|O\0M\0|O\0R\0G\0|P\0A\0|P\0E\0|P\0F\0|P\0G\0|P\0H\0|P\0K\0|P\0L\0|P\0M\0|P\0N\0|P\0R\0|P\0R\0O\0|P\0S\0|P\0T\0|P\0W\0|P\0Y\0)
U_TLD4		(Q\0A\0|R\0E\0|R\0O\0|R\0S\0|R\0U\0|R\0W\0|S\0A\0|S\0B\0|S\0C\0|S\0D\0|S\0E\0|S\0G\0|S\0H\0|S\0I\0|S\0J\0|S\0K\0|S\0L\0|S\0M\0|S\0N\0|S\0O\0|S\0R\0|S\0T\0|S\0U\0|S\0V\0|S\0Y\0|S\0Z\0|T\0C\0|T\0D\0|T\0E\0L\0|T\0F\0|T\0G\0|T\0H\0|T\0J\0|T\0K\0|T\0L\0|T\0M\0|T\0N\0|T\0O\0|T\0P\0|T\0R\0|T\0R\0A\0V\0E\0L\0|T\0T\0|T\0V\0|T\0W\0|T\0Z\0|U\0A\0|U\0G\0|U\0K\0|U\0M\0|U\0S\0|U\0Y\0|U\0Z\0|V\0A\0|V\0C\0|V\0E\0|V\0G\0|V\0I\0|V\0N\0|V\0U\0|W\0F\0|W\0S\0|Y\0E\0|Y\0T\0|Y\0U\0|Z\0A\0|Z\0M\0|Z\0W\0)


/* ******************** FLEX Rules ******************** */
%%

{EMAIL}/[^a-zA-Z]	{
    if(validate_email(yytext)){
        yyextra->db->write(yyextra->filename,
                           yyextra->artifact_index + yyextra->file_offset,
                           yyextra->recursion_path,
                           "email",
                           std::string(yytext),
                           yyextra->current_context(yyleng));
    }
    yyextra->artifact_index += yyleng; 
}

[a-zA-Z0-9]\0([a-zA-Z0-9._%\-+]\0){1,128}@\0([a-zA-Z0-9._%\-]\0){1,128}\.\0({U_TLD1}|{U_TLD2}|{U_TLD3}|{U_TLD4})/[^a-zA-Z]|([^][^\0])	{
    /* fix this to remove \0 chars */
    if(validate_email(yytext)){
        yyextra->db->write(yyextra->filename,
                           yyextra->artifact_index + yyextra->file_offset,
                           yyextra->recursion_path,
                           "email",
                           std::string(yytext),
                           yyextra->current_context(yyleng));
    }
    (yyextra->artifact_index) += yyleng; 
}

.|\n { 
    /**
     * The no-match rule. VERY IMPORTANT!
     * If we are beyond the end of the margin, call it quits.
     */
    ++(yyextra->artifact_index);
}


%%
/* ******************** FLEX User Code ******************** */

std::string be_scan::scan_email(const std::string& filename,
                                const size_t file_offset,
                                const std::string& recursion_path,
                                const char* const buffer,
                                const size_t size,
                                be_scan::db_t* const db) {

  be_scan::flex_scan_parameters_t flex_scan_parameters(filename, file_offset,
                                         recursion_path, buffer, size, db);
  yyscan_t scanner;
  yyemail_lex_init(&scanner);
  yyemail_set_extra(&flex_scan_parameters, scanner);
  std::string status = "";
  try {
    yyemail_lex(scanner);
  } catch (be_scan::flex_scan_exception *e) {
    status = std::string(e->what());
    delete e;
  }
  yyemail_lex_destroy(scanner);
  (void)yyunput;			// avoids defined but not used
  return status;
}

