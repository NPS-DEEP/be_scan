%module be_scan_jni
%include "std_string.i"
%include "various.i"
%include "stdint.i"

// http://stackoverflow.com/questions/33504659/passing-byte-from-java-to-c
%apply char *BYTE { char *buffer };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"

