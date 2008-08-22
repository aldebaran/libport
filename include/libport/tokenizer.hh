#ifndef LIBPORT_TOKENIZER_HH
# define LIBPORT_TOKENIZER_HH

# include <string>

# include <boost/tokenizer.hpp>
# include <libport/export.hh>

namespace libport
{
  LIBPORT_API
  boost::tokenizer< boost::char_separator<char> >
  make_tokenizer (const std::string& str, const char* sep);

}

#endif // !LIBPORT_TOKENIZER_HH
