m4_pattern_forbid([^URBI_])dnl
m4_pattern_forbid([^TC_])dnl

AC_PREREQ([2.60])

# URBI_PROG_CXX
# -------------
# Look for a C++ compiler, and pass interesting warning options.
#
# Define PARSER_CXXFLAGS to -O0 if the compiler is mipsel-linux-*, since
# it is known not to be able to compile the rather large switch statement
# produced by Bison.
AC_DEFUN([URBI_PROG_CXX],
[# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed GCC compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# Useful warnings.  -Wextra is the new name for -W.
TC_CXX_WARNINGS([[-Wall], [-Wextra]])

# Warn about violations of some of the style guidelines from Scott
# Meyers' "Effective C++" and "More Effective C++" books.
TC_CXX_WARNINGS([-Weffc++])

# For the time being, we are not interested about deprecated
# headers such as hash_map.h.  We should use ext/hash_map.
TC_CXX_WARNINGS([-Wno-deprecated])

# Use good warnings.
# TC_CXX_WARNINGS([[-Wall],
# 		 [-W],
# 		 [-Wcast-align],
# 		 [-Wcast-qual],
# 		 [-Wformat],
# 		 [-Wmissing-prototypes],
# 		 [-Wstrict-prototypes],
# 		 [-Wwrite-strings],
# 		 [-Wbad-function-cast],
# 		 [-Wmissing-declarations],
# 		 [-Wnested-externs]])

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

# If using mipsel-linux-c++, then we cannot use optimization flags
# to compile Bison's output.  Don't just look at its name, we
# might have ccache prefixing it.
case $($CXX --version | sed 1q) in
  mipsel-linux-*) AC_SUBST([PARSER_CXXFLAGS], ['-O0']);;
esac
])


## Local Variables:
## mode: autoconf
## End:
