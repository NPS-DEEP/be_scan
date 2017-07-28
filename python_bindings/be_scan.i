%module be_scan
%include "std_string.i"
%include "stdint.i"

%apply(char *STRING, size_t LENGTH) { (const char * const previous_buffer, size_t previous_buffer_size) };
%apply(char *STRING, size_t LENGTH) { (const char * const buffer, size_t buffer_size) };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"
