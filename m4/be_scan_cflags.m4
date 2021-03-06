################################################################
#
# Enable all the compiler debugging we can find
# Simson L. Garfinkel
#
# This is originally from PhotoRec, but modified substantially by Simson
# Figure out which flags we can use with the compiler. 
#
# These I don't like:
# -Wdeclaration-after-statement -Wconversion
# doesn't work: -Wunreachable-code 
# causes configure to crash on gcc-4.2.1: -Wsign-compare-Winline 
# causes warnings with unistd.h:  -Wnested-externs 
# Just causes too much annoyance: -Wmissing-format-attribute 
#
################################################################
# bda - Changed to set BE_SCAN_CFLAGS instead of CFLAGS
#       and BE_SCAN_CXXFLAGS instead of CXXFLAGS
#       to allow visibility to foo_CFLAGS

# First, see if we are using CLANG
using_clang=no
if (g++ --version 2>&1 | grep clang > /dev/null) ; 
then 
   AC_MSG_NOTICE([g++ is really clang++])
   using_clang=yes
fi
if test x$CXX == "xclang++" ; then
   using_clang=yes
fi



# Check GCC
C_FLAGS_TO_TEST="-MD -Wpointer-arith -Wmissing-declarations -Wmissing-prototypes \
    -Wshadow -Wwrite-strings -Wcast-align -Waggregate-return \
    -Wbad-function-cast -Wcast-qual -Wundef -Wredundant-decls -Wdisabled-optimization \
    -Wfloat-equal -Wmultichar -Wc++-compat -Wmissing-noreturn \
    -Wmissing-format-attribute -Wundef -Weffc++ -Wsuggest-attribute=noreturn \
    -Wdeprecated-register"

if test x"${mingw}" != "xyes" ; then
  # add the warnings we do not want to do on mingw
  C_FLAGS_TO_TEST="$C_FLAGS_TO_TEST -Wall -Wstrict-prototypes"
fi

if test $using_clang == "no" ; then
  # -Wstrict-null-sentinel is not supported under clang
  CXX_FLAGS_TO_TEST="$CXX_FLAGS_TO_TEST -Wstrict-null-sentinel"
fi



echo "C flags to test: $C_FLAGS_TO_TEST"

for option in $C_FLAGS_TO_TEST
do
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $option"
  AC_MSG_CHECKING([whether gcc understands $option])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],
      [has_option=yes
       BE_SCAN_CFLAGS="$BE_SCAN_CFLAGS $option"

       # Some attributes may need to be conditionally suppressed.
       # Add more as needed.
       case "$option" in
         -Wmissing-format-attribute)
           AC_DEFINE(HAVE_MISSING_FORMAT_ATTRIBUTE_WARNING,1,
             [Indicates that the compiler supports -Wmissing-format-attribute])
           ;;
         -Wshadow)
           AC_DEFINE(HAVE_DIAGNOSTIC_SHADOW,1,
             [Indicates that the compiler supports -Wshadow])
           ;;
         -Wundef)
           AC_DEFINE(HAVE_DIAGNOSTIC_UNDEF,1,
             [Indicates that the compiler supports -Wundef])
           ;;
         -Wcast-qual)
           AC_DEFINE(HAVE_DIAGNOSTIC_CAST_QUAL,1,
             [Indicates that the compiler supports -Wcast-qual])
           ;;
         -Weffc++)
           AC_DEFINE(HAVE_DIAGNOSTIC_EFFCPP,1,
             [Indicates that the compiler supports -Weffc++])
           ;;
         -Wsuggest-attribute=noreturn)
           AC_DEFINE(HAVE_DIAGNOSTIC_SUGGEST_ATTRIBUTE,1,
             [Indicates that the compiler supports -Wsuggest-attribute=noreturn])
           ;;
         -Wdeprecated-register)
           AC_DEFINE(HAVE_DIAGNOSTIC_DEPRECATED_REGISTER,1,
             [Indicates that the compiler supports -Wdeprecated-register])
           ;;
       esac
      ],
      [has_option=no])
  AC_MSG_RESULT($has_option)
  unset has_option
  CFLAGS="$SAVE_CFLAGS"
  unset SAVE_CFLAGS

done
unset option


# Check G++
# We don't use these warnings:
# -Waggregate-return -- aggregate returns are GOOD; they simplify code design
# We can use these warnings after ZLIB gets upgraded:
# -Wundef  --- causes problems with zlib
# -Wcast-qual 
# -Wmissing-format-attribute  --- Just too annoying
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS([string])
CXX_FLAGS_TO_TEST="-Wall -MD -D_FORTIFY_SOURCE=2 -Wpointer-arith \
    -Wshadow -Wwrite-strings -Wcast-align  \
    -Wredundant-decls -Wdisabled-optimization \
    -Wfloat-equal -Wmultichar -Wmissing-noreturn \
    -Woverloaded-virtual -Wsign-promo \
    -funit-at-a-time"

if test x"${mingw}" != "xyes" ; then
  # add the warnings we don't want to do on mingw
  CXX_FLAGS_TO_TEST="$CXX_FLAGS_TO_TEST  -Weffc++"
fi

echo "C++ flags to test: $CXX_FLAGS_TO_TEST"

for option in $CXX_FLAGS_TO_TEST
do
  SAVE_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $option"
  AC_MSG_CHECKING([whether g++ understands $option])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],
      [has_option=yes; BE_SCAN_CXXFLAGS="$BE_SCAN_CXXFLAGS $option"],
      [has_option=no])
  AC_MSG_RESULT($has_option)
  unset has_option
  CXXFLAGS="$SAVE_CXXFLAGS"
  unset SAVE_CXXFLAGS
done
unset option
AC_LANG_POP()    

AC_SUBST(BE_SCAN_CFLAGS)
AC_SUBST(BE_SCAN_CXXFLAGS)

