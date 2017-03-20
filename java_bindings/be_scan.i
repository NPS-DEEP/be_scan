%module be_scan_jni
%include "std_string.i"
%include "various.i"
%include "stdint.i"

// use Java camelCase instead of underscore_case
// http://stackoverflow.com/questions/8440115/how-to-rename-swig-generated-proxy-java-classes-created-from-c-structures/8444962#comment10444840_8444962
%rename (BEScan) be_scan_t;

// http://stackoverflow.com/questions/33504659/passing-byte-from-java-to-c
%apply char *BYTE { char *buffer };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"

