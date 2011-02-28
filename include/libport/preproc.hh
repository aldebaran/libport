/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PREPROC_HH
# define LIBPORT_PREPROC_HH

# include <boost/function.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/seq/for_each.hpp>
# include <boost/preprocessor/seq/remove.hpp>
# include <boost/preprocessor/seq/transform.hpp>


/// Expand to nothing.
///
/// This is useful to macro arguments that we want empty, but to avoid
/// warnings from MSCV.  For instance the following source produces
/// warnings for each line except the last one, using LIBPORT_EMPTY.
///
///  #pragma warning(4: 4003)
///  #define MACRO(Arg) Arg
///  #define EMPTY
///
///  void foo ()
///  {
///    MACRO();
///    MACRO( );
///    MACRO(/**/);
///    MACRO(/*1*/);
///    MACRO(EMPTY);
///  }
///
/// The problem exists only for unary macros.  The following example
/// triggers no warning at all.
///
///  #pragma warning(4: 4003)
///  #define MACRO(A, B) A B
///  #define EMPTY
///
///  void foo ()
///  {
///    MACRO(,);
///    MACRO(, /**/);
///    MACRO(/**/,);
///    MACRO(/**/, /**/);
///  }
# define LIBPORT_EMPTY
/// The same, but less intrusive.
# define __


# define LIBPORT_MAP_HELPER(R, Macro, Elt) (Macro(Elt))
/// Return \a Seq, with \a Macro applied on every element.
# define LIBPORT_MAP(Macro, Seq)                                 \
  BOOST_PP_SEQ_FOR_EACH(LIBPORT_MAP_HELPER, Macro, Seq)          \

# define LIBPORT_APPLY_HELPER(R, Macro, Elt) Macro(Elt)
/// Apply \a Macro applied on every element of \a Seq.
# define LIBPORT_APPLY(Macro, Seq)                               \
  BOOST_PP_SEQ_FOR_EACH(LIBPORT_APPLY_HELPER, Macro, Seq)        \

/// Return the first element of \a Pair
# define LIBPORT_FIRST(Pair) BOOST_PP_TUPLE_ELEM(2, 0, Pair)
/// Return the second element of \a Pair
# define LIBPORT_SECOND(Pair) BOOST_PP_TUPLE_ELEM(2, 1, Pair)

/// Return the first element of \a Seq
# define LIBPORT_HEAD(Seq) BOOST_PP_SEQ_HEAD(Seq)
/// Return \a Seq, minus the first element
# define LIBPORT_TAIL(Seq) BOOST_PP_SEQ_TAIL(Seq)

# define LIBPORT_SEPARATE_HELPER(Elt) , Elt
/// Separate \a Seq with commas
# define LIBPORT_SEPARATE(Seq)                                          \
  LIBPORT_HEAD(Seq) LIBPORT_APPLY(LIBPORT_SEPARATE_HELPER, LIBPORT_TAIL(Seq))

/// Separate \a Seq with \a Sep
# define LIBPORT_ENUM(Seq, Sep)                                         \
  LIBPORT_HEAD(Seq) BOOST_PP_SEQ_FOR_EACH(LIBPORT_ENUM_HELPER, Sep, BOOST_PP_SEQ_TAIL(Seq))
# define LIBPORT_ENUM_HELPER(Data, Sep, Elt) Sep Elt

/// Separate \a Seq with \a LSep for the last separator and \a Sep everywhere else.
/// Typically: "a, b, c, d and e".
# define LIBPORT_ENUM_PRETTY(Seq, Sep, LSep)                            \
  BOOST_PP_CAT(                                                         \
    LIBPORT_ENUM_PRETTY_,                                               \
    BOOST_PP_IF(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(Seq)), SEVERAL, ONE))    \
  (Seq, Sep, LSep)
# define LIBPORT_ENUM_PRETTY_ONE(Seq, Sep, LSep)        \
  BOOST_PP_SEQ_HEAD(Seq)
# define LIBPORT_ENUM_PRETTY_SEVERAL(Seq, Sep, LSep)    \
  LIBPORT_ENUM(LIBPORT_RTAIL(Seq), Sep) LSep LIBPORT_RHEAD(Seq)

