#                                  -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Order is largely irrevellant, although it must start with AC_INIT and end with AC_OUTPUT
# See http://autotoolset.sourceforge.net/tutorial.html
# and http://www.openismus.com/documents/linux/automake/automake.shtml

AC_PREREQ(2.57)

AC_INIT(be_scan, 0.1.0, github.com/NPS-DEEP/be_scan/issues)
AC_CONFIG_MACRO_DIR(m4)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 java_bindings/Makefile
                ])
#                 python_bindings/Makefile
#                 test/Makefile


# define CC and CXX as desired for alternate compilers
# for example "./configure CC=clang CXX=clang++"
#CC=clang
#CXX=clang++

AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([subdir-objects])
AC_PREFIX_PROGRAM(be_scan)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_DISABLE_STATIC

# use C++11 mode if available; HAVE_CXX11 is defined in config.h if so.  Don't
# use the GNU C++11 extensions for portability's sake (noext).
# https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx_11.html
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AX_CXX_COMPILE_STDCXX_11(noext, optional)

# Endian check is required for MD5 implementation
AC_C_BIGENDIAN

################################################################
## Includes
m4_include([m4/be_scan_cflags.m4])

################################################################
## establish default Python for tests and for SWIG
AM_PATH_PYTHON([2.7],, [:])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])
if test "$PYTHON" == : ; then
  AC_MSG_ERROR([Python is not available.])
fi

################################################################
## Linux
AC_DEFINE([__LINUX__],1,[Linux operating system functions])
CPPFLAGS="$CPPFLAGS -fPIC"

## SWIG Python
m4_include([m4/ax_pkg_swig.m4])
AX_PKG_SWIG(2.0.10, [], [ AC_MSG_NOTICE([SWIG is required to build API bindings]) ])
m4_include([m4/ax_python_devel.m4])
m4_include([m4/ax_swig_enable_cxxx.m4])
m4_include([m4/ax_swig_python.m4])
AX_SWIG_PYTHON
AX_SWIG_ENABLE_CXX

## SWIG Java
m4_include([m4/ax_jni_include_dir.m4])
AX_JNI_INCLUDE_DIR
for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
  do
          JNI_CPPFLAGS="$JNI_CPPFLAGS -I$JNI_INCLUDE_DIR"
  done

################################################################
## mcheck support
## enable memory analysis support for detecting memory leaks
## and detecting multiply deallocated memory resources
## see http://en.wikipedia.org/wiki/Mtrace
## here is the basic workflow:
##   1. example usage and recompile: "./configure --enable-mcheck"
##   2. export the environment variable for the output file,
##        e.g. "export MALLOC_TRACE=~/malloc_outfile.txt"
##   3. improve readability of malloc_outfile.txt using mtrace,
##        e.g. "mtrace /usr/local/bin/bulk_extractor ~/malloc_outfile.txt
AC_ARG_ENABLE([mcheck], AS_HELP_STRING([--enable-mcheck], \
              [enable memory analysis support for detecting memory leaks \
               and detecting multiply deallocated memory resources]), \
              AC_DEFINE(HAVE_MCHECK, 1, \
              [define the output file using global variable MALLOC_TRACE, \
               for example "MALLOC_TRACE=./mcheck.out; export MALLOC_TRACE". \
               Then post-process unreadable file mcheck.out using the mtrace command line tool, \
               for example "mtrace /usr/local/bin/bulk_extractor mcheck.out".  \
               Unfortunately, only addresses with written code are looked up, \
               in otherwords, use of malloc is looked up, but use of new is not.]))

################################################################
## AddressSanitizer support
AC_ARG_ENABLE([address-sanitizer], AS_HELP_STRING([--enable-address-sanitizer], \
              [enabled AddressSanitizer support for detecting a wide variety of \
               memory allocation and deallocation errors]), \
              [AC_DEFINE(HAVE_ADDRESS_SANITIZER, 1, [enable AddressSanitizer])
              CFLAGS="$CFLAGS -fsanitize=address"
              CXXFLAGS="$CXXFLAGS -fsanitize=address"
              AC_TRY_COMPILE([],[const int i=0;],[AC_MSG_NOTICE([Address Sanitizer Enabled])],
                                                 [AC_MSG_ERROR([Address Sanitizer not available])])
              ])


################################################################
# support for mallinfo for malloc usage reporting
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_LIB([stdc++],[mallinfo],
  [AC_DEFINE(HAVE_MALLINFO,1,[define 1 if stdc++ supports mallinfo])]
  ,[])

# Specific functions, may not be required
AC_TYPE_INT64_T
AC_SYS_LARGEFILE
AC_SYS_POSIX_TERMIOS

################################################################
## zlib is required for zip, gzip recursive decompression
AC_CHECK_LIB([z], [inflate],[], [AC_MSG_ERROR([Could not find zlib library])])

################################################################
## OpenSSL Support is required for md5 hash
AC_CHECK_HEADERS([openssl/aes.h openssl/bio.h openssl/evp.h openssl/hmac.h openssl/md5.h openssl/pem.h openssl/rand.h openssl/rsa.h openssl/sha.h openssl/pem.h openssl/x509.h])
# OpenSSL has been installed under at least two different names...
# note that crypto requires zlib
AC_CHECK_LIB([crypto],[EVP_get_digestbyname],[],[AC_MSG_ERROR([Could not find crypto library])])	
AC_CHECK_LIB([ssl],[SSL_library_init],[],[AC_MSG_ERROR([Could not find ssl library])])
AC_CHECK_FUNCS([EVP_get_digestbyname],,
	AC_MSG_ERROR([SSL/OpenSSL support required]))

