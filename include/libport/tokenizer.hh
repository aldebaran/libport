#ifndef LIBPORT_TOKENIZER_HH
# define LIBPORT_TOKENIZER_HH

# include <string>

# include <boost/tokenizer.hpp>
# include <libport/export.hh>

namespace libport
{

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

}

#endif // !LIBPORT_TOKENIZER_HH
