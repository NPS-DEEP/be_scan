%module be_scan_jni
%include "std_string.i"
%include "various.i"
%include "stdint.i"

// use Java camelCase instead of underscore_case
// http://stackoverflow.com/questions/8440115/how-to-rename-swig-generated-proxy-java-classes-created-from-c-structures/8444962#comment10444840_8444962
%rename (availableScanners) available_scanners;
%rename (Artifact) artifact_t;
//%rename (BEBuffer) be_buffer_t;
%rename (artifactClass) artifact_class;
%rename (bufferOffset) buffer_offset;
%rename (BEScan) be_scan_t;
%rename (isInitialized) is_initialized;
%rename (nextArtifact) next_artifact;

// http://stackoverflow.com/questions/33504659/passing-byte-from-java-to-c
%apply char *BYTE { char *p_buffer };

// not compatible with SWIG 1.3.40
////http://stackoverflow.com/questions/12497175/correct-way-to-interact-with-arrays-using-swig
//%apply(char *STRING, size_t LENGTH) { (const char * const p_buffer, size_t p_buffer_size) };

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

// void javaTestLoopback(std::string&)
%typemap(jtype) void javaTestLoopback "byte[]"
%typemap(jstype) void javaTestLoopback "byte[]"
%typemap(jni) void javaTestLoopback "jbyteArray"
%typemap(javaout) void javaTestLoopback { return $jnicall; }
%typemap(in, numinputs=0) std::string& java_test_loopback (std::string temp) "$1=&temp;"
%typemap(argout) std::string& java_test_loopback {
  $result = JCALL1(NewByteArray, jenv, $1->size());
  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1->size(),
         (const jbyte*)$1->c_str());
}

//// http://stackoverflow.com/questions/12192624/swig-convert-return-type-stdstringbinary-to-java-byte
//%typemap(jtype) void foo "byte[]"
//%typemap(jstype) void foo "byte[]"
//%typemap(jni) void foo "jbyteArray"
//%typemap(javaout) void foo { return $jnicall; }
//%typemap(in, numinputs=0) std::string& z (std::string temp) "$1=&temp;"
//%typemap(argout) std::string& z {
//  $result = JCALL1(NewByteArray, jenv, $1->size());
//  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1->size(), (const jbyte*)$1->c_str());
//}
//
//%inline %{
//struct Bar {
//  void foo(std::string& z) {
//    std::string s;
//    s.push_back('H');
//    s.push_back('o');
//    s.push_back(0);
//    s.push_back('l');
//    s.push_back('a');
//    z = s;
//  }
//};
//%}

%{
#include "be_scan.hpp"
%}

%feature("autodoc", "1");

%include "be_scan.hpp"

