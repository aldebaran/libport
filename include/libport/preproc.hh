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
# define LIBPORT_CAT BOOST_PP_CAT

/// __FILE__:__LINE__ as a string.
# define __HERE__				\
  __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)

#endif
