#include <libport/tokenizer.hh>

namespace libport
{

  boost::tokenizer< boost::char_separator<char> >
  make_tokenizer (const std::string& str, const char* sep)
  {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> s (sep);
    return tokenizer (str, s);
  }
  
}
