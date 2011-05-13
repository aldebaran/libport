/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_LOCALE_HH
# define LIBPORT_LOCALE_HH

# include <boost/optional.hpp>
# include <locale.h>
# include <string>
# include <libport/export.hh>

namespace libport
{
  /// Wrapper around std::setlocale.
  /// \param cat  the category as a string ("LC_ALL", etc.).
  /// \param loc  if 0, then return the current locale,
  ///             if "", then set according to the environment,
  ///             otherwise set to \a loc.
  /// \throw std::runtime_error on invalid combinations.
  LIBPORT_API
  std::string
  setlocale(const std::string& cat,
            boost::optional<std::string> loc = std::string(""));
}

#endif // !LIBPORT_LOCALE_HH