/// The last element element of the sequence (reverse head)
# define LIBPORT_RHEAD(Seq) BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(Seq)), Seq)
/// The sequence minus the last element (reverse tail)
# define LIBPORT_RTAIL(Seq) BOOST_PP_SEQ_REMOVE(Seq, BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(Seq)))

/// Concatenate the two arguments.
# ifdef _MSC_VER

#  define LIBPORT_CAT(A, B) LIBPORT_CAT_0(A, B)
/*
#!/usr/bin/python

for i in range(0, 32):
  print '#  define LIBPORT_CAT_%s(A, B) LIBPORT_CAT_%s(A, B)' % (i, i + 1)
*/
#  define LIBPORT_CAT_0(A, B) LIBPORT_CAT_1(A, B)
#  define LIBPORT_CAT_1(A, B) LIBPORT_CAT_2(A, B)
#  define LIBPORT_CAT_2(A, B) LIBPORT_CAT_3(A, B)
#  define LIBPORT_CAT_3(A, B) LIBPORT_CAT_4(A, B)
#  define LIBPORT_CAT_4(A, B) LIBPORT_CAT_5(A, B)
#  define LIBPORT_CAT_5(A, B) LIBPORT_CAT_6(A, B)
#  define LIBPORT_CAT_6(A, B) LIBPORT_CAT_7(A, B)
#  define LIBPORT_CAT_7(A, B) LIBPORT_CAT_8(A, B)
#  define LIBPORT_CAT_8(A, B) LIBPORT_CAT_9(A, B)
#  define LIBPORT_CAT_9(A, B) LIBPORT_CAT_10(A, B)
#  define LIBPORT_CAT_10(A, B) LIBPORT_CAT_11(A, B)
#  define LIBPORT_CAT_11(A, B) LIBPORT_CAT_12(A, B)
#  define LIBPORT_CAT_12(A, B) LIBPORT_CAT_13(A, B)
#  define LIBPORT_CAT_13(A, B) LIBPORT_CAT_14(A, B)
#  define LIBPORT_CAT_14(A, B) LIBPORT_CAT_15(A, B)
#  define LIBPORT_CAT_15(A, B) LIBPORT_CAT_16(A, B)
#  define LIBPORT_CAT_16(A, B) LIBPORT_CAT_17(A, B)
#  define LIBPORT_CAT_17(A, B) LIBPORT_CAT_18(A, B)
#  define LIBPORT_CAT_18(A, B) LIBPORT_CAT_19(A, B)
#  define LIBPORT_CAT_19(A, B) LIBPORT_CAT_20(A, B)
#  define LIBPORT_CAT_20(A, B) LIBPORT_CAT_21(A, B)
#  define LIBPORT_CAT_21(A, B) LIBPORT_CAT_22(A, B)
#  define LIBPORT_CAT_22(A, B) LIBPORT_CAT_23(A, B)
#  define LIBPORT_CAT_23(A, B) LIBPORT_CAT_24(A, B)
#  define LIBPORT_CAT_24(A, B) LIBPORT_CAT_25(A, B)
#  define LIBPORT_CAT_25(A, B) LIBPORT_CAT_26(A, B)
#  define LIBPORT_CAT_26(A, B) LIBPORT_CAT_27(A, B)
#  define LIBPORT_CAT_27(A, B) LIBPORT_CAT_28(A, B)
#  define LIBPORT_CAT_28(A, B) LIBPORT_CAT_29(A, B)
#  define LIBPORT_CAT_29(A, B) LIBPORT_CAT_30(A, B)
#  define LIBPORT_CAT_30(A, B) LIBPORT_CAT_31(A, B)
#  define LIBPORT_CAT_31(A, B) A ## B
# else
#  define LIBPORT_CAT(A, B) LIBPORT_CAT_(A, B)
#  define LIBPORT_CAT_(A, B) A ## B
# endif

/// __FILE__:__LINE__ as a string.
# define __HERE__				\
  __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)

// FIXME: Generate all these

/*-------------.
| LIBPORT_ID.  |
`-------------*/

# define LIBPORT_ID(...) __VA_ARGS__

/*----------------.
| LIBPORT_DELAY.  |
`----------------*/

# define LIBPORT_DELAY(X) X

/*----------------.
| LIBPORT_COMMA.  |
`----------------*/

# define LIBPORT_COMMA ,

/*---------------.
| LIBPORT_WRAP.  |
`---------------*/

