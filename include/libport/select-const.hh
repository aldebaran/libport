/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/select-const.hh
 ** \brief Select between a non const or a const type.
 */

#ifndef LIBPORT_SELECT_CONST_HH
# define LIBPORT_SELECT_CONST_HH

namespace libport
{

  /*------------------.
  | const selectors.  |
  `------------------*/

  /// Return \a T constified.
  template <typename T>
  struct constify_traits
  {
    typedef const T type;
  };

  /// Return \a T as is.
  template <typename T>
  struct id_traits
  {
    typedef T type;
  };



  /*------------------.
  | select_iterator.  |
  `------------------*/

  /// The iterator over a non const structure is plain iterator.
  template <typename T>
  struct select_iterator
  {
    typedef typename T::iterator type;
  };

  /// The iterator over a const structure is a const_iterator.
  template <typename T>
  struct select_iterator<const T>
  {
    typedef typename T::const_iterator type;
  };

} //namespace libport

#endif // !LIBPORT_SELECT_CONST_HH
