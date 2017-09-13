%module be_scan_jni
%include "std_string.i"
%include "various.i"
%include "stdint.i"

// use Java camelCase instead of underscore_case
// http://stackoverflow.com/questions/8440115/how-to-rename-swig-generated-proxy-java-classes-created-from-c-structures/8444962#comment10444840_8444962
%rename (availableScanners) available_scanners;
%rename (ScanEngine) scan_engine_t;
%rename (Scanner) scanner_t;
%rename (scanSetup) scan_setup;
%rename (scanFinal) scan_final;
%rename (scanStream) scan_stream;
%rename (scanFenceFinal) scan_fence_final;

%rename (Artifact) artifact_t;
%rename (Artifacts) artifacts_t;
%rename (artifactClass) artifact_class;
%rename (streamName) stream_name;
%rename (recursionPrefix) recursion_prefix;
%rename (toString) to_string;
%rename (Uncompressed) uncompressed_t;
%rename (Uncompressor) uncompressor_t;
%rename (badAlloc) bad_alloc;

// C Strings need to return byte[] with \0 bytes intact.
// void javaArtifact(std::string&)
%typemap(jtype) void javaArtifact "byte[]"
%typemap(jstype) void javaArtifact "byte[]"
%typemap(jni) void javaArtifact "jbyteArray"
%typemap(javaout) void javaArtifact { return $jnicall; }
%typemap(in, numinputs=0) std::string& java_artifact (std::string temp) "$1=&temp;"
%typemap(argout) std::string& java_artifact {
  $result = JCALL1(NewByteArray, jenv, $1->size());
  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1->size(),
         (const jbyte*)$1->c_str());
}

// void javaContext(std::string&)
%typemap(jtype) void javaContext "byte[]"
%typemap(jstype) void javaContext "byte[]"
%typemap(jni) void javaContext "jbyteArray"
%typemap(javaout) void javaContext { return $jnicall; }
%typemap(in, numinputs=0) std::string& java_context (std::string temp) "$1=&temp;"
%typemap(argout) std::string& java_context {
  $result = JCALL1(NewByteArray, jenv, $1->size());
  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1->size(),
         (const jbyte*)$1->c_str());
}

// void javaBuffer(std::string&)
%typemap(jtype) void javaBuffer "byte[]"
%typemap(jstype) void javaBuffer "byte[]"
%typemap(jni) void javaBuffer "jbyteArray"
%typemap(javaout) void javaBuffer { return $jnicall; }
%typemap(in, numinputs=0) std::string& java_buffer (std::string temp) "$1=&temp;"
%typemap(argout) std::string& java_buffer {
  $result = JCALL1(NewByteArray, jenv, $1->size());
  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1->size(),
         (const jbyte*)$1->c_str());
}

//// http://stackoverflow.com/questions/33504659/passing-byte-from-java-to-c
//%apply char *BYTE { char *buffer };

// not compatible with SWIG 1.3.40
//http://stackoverflow.com/questions/12497175/correct-way-to-interact-with-arrays-using-swig
%apply(char *STRING, size_t LENGTH) { (const char * const previous_buffer, size_t previous_buffer_size) };
%apply(char *STRING, size_t LENGTH) { (const char * const buffer, size_t buffer_size) };

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"