# define LIBPORT_WRAP(...) (__VA_ARGS__)

/*-----------------.
| LIBPORT_UNWRAP.  |
`-----------------*/

# define LIBPORT_UNWRAP(...) LIBPORT_ID __VA_ARGS__


/*---------------.
| LIBPORT_LIST.  |
`---------------*/

/*
#!/usr/bin/python

def gen(n):
  print '#define LIBPORT_LIST_BUILD_%(max)s(%(formals)s IGNORED) %(list)s' % {
    'formals' : ''.join(map(lambda i: 'A%s,' % i, range(n))),
    'list'    : ''.join(map(lambda i: '(A%s)' % i, range(n))),
    'max'     : n,
    }

for i in range(10):
  gen(i)
*/

#define LIBPORT_LIST_BUILD_0( IGNORED)
#define LIBPORT_LIST_BUILD_1(A0, IGNORED)       \
  (A0)
#define LIBPORT_LIST_BUILD_2(A0,A1, IGNORED)    \
  (A0)(A1)
#define LIBPORT_LIST_BUILD_3(A0,A1,A2, IGNORED) \
  (A0)(A1)(A2)
#define LIBPORT_LIST_BUILD_4(A0,A1,A2,A3, IGNORED)      \
  (A0)(A1)(A2)(A3)
#define LIBPORT_LIST_BUILD_5(A0,A1,A2,A3,A4, IGNORED)   \
  (A0)(A1)(A2)(A3)(A4)
#define LIBPORT_LIST_BUILD_6(A0,A1,A2,A3,A4,A5, IGNORED)        \
  (A0)(A1)(A2)(A3)(A4)(A5)
#define LIBPORT_LIST_BUILD_7(A0,A1,A2,A3,A4,A5,A6, IGNORED)     \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)
#define LIBPORT_LIST_BUILD_8(A0,A1,A2,A3,A4,A5,A6,A7, IGNORED)  \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)
#define LIBPORT_LIST_BUILD_9(A0,A1,A2,A3,A4,A5,A6,A7,A8, IGNORED)       \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)
#define LIBPORT_LIST_BUILD_10(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9, IGNORED)   \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)
#define LIBPORT_LIST_BUILD_11(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)
#define LIBPORT_LIST_BUILD_12(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)
#define LIBPORT_LIST_BUILD_13(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)
#define LIBPORT_LIST_BUILD_14(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)
#define LIBPORT_LIST_BUILD_15(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)
#define LIBPORT_LIST_BUILD_16(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)
#define LIBPORT_LIST_BUILD_17(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)
#define LIBPORT_LIST_BUILD_18(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)
#define LIBPORT_LIST_BUILD_19(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)
#define LIBPORT_LIST_BUILD_20(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)
#define LIBPORT_LIST_BUILD_21(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)
#define LIBPORT_LIST_BUILD_22(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)
#define LIBPORT_LIST_BUILD_23(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)
#define LIBPORT_LIST_BUILD_24(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)
#define LIBPORT_LIST_BUILD_25(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)
#define LIBPORT_LIST_BUILD_26(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)
#define LIBPORT_LIST_BUILD_27(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,A26, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)(A26)
#define LIBPORT_LIST_BUILD_28(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,A26,A27, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)(A26)(A27)
#define LIBPORT_LIST_BUILD_29(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,A26,A27,A28, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)(A26)(A27)(A28)
#define LIBPORT_LIST_BUILD_30(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,A26,A27,A28,A29, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)(A26)(A27)(A28)(A29)
#define LIBPORT_LIST_BUILD_31(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,A26,A27,A28,A29,A30, IGNORED) \
  (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)(A9)(A10)(A11)(A12)(A13)(A14)(A15)(A16)(A17)(A18)(A19)(A20)(A21)(A22)(A23)(A24)(A25)(A26)(A27)(A28)(A29)(A30)

# ifdef _MSC_VER
#  define LIBPORT_LIST_VASIZE_HELPER1(...)                                \
  LIBPORT_DELAY(LIBPORT_LIST_VASIZE_HELPER)##2(__VA_ARGS__, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