################################################################
# Should we disable optimization?
AC_ARG_WITH([opt], AC_HELP_STRING([--without-opt], [Drop all -O C flags]))
# Or maybe just tone it down a bit?
AC_ARG_WITH([o3], AC_HELP_STRING([--without-o3], [Do not force O3 optimization; use default level]))

################################################################
## regex support
## there are several options
## tre is better than regex
AC_CHECK_LIB([stdc++],[main])
AC_CHECK_LIB([sicudt],[main])
AC_CHECK_HEADER([unicode/ucnv.h])
AC_SEARCH_LIBS([uset_close], [sicuuc])


################################################################
## libtool required for preparing the libbe_scan.so library
AC_CHECK_PROG(has_libtool, libtool, true, false)
if test x"$has_libtool" == "xfalse" ; then
  AC_MSG_ERROR([libtool is required, please install libtool such as The GNU Portable Library Tool])
fi

################################################################
## diagnostic defines
AC_TRY_COMPILE([#pragma GCC diagnostic ignored "-Wshadow"],[int a=3;],
  [AC_DEFINE(HAVE_DIAGNOSTIC_SHADOW,1,[define 1 if GCC supports -Wshadow])]
)
AC_TRY_COMPILE([#pragma GCC diagnostic ignored "-Wundef"],[int a=3;],
  [AC_DEFINE(HAVE_DIAGNOSTIC_UNDEF,1,[define 1 if GCC supports -Wundef])]
)
AC_TRY_COMPILE([#pragma GCC diagnostic ignored "-Wcast-qual"],[int a=3;],
  [AC_DEFINE(HAVE_DIAGNOSTIC_CAST_QUAL,1,[define 1 if GCC supports -Wcast-qual])]
)
AC_TRY_COMPILE([#pragma GCC diagnostic ignored "-Weffcpp"],[int a=3;],
  [AC_DEFINE(HAVE_DIAGNOSTIC_EFFCPP,1,[define 1 if GCC supports -Weffc++])]
)
AC_TRY_COMPILE([#pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"],[int a=3;],
  [AC_DEFINE(HAVE_DIAGNOSTIC_SUGGEST_ATTRIBUTE,1,[define 1 if GCC supports -Wsuggest-attribute=noreturn])]
)

################################################################
# do we have a broken strchr?
# This prevents errors in base64_forensics.cpp
AC_LANG_PUSH(C++)
AC_TRY_COMPILE([#include <string.h>],
               [static const char Base64[] = "ABCDEFGH";int ch=3;char *pos = strchr(Base64, ch);],
               conforming_strchr=yes,
               conforming_strchr=no)
if test "$conforming_strchr" = yes; then
  AC_MSG_NOTICE([strchr is conforming])
  AC_DEFINE(HAVE_CONFORMING_STRCHR, 1,[define to 1 if strchr takes const char*])
else
  AC_MSG_NOTICE([strchr is not conforming])
fi
AC_LANG_POP()

#################################################################
# Get the GIT commit into the GIT_COMMIT variable
AC_CHECK_PROG([git],[git],[yes],[no])
AM_CONDITIONAL([FOUND_GIT],[test "x$git" = xyes])
AM_COND_IF([FOUND_GIT],
        [GIT_COMMIT=`git describe --dirty --always`
         AC_MSG_NOTICE([git commit $GIT_COMMIT])],
        [AC_MSG_WARN([git not found])])

################################################################
############## drop optimization flags and add -g if requested ################
if test x"${AFF_NOOPT}" != "x" ; then
   with_noopt="yes";
fi

if test "${with_opt}" = "no" ; then
  CFLAGS=`echo   -g "$CFLAGS"   | sed s/-O[[0-9]]//`             # note the double quoting!
  CXXFLAGS=`echo -g "$CXXFLAGS" | sed s/-O[[0-9]]//`
else
  # If we are not stripping the optimizer, add fortify source
  CFLAGS="  $CFLAGS   -D_FORTIFY_SOURCE=2 "
  CXXFLAGS="$CXXFLAGS -D_FORTIFY_SOURCE=2 "

  # and increase optimizer from -O2 to -O3 if not explicitly forbidden
  if test "${with_o3}" != "no" ; then
      CFLAGS=`echo   -g "$CFLAGS"   | sed s/-O2/-O3/`             # note the double quoting!
      CXXFLAGS=`echo -g "$CXXFLAGS" | sed s/-O2/-O3/`
  fi
fi

AC_MSG_NOTICE([
*************************************
*************************************
  PACKAGE_NAME:     $PACKAGE_NAME
  PACKAGE_VERSION:  $PACKAGE_VERSION
  CC:               $CC
  CXX:              $CXX
  CPPFLAGS:         $CPPFLAGS
  CFLAGS:           $CFLAGS
  CXXFLAGS:         $CXXFLAGS
  LIBS:             $LIBS
  LDFLAGS:          $LDFLAGS

  PYTHON_VERSION:   $PYTHON_VERSION
])
  
AC_OUTPUT

## for git version
if test x"$GIT_COMMIT" != "x" ; then
  echo "#define GIT_COMMIT  \"$GIT_COMMIT\""  >> config.h
fi

