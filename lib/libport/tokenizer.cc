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
