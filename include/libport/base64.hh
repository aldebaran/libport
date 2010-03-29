/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BASE64_HH
# define LIBPORT_BASE64_HH

# include <string>
# include <libport/export.hh>

namespace libport
{
  /// Convert \a input to base 64
  /**
   ** @param input The string to convert
   ** @return      The string converted to base64
   **
   ** See http://fr.wikipedia.org/wiki/Base64 for an excellent documentation.
   */
  LIBPORT_API
  std::string base64(const std::string& input);
}

#endif
