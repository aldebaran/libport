#
# urbi-prog-cxx.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
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
AC_DEFUN([URBI_PROG_CXX],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
# Look for a C++ compiler.
AC_LANG_PUSH([C++])
AC_PROG_CXX

# Compute the flavor of this compiler.
URBI_PROG_CXX_FLAVOR

# Try to restrict the default visibility as much as possible.
TC_COMPILER_OPTION_IF([-fvisibility=hidden],
                      [URBI_APPEND_CXXFLAGS([-fvisibility=hidden])])
TC_COMPILER_OPTION_IF([-fvisibility-inlines-hidden],
                      [URBI_APPEND_CXXFLAGS([-fvisibility-inlines-hidden])])

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
  # than in config.h.
  for ac_flag in                                \
     _CRT_SECURE_NO_DEPRECATE                   \
     _CRT_NONSTDC_NO_DEPRECATE                  \
     _AFX_SECURE_NO_WARNINGS                    \
     _ATL_SECURE_NO_WARNINGS
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

# ----------------------------------------------- #
# Remove MS Visual Compiler's spurious warnings.  #
# ----------------------------------------------- #

# C4003: not enough actual parameters for macro
#
# When calling FOO() or even FOO(/* Something */).


# C4061: enumerator 'destroy_functor_tag' in switch of enum
#    'boost::detail::function::functor_manager_operation_type' is not
#    explicitly handled by a case label
# Although the switch has a "default:" case.  Disable this, since anyway
# GCC does the check properly.

# C4099: 'symbol' : type name first seen using 'class' now seen using
#                           'struct'
# (which prints lots of useless warnings when forward declarations are
# specified with "class" while the real definition uses "struct")

# C4121: 'symbol' : alignment of a member was sensitive to packing
# "A structure member is aligned on a memory offset whose value is not a
#  multiple of the member's size."
# (Quoting http://msdn2.microsoft.com/en-US/library/kabt0ka3.aspx)
# I see this as a spurious warning because the Visual's compiler align members
# on word boundaries by default.

# C4127: conditional expression is constant
# eg in: while (true) ...
#

# C4290: http://msdn.microsoft.com/en-us/library/sa28fef8.aspx
#
# A function is declared using exception specification, which Visual
# C++ accepts but does not implement. Code with exception
# specifications that are ignored during compilation may need to be
# recompiled and linked to be reused in future versions supporting
# exception specifications.

# C4347: behavior change: 'non_template_func()' is called instead of
# 'templat_func()'.  According to the MSDN: "In Visual Studio .NET, if you had
# a template function and a nontemplate function with the same name as the
# template function, the compiler incorrectly treated the nontemplate function
# as a specialization of the template function.".  So this is basically a
# backward compatibility warning that we can ignore.


# C4355: 'this' : used in base member initializer list
#
# For instance: UVar::UVar() : rangemin(*this, ...)
#                                       ^^^^^^


# C4251: 'urbi::UList::array' : class 'std::vector<_Ty>' needs
# to have dll-interface to be used by clients of class 'urbi::UList'
#
# In practice, I (Akim) have not seen any problems with this.


# C4512: 'classname' : assignment operator could not be generated


# C4571: Informational: catch(...) semantics changed since Visual
#                C++ 7.1; structured exceptions (SEH) are no longer caught.
# http://msdn2.microsoft.com/en-us/library/55s8esw4.aspx:
# "When compiling with /EHs, a catch(...) block will not catch a structured
#  exception (divide by zero, null pointer, for example); a catch(...) block
#  will only catch explicitly-thrown, C++ exceptions."
# Reminder: SEH are Windows' exceptions (with __try, __catch etc..) we don't
# care about them so we just drop that warning.
#
# #pragma warning : there is no warning number 'number'
# this often occurs in Boost headers, and we really don't care.

# When a base class hides its copy constructor (private):
# C4625: 'classname' : copy constructor could not be generated because
#                              a base class copy constructor is inaccessible
#
# C4626: 'derived class' : assignment operator could not be generated
#                 because a base class assignment operator is inaccessible
# "An assignment operator was not accessible in a base class and was therefore
#  not generated for a derived class."

# C4668: 'MACRO' is not defined as a preprocessor macro, replacing
#                 with '0' for '#if/#elif'

# C4710: 'method_inline' : function not inlined

# C4711: 'method' selected for inline expansion

# C4800: 'type' : forcing value to bool 'true' or 'false' (performance
#                warning)
# "This warning is generated when a value that is not bool is assigned or
#  coerced into type bool."
# http://msdn2.microsoft.com/en-us/library/b6801kcy.aspx

# C4820: 'classname' : 'N' bytes padding added after data member 'foo'
#
case $CXX_FLAVOR in
 (msvc)
    TC_CXX_WARNINGS([m4_do([-wd4003],
                           [ -wd4061],
                           [ -wd4099],
                           [ -wd4121],
                           [ -wd4127],
                           [ -wd4251],
                           [ -wd4290],
                           [ -wd4347],
                           [ -wd4355],
                           [ -wd4512],
                           [ -wd4571],
                           [ -wd4619],
                           [ -wd4625],
                           [ -wd4626],
                           [ -wd4668],
                           [ -wd4710],
                           [ -wd4711],
                           [ -wd4800],
                           [ -wd4820])])
   ;;
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
  # no:*) # ignore (this would enable -Werror for VC++ where we have *way* too
	  #         many warnings to deal with first).
esac

AC_LANG_POP([C++])
])


## Local Variables:
## mode: autoconf
## End:
