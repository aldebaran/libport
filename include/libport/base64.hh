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