# else
#  define LIBPORT_LIST_VASIZE_HELPER1(...)                                \
  LIBPORT_LIST_VASIZE_HELPER2(__VA_ARGS__, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
# endif
# define LIBPORT_LIST_VASIZE_HELPER2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, ...) \
  A32

# define LIBPORT_LIST_VASIZE(List)                                    \
  LIBPORT_LIST_VASIZE_HELPER1 List

#define LIBPORT_LIST_HELPER(M, A) M A

#define LIBPORT_LIST(...)                                               \
  LIBPORT_LIST_HELPER(                                                 \
  LIBPORT_CAT(LIBPORT_LIST_BUILD_, LIBPORT_LIST_VASIZE((__VA_ARGS__))),        \
  (__VA_ARGS__))                                                       \

/*---------------.
| LIBPORT_SIZE.  |
`---------------*/

# define LIBPORT_LIST_SIZE(List) BOOST_PP_SEQ_SIZE(List)

/*---------------------.
| LIBPORT_LIST_EMPTY.  |
`---------------------*/

# define LIBPORT_LIST_EMPTY_0 LIBPORT_TRUE
# define LIBPORT_LIST_EMPTY_1 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_2 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_3 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_4 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_5 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_6 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_7 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_8 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_9 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_10 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_11 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_12 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_13 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_14 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_15 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_16 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_17 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_18 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_19 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_20 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_21 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_22 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_23 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_24 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_25 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_26 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_27 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_28 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_29 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_30 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY_31 LIBPORT_FALSE
# define LIBPORT_LIST_EMPTY(List) LIBPORT_CAT(LIBPORT_LIST_EMPTY_, LIBPORT_LIST_SIZE(List))

/*--------------------.
| LIBPORT_LIST_HEAD.  |
`--------------------*/

# define LIBPORT_LIST_HEAD(List) BOOST_PP_SEQ_HEAD(List)

/*--------------------.
| LIBPORT_LIST_TAIL.  |
`--------------------*/

# define LIBPORT_LIST_TAIL(List) BOOST_PP_SEQ_TAIL(List)

/*-------------------.
| LIBPORT_LIST_MAP.  |
`-------------------*/

# define LIBPORT_LIST_MAP_HELPER(Unused, Macro, Elt) Macro(Elt)
# define LIBPORT_LIST_MAP(Macro, List) BOOST_PP_SEQ_TRANSFORM(LIBPORT_LIST_MAP_HELPER, Macro, List)

/*-------------------.
| LIBPORT_LIST_AUX.  |
`-------------------*/

# define LIBPORT_LIST_MAP_AUX_HELPER(Unused, Aux, Elt) BOOST_PP_TUPLE_ELEM(2, 0, Aux)(Elt, BOOST_PP_TUPLE_ELEM(2, 1, Aux))
# define LIBPORT_LIST_MAP_AUX(Macro, List, Aux) BOOST_PP_SEQ_TRANSFORM(LIBPORT_LIST_MAP_AUX_HELPER, (Macro, Aux), List)

/*---------------------.
| LIBPORT_LIST_APPLY.  |
`---------------------*/

# define LIBPORT_LIST_APPLY_HELPER(Unused, Macro, Elt) Macro(Elt)
# define LIBPORT_LIST_APPLY(Macro, List) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_HELPER, Macro, List)

/*--------------------------.
| LIBPORT_LIST_APPLY_ARGS.  |
`--------------------------*/

# define LIBPORT_LIST_APPLY_ARGS_HELPER(Unused, Macro, Elt) Macro Elt
# define LIBPORT_LIST_APPLY_ARGS(Macro, List) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_ARGS_HELPER, Macro, List)

/*-------------------.
| LIBPORT_LIST_AUX.  |
`-------------------*/

# define LIBPORT_LIST_APPLY_AUX_HELPER(Unused, Aux, Elt) BOOST_PP_TUPLE_ELEM(2, 0, Aux)(Elt, BOOST_PP_TUPLE_ELEM(2, 1, Aux))
# define LIBPORT_LIST_APPLY_AUX(Macro, List, Aux) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_AUX_HELPER, (Macro, Aux), List)

/*-------------------.
| LIBPORT_LIST_NTH.  |
`-------------------*/

/*
#!/usr/bin/python

def gen(n):
  args = {'n': n, 'n_1': n - 1,}
  print '# define LIBPORT_LIST_NTH_%(n)s(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_%(n)s_, LIBPORT_LIST_EMPTY(List))(List)' % args
  print '# define LIBPORT_LIST_NTH_%(n)s_LIBPORT_TRUE(List)' % args
  if n != 0:
    print '# define LIBPORT_LIST_NTH_%(n)s_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_%(n_1)s(LIBPORT_LIST_TAIL(List))' %args

for i in range(0, 10):
  gen(i)
*/

# define LIBPORT_LIST_NTH_0(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_0_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_0_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_1(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_1_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_1_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_1_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_0(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_2(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_2_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_2_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_2_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_1(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_3(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_3_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_3_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_3_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_2(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_4(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_4_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_4_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_4_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_3(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_5(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_5_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_5_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_5_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_4(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_6(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_6_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_6_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_6_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_5(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_7(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_7_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_7_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_7_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_6(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_8(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_8_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_8_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_8_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_7(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_9(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_9_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_9_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_9_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_8(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_10(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_10_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_10_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_10_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_9(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_11(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_11_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_11_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_11_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_10(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_12(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_12_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_12_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_12_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_11(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_13(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_13_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_13_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_13_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_12(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_14(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_14_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_14_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_14_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_13(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_15(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_15_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_15_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_15_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_14(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_16(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_16_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_16_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_16_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_15(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_17(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_17_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_17_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_17_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_16(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_18(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_18_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_18_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_18_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_17(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_19(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_19_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_19_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_19_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_18(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_20(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_20_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_20_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_20_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_19(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_21(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_21_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_21_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_21_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_20(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_22(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_22_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_22_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_22_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_21(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_23(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_23_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_23_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_23_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_22(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_24(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_24_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_24_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_24_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_23(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_25(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_25_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_25_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_25_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_24(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_26(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_26_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_26_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_26_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_25(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_27(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_27_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_27_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_27_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_26(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_28(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_28_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_28_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_28_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_27(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_29(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_29_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_29_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_29_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_28(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_30(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_30_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_30_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_30_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_29(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_31(List) BOOST_PP_CAT(LIBPORT_LIST_NTH_31_, LIBPORT_LIST_EMPTY(List))(List)
# define LIBPORT_LIST_NTH_31_LIBPORT_TRUE(List)
# define LIBPORT_LIST_NTH_31_LIBPORT_FALSE(List) LIBPORT_LIST_NTH_30(LIBPORT_LIST_TAIL(List))

# define LIBPORT_LIST_NTH_0_LIBPORT_FALSE(List) LIBPORT_LIST_HEAD(List)
# define LIBPORT_LIST_NTH(Nth, List) BOOST_PP_CAT(LIBPORT_LIST_NTH_, Nth)(List)

/*------------------.
| LIBPORT_FLATTEN.  |
`------------------*/

# define LIBPORT_LIST_FLATTEN(List) LIBPORT_LIST_SEP(List, )

/*-------------------.
| LIBPORT_LIST_SEP.  |
`-------------------*/

# define LIBPORT_LIST_SEP_HELPER(Elt, Sep) LIBPORT_UNWRAP(Sep) Elt
# define LIBPORT_LIST_SEP_LIBPORT_FALSE(List, Sep) LIBPORT_LIST_HEAD(List) \
  LIBPORT_LIST_APPLY_AUX(LIBPORT_LIST_SEP_HELPER, LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_SEP_LIBPORT_TRUE(List, Sep)
# define LIBPORT_LIST_SEP(List, Sep)                                    \
  LIBPORT_CAT(LIBPORT_LIST_SEP_,                                        \
              LIBPORT_LIST_EMPTY(List))(List, LIBPORT_WRAP(Sep))

/*-------------------.
| LIBPORT_LIST_ARG.  |
`-------------------*/

# define LIBPORT_LIST_ARG_HELPER(Macro, Args)   \
  Macro Args
# define LIBPORT_LIST_ARG(Macro, List)                                  \
  LIBPORT_LIST_ARG_HELPER(Macro, (LIBPORT_LIST_SEP(List, LIBPORT_COMMA)))

/*-----------------------.
| LIBPORT_VAARGS_APPLY.  |
`-----------------------*/

/// Call Macro(useless, Arg, elem) for each elem (extra argument).
#define LIBPORT_VAARGS_APPLY(Macro, Arg, ...)           \
  BOOST_PP_SEQ_FOR_EACH(Macro, Arg,                     \
                        LIBPORT_LIST(__VA_ARGS__,))

#endif
