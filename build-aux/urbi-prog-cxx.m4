#
# urbi-prog-cxx.m4: This file is part of build-aux.
# Copyright (C) 2006-2009 Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])dnl
m4_pattern_forbid([^TC_])dnl

AC_PREREQ([2.60])

# _URBI_PROG_CXX_FLAVOR(CACHE-VAR)
# --------------------------------
# Compute $urbi_cv_cxx_flavor = gcc, gcc-apple, msvc, unknown.
m4_define([_URBI_PROG_CXX_FLAVOR],
[$1=unknown
case $GXX in
  (yes)
     case $($CXX --version | sed 1q) in
       (*"Apple Inc."*) $1=gcc-apple;;
       (*)              $1=gcc;;
     esac
     ;;
  (*)
AC_COMPILE_IFELSE([[
#ifndef _MSC_VER
# error This is not a Microsoft compiler
#endif
]],
                  [$1=msvc])
     ;;
esac
])


# URBI_PROG_CXX_FLAVOR
# ---------------------
# Compute CXX_FLAVOR = gcc, gcc-apple, msvc, unknown.
m4_define([URBI_PROG_CXX_FLAVOR],
[AC_CACHE_CHECK([the flavor of $CXX], [urbi_cv_cxx_flavor],
                [_URBI_PROG_CXX_FLAVOR([urbi_cv_cxx_flavor])])
CXX_FLAVOR=$urbi_cv_cxx_flavor
])


