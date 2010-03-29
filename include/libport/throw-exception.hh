/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/throw-exception.hh
/// \brief Provide a wrapper which cannot return to avoid incorrect
/// compiler warnings.

#ifndef LIBPORT_THROW_EXCEPTION_HH
# define LIBPORT_THROW_EXCEPTION_HH

# include <libport/cstdlib>

# include <boost/throw_exception.hpp>

# include <libport/compiler.hh>

namespace libport
{
  template<class E>
  inline
  ATTRIBUTE_NORETURN
  void throw_exception(E const & e)
  {
    boost::throw_exception(e);
    pabort("boost::throw_exception failed");
  }
}

using libport::throw_exception;

#endif // !LIBPORT_THROW_EXCEPTION_HH
