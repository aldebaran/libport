/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
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
# define LIBPORT_CAT(A, B) BOOST_PP_CAT(A, B)

/// __FILE__:__LINE__ as a string.
# define __HERE__				\
  __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)

// FIXME: Generate all these

/*-----------.
| LIBPORT_ID |
`-----------*/

# define LIBPORT_ID(...) __VA_ARGS__

/*--------------.
| LIBPORT_COMMA |
`--------------*/

# define LIBPORT_COMMA ,

/*--------------.
| LIBPORT_WRAP |
`--------------*/

# define LIBPORT_WRAP(...) (__VA_ARGS__)

/*---------------.
| LIBPORT_UNWRAP |
`---------------*/

# define LIBPORT_UNWRAP(...) LIBPORT_ID __VA_ARGS__

/*-------------.
| LIBPORT_SIZE |
`-------------*/

# define LIBPORT_LIST_SIZE_HELPER1(...)             \
  LIBPORT_LIST_SIZE_HELPER2(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
# define LIBPORT_LIST_SIZE_HELPER2(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, ...) \
  A10

# define LIBPORT_LIST_SIZE(List)                                    \
  LIBPORT_LIST_SIZE_HELPER1 List

/*------------------.
| LIBPORT_LIST_HEAD |
`------------------*/

# define LIBPORT_LIST_HEAD_HELPER(Head, ...) Head

# define LIBPORT_LIST_HEAD(List) LIBPORT_LIST_HEAD_HELPER List

/*------------------.
| LIBPORT_LIST_TAIL |
`------------------*/

# define LIBPORT_LIST_TAIL_HELPER(Head, ...) (__VA_ARGS__)

# define LIBPORT_LIST_TAIL(List) LIBPORT_LIST_TAIL_HELPER List

/*-----------------.
| LIBPORT_LIST_MAP |
`-----------------*/

# define LIBPORT_LIST_MAP_0(Macro, List)
# define LIBPORT_LIST_MAP_1(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_0(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_2(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_1(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_3(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_2(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_4(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_3(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_5(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_4(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_6(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_5(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_7(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_6(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_8(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_7(Macro, LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_MAP_9(Macro, List) \
  Macro(LIBPORT_LIST_HEAD(List)), LIBPORT_LIST_MAP_8(Macro, LIBPORT_LIST_TAIL(List))

# define LIBPORT_LIST_MAP(Macro, List)                                       \
  (BOOST_PP_CAT(LIBPORT_LIST_MAP_, LIBPORT_LIST_SIZE(List))(Macro, List))

/*-----------------.
| LIBPORT_LIST_NTH |
`-----------------*/

# define LIBPORT_LIST_NTH_0(List) LIBPORT_LIST_HEAD(List)
# define LIBPORT_LIST_NTH_1(List) LIBPORT_LIST_NTH_0(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_2(List) LIBPORT_LIST_NTH_1(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_3(List) LIBPORT_LIST_NTH_2(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_4(List) LIBPORT_LIST_NTH_3(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_5(List) LIBPORT_LIST_NTH_4(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_6(List) LIBPORT_LIST_NTH_5(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_7(List) LIBPORT_LIST_NTH_6(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_8(List) LIBPORT_LIST_NTH_7(LIBPORT_LIST_TAIL(List))
# define LIBPORT_LIST_NTH_9(List) LIBPORT_LIST_NTH_8(LIBPORT_LIST_TAIL(List))

# define LIBPORT_LIST_NTH(Nth, List)                    \
  LIBPORT_CAT(LIBPORT_LIST_NTH_, Nth)(List)

/*----------------.
| LIBPORT_FLATTEN |
`----------------*/

# define LIBPORT_LIST_FLATTEN(List) LIBPORT_LIST_FLATTEN_SEP(List, )

/*-------------------------.
| LIBPORT_LIST_FLATTEN_SEP |
`-------------------------*/

# define LIBPORT_LIST_FLATTEN_0(List, Sep)
# define LIBPORT_LIST_FLATTEN_1(List, Sep) LIBPORT_LIST_HEAD(List)                     LIBPORT_LIST_FLATTEN_0(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_2(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_1(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_3(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_2(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_4(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_3(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_5(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_4(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_6(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_5(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_7(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_6(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_8(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_7(LIBPORT_LIST_TAIL(List), Sep)
# define LIBPORT_LIST_FLATTEN_9(List, Sep) LIBPORT_LIST_HEAD(List) LIBPORT_UNWRAP(Sep) LIBPORT_LIST_FLATTEN_8(LIBPORT_LIST_TAIL(List), Sep)

# define LIBPORT_LIST_FLATTEN_SEP(List, Sep) LIBPORT_CAT(LIBPORT_LIST_FLATTEN_, LIBPORT_LIST_SIZE(List))(List, LIBPORT_WRAP(Sep))

/*-------------------.
| LIBPORT_LIST_APPLY |
`-------------------*/

# define LIBPORT_LIST_APPLY(Macro, List) LIBPORT_LIST_FLATTEN(LIBPORT_LIST_MAP(Macro, List))

/*-----------------.
| LIBPORT_LIST_ARG |
`-----------------*/

# define LIBPORT_LIST_ARG(Macro, List) Macro LIBPORT_LIST_ARG_HELPER(List)
# define LIBPORT_LIST_ARG_HELPER(List) (LIBPORT_LIST_FLATTEN_SEP(List, LIBPORT_COMMA))

/*----------------------.
| LIBPORT_LIST_DECORATE |
`----------------------*/

#define LIBPORT_LIST_DECORATE

#endif
