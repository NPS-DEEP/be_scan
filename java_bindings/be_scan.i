%module be_scan_jni
%include "std_string.i"
%include "various.i"
%include "stdint.i"

// use Java camelCase instead of underscore_case
// http://stackoverflow.com/questions/8440115/how-to-rename-swig-generated-proxy-java-classes-created-from-c-structures/8444962#comment10444840_8444962
%rename (availableScanners) available_scanners;
%rename (ScanEngine) scan_engine_t;
%rename (Scanner) scanner_t;

// http://stackoverflow.com/questions/33504659/passing-byte-from-java-to-c
%apply char *BYTE { char *buffer };

// not compatible with SWIG 1.3.40
////http://stackoverflow.com/questions/12497175/correct-way-to-interact-with-arrays-using-swig
//%apply(char *STRING, size_t LENGTH) { (const char * const p_buffer, size_t p_buffer_size) };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"

