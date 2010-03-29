/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
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
# include <boost/preprocessor/seq/transform.hpp>

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

/// Concatenate the two arguments.
# ifdef _MSC_VER
#  define LIBPORT_CAT(A, B)   LIBPORT_CAT_0(A, B)
#  define LIBPORT_CAT_0(A, B) LIBPORT_CAT_1(A, B)
#  define LIBPORT_CAT_1(A, B) LIBPORT_CAT_2(A, B)
#  define LIBPORT_CAT_2(A, B) LIBPORT_CAT_3(A, B)
#  define LIBPORT_CAT_3(A, B) LIBPORT_CAT_4(A, B)
#  define LIBPORT_CAT_4(A, B) LIBPORT_CAT_5(A, B)
#  define LIBPORT_CAT_5(A, B) LIBPORT_CAT_6(A, B)
#  define LIBPORT_CAT_6(A, B) LIBPORT_CAT_7(A, B)
#  define LIBPORT_CAT_7(A, B) LIBPORT_CAT_8(A, B)
#  define LIBPORT_CAT_8(A, B) LIBPORT_CAT_9(A, B)
#  define LIBPORT_CAT_9(A, B) A ## B
# else
#  define LIBPORT_CAT(A, B) LIBPORT_CAT_(A, B)
#  define LIBPORT_CAT_(A, B) A ## B
# endif

/// __FILE__:__LINE__ as a string.
# define __HERE__				\
  __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)

// FIXME: Generate all these

/*-----------.
| LIBPORT_ID |
`-----------*/

# define LIBPORT_ID(...) __VA_ARGS__

/*--------------.
| LIBPORT_DELAY |
`--------------*/

# define LIBPORT_DELAY(X) X

/*--------------.
| LIBPORT_COMMA |
`--------------*/

# define LIBPORT_COMMA ,

/*-------------.
| LIBPORT_WRAP |
`-------------*/

# define LIBPORT_WRAP(...) (__VA_ARGS__)

/*---------------.
| LIBPORT_UNWRAP |
`---------------*/

# define LIBPORT_UNWRAP(...) LIBPORT_ID __VA_ARGS__


/*-------------.
| LIBPORT_LIST |
`-------------*/

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
#define LIBPORT_LIST_BUILD_1(A0, IGNORED) (A0)
#define LIBPORT_LIST_BUILD_2(A0,A1, IGNORED) (A0)(A1)
#define LIBPORT_LIST_BUILD_3(A0,A1,A2, IGNORED) (A0)(A1)(A2)
#define LIBPORT_LIST_BUILD_4(A0,A1,A2,A3, IGNORED) (A0)(A1)(A2)(A3)
#define LIBPORT_LIST_BUILD_5(A0,A1,A2,A3,A4, IGNORED) (A0)(A1)(A2)(A3)(A4)
#define LIBPORT_LIST_BUILD_6(A0,A1,A2,A3,A4,A5, IGNORED) (A0)(A1)(A2)(A3)(A4)(A5)
#define LIBPORT_LIST_BUILD_7(A0,A1,A2,A3,A4,A5,A6, IGNORED) (A0)(A1)(A2)(A3)(A4)(A5)(A6)
#define LIBPORT_LIST_BUILD_8(A0,A1,A2,A3,A4,A5,A6,A7, IGNORED) (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)
#define LIBPORT_LIST_BUILD_9(A0,A1,A2,A3,A4,A5,A6,A7,A8, IGNORED) (A0)(A1)(A2)(A3)(A4)(A5)(A6)(A7)(A8)

# ifdef _MSC_VER
#  define LIBPORT_LIST_VASIZE_HELPER1(...)                                \
  LIBPORT_DELAY(LIBPORT_LIST_VASIZE_HELPER)##2(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
# else
#  define LIBPORT_LIST_VASIZE_HELPER1(...)                                \
  LIBPORT_LIST_VASIZE_HELPER2(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
# endif
# define LIBPORT_LIST_VASIZE_HELPER2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, ...) \
  A10

# define LIBPORT_LIST_VASIZE(List)                                    \
  LIBPORT_LIST_VASIZE_HELPER1 List

#define LIBPORT_LIST_HELPER(M, A) M A

#define LIBPORT_LIST(...)                                               \
  LIBPORT_LIST_HELPER(                                                 \
  LIBPORT_CAT(LIBPORT_LIST_BUILD_, LIBPORT_LIST_VASIZE((__VA_ARGS__))),        \
  (__VA_ARGS__))                                                       \

/*-------------.
| LIBPORT_SIZE |
`-------------*/

# define LIBPORT_LIST_SIZE(List) BOOST_PP_SEQ_SIZE(List)

/*-------------------.
| LIBPORT_LIST_EMPTY |
`-------------------*/

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
# define LIBPORT_LIST_EMPTY(List) LIBPORT_CAT(LIBPORT_LIST_EMPTY_, LIBPORT_LIST_SIZE(List))

/*------------------.
| LIBPORT_LIST_HEAD |
`------------------*/

# define LIBPORT_LIST_HEAD(List) BOOST_PP_SEQ_HEAD(List)

/*------------------.
| LIBPORT_LIST_TAIL |
`------------------*/

# define LIBPORT_LIST_TAIL(List) BOOST_PP_SEQ_TAIL(List)

/*-----------------.
| LIBPORT_LIST_MAP |
`-----------------*/

# define LIBPORT_LIST_MAP_HELPER(Unused, Macro, Elt) Macro(Elt)
# define LIBPORT_LIST_MAP(Macro, List) BOOST_PP_SEQ_TRANSFORM(LIBPORT_LIST_MAP_HELPER, Macro, List)

/*-----------------.
| LIBPORT_LIST_AUX |
`-----------------*/

# define LIBPORT_LIST_MAP_AUX_HELPER(Unused, Aux, Elt) BOOST_PP_TUPLE_ELEM(2, 0, Aux)(Elt, BOOST_PP_TUPLE_ELEM(2, 1, Aux))
# define LIBPORT_LIST_MAP_AUX(Macro, List, Aux) BOOST_PP_SEQ_TRANSFORM(LIBPORT_LIST_MAP_AUX_HELPER, (Macro, Aux), List)

/*-------------------.
| LIBPORT_LIST_APPLY |
`-------------------*/

# define LIBPORT_LIST_APPLY_HELPER(Unused, Macro, Elt) Macro(Elt)
# define LIBPORT_LIST_APPLY(Macro, List) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_HELPER, Macro, List)

/*------------------------.
| LIBPORT_LIST_APPLY_ARGS |
`------------------------*/

# define LIBPORT_LIST_APPLY_ARGS_HELPER(Unused, Macro, Elt) Macro Elt
# define LIBPORT_LIST_APPLY_ARGS(Macro, List) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_ARGS_HELPER, Macro, List)

/*-----------------.
| LIBPORT_LIST_AUX |
`-----------------*/

# define LIBPORT_LIST_APPLY_AUX_HELPER(Unused, Aux, Elt) BOOST_PP_TUPLE_ELEM(2, 0, Aux)(Elt, BOOST_PP_TUPLE_ELEM(2, 1, Aux))
# define LIBPORT_LIST_APPLY_AUX(Macro, List, Aux) BOOST_PP_SEQ_FOR_EACH(LIBPORT_LIST_APPLY_AUX_HELPER, (Macro, Aux), List)

/*-----------------.
| LIBPORT_LIST_NTH |
`-----------------*/

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

# define LIBPORT_LIST_NTH_0_LIBPORT_FALSE(List) LIBPORT_LIST_HEAD(List)
# define LIBPORT_LIST_NTH(Nth, List) BOOST_PP_CAT(LIBPORT_LIST_NTH_, Nth)(List)

/*----------------.
| LIBPORT_FLATTEN |
`----------------*/

# define LIBPORT_LIST_FLATTEN(List) LIBPORT_LIST_SEP(List, )

/*-----------------.
| LIBPORT_LIST_SEP |
`-----------------*/

# define LIBPORT_LIST_SEP_HELPER(Elt, Sep) LIBPORT_UNWRAP(Sep) Elt
# define LIBPORT_LIST_SEP_LIBPORT_FALSE(List, Sep) LIBPORT_LIST_HEAD(List) \
  LIBPORT_LIST_APPLY_AUX(LIBPORT_LIST_SEP_HELPER, LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_SEP_LIBPORT_TRUE(List, Sep)
# define LIBPORT_LIST_SEP(List, Sep) LIBPORT_CAT(LIBPORT_LIST_SEP_, LIBPORT_LIST_EMPTY(List))(List, LIBPORT_WRAP(Sep))

/*-----------------.
| LIBPORT_LIST_ARG |
`-----------------*/

# define LIBPORT_LIST_ARG_HELPER(Macro, Args) Macro Args
# define LIBPORT_LIST_ARG(Macro, List) LIBPORT_LIST_ARG_HELPER(Macro, (LIBPORT_LIST_SEP(List, LIBPORT_COMMA)))

/*----------------------.
| LIBPORT_LIST_DECORATE |
`----------------------*/

#define LIBPORT_LIST_DECORATE

#endif
