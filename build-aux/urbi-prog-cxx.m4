m4_pattern_forbid([^URBI_])dnl
m4_pattern_forbid([^TC_])dnl

AC_PREREQ([2.60])

# URBI_PROG_CXX
# -------------
# Look for a C++ compiler, and pass interesting warning options.
AC_DEFUN([URBI_PROG_CXX],
[# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed GCC compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# Warn about violations of some of the style guidelines from Scott
# Meyers' "Effective C++" and "More Effective C++" books.
#TC_CXX_WARNINGS([-Weffc++])

# For the time being, we are not interested about deprecated
# headers such as hash_map.h.  We should use ext/hash_map.
TC_CXX_WARNINGS([-Wno-deprecated])

dnl 		 [-Wcast-align],
dnl 		 [-Wcast-qual],
dnl 		 [-Wmissing-prototypes],
dnl 		 [-Wstrict-prototypes],
dnl 		 [-Wwrite-strings],
dnl 		 [-Wbad-function-cast],
dnl 		 [-Wmissing-declarations],
dnl		 [-Wold-style-cast],
# Use good warnings.
TC_CXX_WARNINGS([[-Wall],
		 [-W],
		 [-Woverloaded-virtual],
 		 [-Wnested-externs],
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
TC_CXX_WARNINGS([[[-wd111,193,279,383,444,522,654,810,981,1418]]])

# ------- #
# MS VC++ #
# ------- #

# If the compiler is MS VC++, define WIN32.
AC_CACHE_CHECK([whether $CXX is Microsoft's compiler], [ac_cv_cxx_compiler_ms],
[AC_COMPILE_IFELSE([[
#ifndef _MSC_VER
# error This is not a Microsoft compiler
#endif
]],
   [ac_cv_cxx_compiler_ms=yes], [ac_cv_cxx_compiler_ms=no])
])

if test "$ac_cv_cxx_compiler_ms" = yes; then
  AC_DEFINE([WIN32], [], [Whether or not we're on Windows])
fi

# ----------------------------------------------- #
# Remove MS Visual Compiler's spurious warnings.  #
# ----------------------------------------------- #

# /EHsc: enable C++ exception handling + extern "C" defaults to nothrow.
#
# warning C4121: 'symbol' : alignment of a member was sensitive to packing
# "A structure member is aligned on a memory offset whose value is not a
#  multiple of the member's size."
# (Quoting http://msdn2.microsoft.com/en-US/library/kabt0ka3.aspx)
# I see this as a spurious warning because the Visual's compiler align members
# on word boundaries by default.
#
# warning C4127: conditional expression is constant
# eg in: while (true) ...
#
# warning C4571: Informational: catch(...) semantics changed since Visual
#                C++ 7.1; structured exceptions (SEH) are no longer caught.
# http://msdn2.microsoft.com/en-us/library/55s8esw4.aspx:
# "When compiling with /EHs, a catch(...) block will not catch a structured
#  exception (divide by zero, null pointer, for example); a catch(...) block
#  will only catch explicitly-thrown, C++ exceptions."
# Reminder: SEH are Windows' exceptions (with __try, __catch etc..) we don't
# care about them so we just drop that warning.
#
# When a base class hides its copy constructor (private):
# warning C4625: 'classname' : copy constructor could not be generated because
#                              a base class copy constructor is inaccessible
#
# warning C4626: 'derived class' : assignment operator could not be generated
#                 because a base class assignment operator is inaccessible
# "An assignment operator was not accessible in a base class and was therefore
#  not generated for a derived class."
#
# warning C4668: 'MACRO' is not defined as a preprocessor macro, replacing
#                 with '0' for '#if/#elif'
#
# warning C4710: 'method_inline' : function not inlined
#
# warning C4800: 'type' : forcing value to bool 'true' or 'false' (performance
#                warning)
# "This warning is generated when a value that is not bool is assigned or
#  coerced into type bool."
# http://msdn2.microsoft.com/en-us/library/b6801kcy.aspx
#
# warning C4820: 'classname' : 'N' bytes padding added after data member 'foo'
#
TC_CXX_WARNINGS([[/EHsc],
		 [/wd4121],
		 [/wd4127],
		 [/wd4571],
		 [/wd4625],
		 [/wd4626],
		 [/wd4668],
		 [/wd4710],
		 [/wd4800],
		 [/wd4820]])


# --------------------- #
# Warnings are errors.  #
# --------------------- #

# There are too many warnings in OPEN-R (its fault).
# Don't just look at its name, we might have ccache prefixing it.
case $($CXX --version | sed 1q) in
  mipsel-linux-*) ;;
  *)
    # We currently have too many unresolved warnings with this compiler.
    # (Not its fault, ours.)
    if test $ac_cv_cxx_compiler_ms != yes; then
      TC_CXX_WARNINGS([-Werror])
    fi
  ;;
esac
])


## Local Variables:
## mode: autoconf
## End:
