/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/tokenizer.hh>

namespace libport
{

  tokenizer_type
  make_tokenizer(const std::string& str,
                 const char* dropped_delims,
                 const char* kept_delims,
                 boost::empty_token_policy empty_tokens)
  {
    boost::char_separator<char> s(dropped_delims, kept_delims, empty_tokens);
    return tokenizer_type(str, s);
  }

  tokenizer_type
  lines(const std::string& str)
  {
    return make_tokenizer(str, "\n", 0, boost::keep_empty_tokens);
  }

}
