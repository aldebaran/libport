/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_HMAC_SHA1_HH
# define LIBPORT_HMAC_SHA1_HH
# include <libport/config.h>
# ifdef LIBPORT_ENABLE_SSL
#   include <string>
#   include <libport/export.hh>

namespace libport
{

  /// Compute the digest of \a input using algorithm HMAC-SHA1 and \a key
  /**
   ** @param input The string for which we compute the HMAC-SHA1 digest
   ** @param key   The key used in the HMAC-SHA1 algorithm
   ** @return      The HMAC-SHA1 digest of the input string
   **
   ** See http://en.wikipedia.org/wiki/HMAC for an excellent documentation.
   */
  LIBPORT_API
  std::string hmac_sha1(const std::string& input,
			const std::string& key);
}

# endif
#endif
