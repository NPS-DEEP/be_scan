%module be_scan_jni
%include "std_string.i"
%include "stdint.i"

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"
