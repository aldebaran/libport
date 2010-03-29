/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#ifdef LIBPORT_ENABLE_SSL

# include <openssl/hmac.h>
# include <libport/hmac-sha1.hh>

namespace libport
{
  std::string hmac_sha1(const std::string& input,
			const std::string& key)
  {
    unsigned int res_size = 0;
    const unsigned char *in = (const unsigned char*)(input.c_str ());
    unsigned char *res = HMAC(EVP_sha1(),
			      key.c_str(), key.size(),
			      in, input.size(),
			      0, &res_size);
    return std::string ((const char*) res, res_size);
  }
}

#endif
