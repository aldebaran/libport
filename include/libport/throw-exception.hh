/// \file libport/throw-exception.hh
/// \brief Provide a wrapper which cannot return to avoid incorrect
/// compiler warnings.

#ifndef LIBPORT_THROW_EXCEPTION_HH
# define LIBPORT_THROW_EXCEPTION_HH

# include <cstdlib>

# include <boost/throw_exception.hpp>

# include <libport/compiler.hh>

namespace libport
{
  template<class E>
  ATTRIBUTE_NORETURN
  inline void throw_exception(E const & e)
  {
    boost::throw_exception(e);
    abort();
  }
}

using libport::throw_exception;

#endif // !LIBPORT_THROW_EXCEPTION_HH
