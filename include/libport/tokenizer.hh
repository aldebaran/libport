/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_TOKENIZER_HH
# define LIBPORT_TOKENIZER_HH

# include <string>

# include <boost/tokenizer.hpp>
# include <libport/export.hh>

namespace libport
{
  // WARNING: Passing temporary values to make_tokenizer or to lines
  // is a very bad idea.  The value needs to be existing during the
  // iteration, not during the construction of the tokenizer.  The
  // following peice of code is exactly what must not be done:
  //
  //    {
  //      std::stringstream s;
  //      s << in;
  //      foreach (const std::string& l, libport::lines(s.str()))
  //        std::cerr << l << std::endl;
  //    }
  //
  // because `s.str()' no longer exists during the iteration (just
  // imagine the for-loop behind).  Instead, you really need a
  // temporary string.  The bad news is: I don't know how to enforce
  // this.
  //
  //    {
  //      std::stringstream s;
  //      s << in;
  //      std::string str = s.str();
  //      foreach (const std::string& l, libport::lines(str())
  //        std::cerr << l << std::endl;
  //    }

  typedef boost::tokenizer< boost::char_separator<char> > tokenizer_type;

  /// Split \a str at any occurrence of a character in \a dropped_delims.
  /// Members of \a kept_delims become components in there own right.
  /// \param empty_token if set to boost::keep_empty_tokens, empty
  ///        components are kept.
  LIBPORT_API
  tokenizer_type
  make_tokenizer(const std::string& str,
                 const char* dropped_delims,
                 const char* kept_delims = "",
                 boost::empty_token_policy empty_tokens = boost::drop_empty_tokens);

  /// Split \a str at each line (terminated by \n).
  /// Empty lines are kept.
  LIBPORT_API
  tokenizer_type
  lines(const std::string& str);
}

#endif // !LIBPORT_TOKENIZER_HH
