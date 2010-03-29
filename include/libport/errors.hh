/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_META_ERRORS_HH
# define LIBPORT_META_ERRORS_HH

namespace libport
{
  namespace meta
  {
    /// Dummy type
    class Null
    {};

    template <typename>
    struct Error;

    template <bool test, typename E>
    struct Assert: public Error<E>
    {};

    template <typename E>
    struct Assert<true, E>
    {};


    /*----------------.
    | Useful errors.  |
    `----------------*/

    template <int I>
    struct Must_be_positive {};

  }
}


#endif