# URBI_PROG_CXX
# -------------
# Look for a C++ compiler, and pass interesting warning options.
# URBI_COMPILATION_MODE must be called after, since it needs to know
# the flavor.
AC_DEFUN([URBI_PROG_CXX],
[AC_BEFORE([$0], [URBI_COMPILATION_MODE])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
# Look for a C++ compiler.
AC_LANG_PUSH([C++])
AC_REQUIRE([AC_PROG_CXX])dnl

# Compute the flavor of this compiler.
URBI_PROG_CXX_FLAVOR

# Try to restrict the default visibility as much as possible.
case $CXX_FLAVOR in
  (msvc) # It accepts the option, but complains about it.
     ;;
  (*)
  # Use -Werror since using -fvisibility under MinGW is only a warning.
  # (The option is ignored anyway since this does not make sense under windows).
  TC_COMPILER_OPTION_IF([-Werror -fvisibility=hidden],
    [URBI_APPEND_CXXFLAGS([-fvisibility=hidden])
    TC_COMPILER_OPTION_IF([-fvisibility-inlines-hidden],
      [URBI_APPEND_CXXFLAGS([-fvisibility-inlines-hidden])])])
  ;;
esac

# Warn about violations of some of the style guidelines from Scott
# Meyers' "Effective C++" and "More Effective C++" books.
#TC_CXX_WARNINGS([-Weffc++])

# For the time being, we are not interested about deprecated
# headers such as hash_map.h.  We should use ext/hash_map.
TC_CXX_WARNINGS([-Wno-deprecated])

#		 [-Wcast-align],
#		 [-Wcast-qual],
#		 [-Wmissing-prototypes],   C only
#		 [-Wstrict-prototypes],    C only
#		 [-Wwrite-strings],
#		 [-Wbad-function-cast],    C only
#		 [-Wnested-externs],       C only
#		 [-Wmissing-declarations], C only
#		 [-Wold-style-cast],
# Use good warnings.
TC_CXX_WARNINGS([[-Wall],
		 [-W],
		 [-Woverloaded-virtual],
		 [-Wformat]])

# Pacify g++ on Boost Variants.
# TC_CXX_WARNINGS([[-Wno-shadow]])

# This is so that ICC no longer complain that
#
#  scantiger.ll(177): remark #111: statement is unreachable
#          break;
#          ^
#
#   ./../ast/seqexp.hh(36): remark #193:
#      zero used for undefined preprocessing identifier
#   #if STUDENT
#       ^
#
#    scantiger.cc(924): remark #279: controlling expression is constant
#          while ( 1 )             /* loops until end-of-file is reached */
#                  ^
#
#   ../../src/task/task.hh(38): remark #383:
#     value copied to temporary, reference to
#    temporary used
#           int key = 0, const std::string& deps = "");
#                                                  ^
#   /intel/compiler70/ia32/include/xstring(41): remark #444:
#      destructor for base class "std::_String_val<_Elem, _Ax>" is not virtual
#                   : public _String_val<_Elem, _Ax>
#                            ^
#
#  ../../../src/type/types.hxx(64): remark #522:
#    function "type::Named::type_set(const type::Type *)" redeclared "inline"
#    after being called
#      Named::type_set (const Type* type)
#              ^
#
#  ./../ast/print-visitor.hh(21): warning #654:
#  overloaded virtual function "ast::GenVisitor<K>::visit
#                               [with K=ast::const_kind]"
#   is only partially overridden in class "ast::PrintVisitor"
#      class PrintVisitor : public DefaultVisitor<const_kind>
#            ^
#
#  /intel/compiler70/ia32/include/xlocale(1381): remark #810:
#     conversion from "int" to "char" may lose significant bits
#                  return (widen((char)_Tolower((unsigned char)narrow(_Ch),
#                                ^
#
#
#   ./../ast/print-visitor.hh(331): remark #981:
#      operands are evaluated in unspecified order
#           _ostr << "type " << e.name_get () << " = ";
#                               ^
#
#  scantiger.cc(324): remark #1418:
#     external definition with no prior declaration
#    static char yy_hold_char;
#                ^
#
#
# Apple's GCC accepts any garbage after "-w", which clutters our command
# lines.
case $CXX_FLAVOR in
  (gcc-apple) ;;
  (*) TC_CXX_WARNINGS([[[-wd111,193,279,383,444,522,654,810,981,1418]]]);;
esac


# ------- #
# MS VC++ #
# ------- #

# If the compiler is MS VC++, define WIN32.
case $CXX_FLAVOR in
  (msvc)
  AC_DEFINE([WIN32], [], [Whether the host is Windows using VC++.])
  # http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/167fb6e5-ef74-4b03-83ee-f1de8f7ef291/
  # http://msdn.microsoft.com/en-us/library/ttcz0bys(VS.80).aspx

  # Unfortunately there are many files that do not use config.h, so it
  # is safer, but adds clutter, to add this to the CPPFLAGS rather
  # than in config.h.  See libport/cmath for _USE_MATH_DEFINES.
  for ac_flag in                                \
     _AFX_SECURE_NO_WARNINGS                    \
     _ATL_SECURE_NO_WARNINGS                    \
     _CRT_NONSTDC_NO_DEPRECATE                  \
     _CRT_SECURE_NO_DEPRECATE                   \
     _USE_MATH_DEFINES
  do
    URBI_APPEND_CPPFLAGS([-D$ac_flag=1])
  done
  ;;
esac

# -OPTION or /OPTION
# ------------------
# We use -OPTION instead of /OPTION because vc++ accepts both, and
# the former is (i) easier to read for us Unix guys, and (ii) accepted
# by distcc while /OPTION is interpreted as a file name.

# There seems to be several models of exception handling with VC++.
# There are weird warnings if we don't activate the C++ style exception
# handling.  Let's play it safe and activate it.
#
# /EHsc: enable C++ exception handling + extern "C" defaults to nothrow.
case $CXX_FLAVOR in
 (msvc) TC_COMPILER_OPTION_IF([[-EHsc]],
                              [URBI_APPEND_CXXFLAGS([-EHsc])]);;
esac

# --------------------- #
# Warnings are errors.  #
# --------------------- #

# There are too many warnings in OPEN-R (its fault).
# If we're building on Windows with GCC, it's likely to be gcc 3.4.5 which has
# *many* false positive when it comes to uninitialized variable use.
# Generally speaking, once there will be a decent version of GCC for MinGW,
# we'll remove this.
case $GXX:$host in
  ( yes:cygwin* | yes:*mingw* | yes:mipsel-*linux-* | yes:*arm*) :;;
  ( yes:*) # for other occurrences of G++, it's fine to use -Werror.
    TC_CXX_WARNINGS([-Werror])
  ;;
esac

# ----------------------------------------------- #
# Remove MS Visual Compiler's spurious warnings.  #
# ----------------------------------------------- #

# We use to put lots of -wd options here, but it is simpler to have
# the compiler load a file with lots of pragmas instead.  Expect for
# C4668, which is about the preprocessor: the warnings in preprocessor
# mode (-E) are not affected by #pragma (see warning-push.hh for a
# description of this warning).  It does matter to us, since we use
# ccache, which uses -E first.
#
# We don't check for support of "-include libport/warning-push.hh",
# because we know it supports it, but will fail as the -I needed
# to find the file are not set yet.  They will be given in the
# Makefiles.
#
# Check for this in last resort, as it does break the use of
# WARNING_CXXFLAGS without paths to libport headers.
case $CXX_FLAVOR in
 (msvc) TC_CXX_WARNINGS([-wd4668])
        URBI_APPEND_FLAGS([WARNING_CXXFLAGS],
                          [-include libport/warning-push.hh]);;
esac

AC_LANG_POP([C++])
])


## Local Variables:
## mode: autoconf
## End:
