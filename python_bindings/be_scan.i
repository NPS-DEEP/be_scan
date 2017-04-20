%module be_scan
%include "std_string.i"
%include "stdint.i"
%include "std_set.i"
%include "std_pair.i"

%apply std::string& OUTPUT { std::string& loopback_buffer };

// to protect the buffer and to suppress a warning, do not offer
// visibility to artifact.new_buffer
%ignore be_scan::artifact_t::new_buffer;

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"
