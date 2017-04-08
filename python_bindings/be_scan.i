%module be_scan
%include "std_string.i"
%include "stdint.i"
%include "std_set.i"
%include "std_pair.i"

%apply std::string& OUTPUT { std::string& loopback_buffer };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"
