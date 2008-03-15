/**
 ** \file  libport/escape.cc
 ** \brief Implementation for libport/escape.hh.
 **/

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <stdexcept>
#include "libport/escape.hh"

namespace libport
{
  std::ostream&
  Escape::print (std::ostream& ostr) const
  {
    return escape_ (ostr, str_);
  }

  std::ostream&
  Escape::escape_ (std::ostream& o, const std::string& es) const
  {
    // For some reason yet to be found, when we use the locale for
    // std::isprint, Valgrind goes berzerk.  So we no longer do the
    // following:
    //
    // static std::locale locale ("");
    //
    // if (std::isprint (*p, locale))
    std::ios_base::fmtflags flags = o.flags (std::ios_base::hex);
    char fill = o.fill ('0');

    for (std::string::const_iterator p = es.begin (); p != es.end (); ++p)
      switch (*p)
      {
	case '\b': o << "\\b"; break;
	case '\f': o << "\\f"; break;
	case '\n': o << "\\n"; break;
	case '\r': o << "\\r"; break;
	case '\t': o << "\\t"; break;
	case '\v': o << "\\v"; break;

	case '\\': o << "\\\\"; break;
	case '\"': o << "\\\""; break;
	case '\'': o << "\\\'"; break;
	default:
	  if (std::isprint (*p))
	    o << *p;
	  else
	    o << "\\x" << std::setw (2) << (int) (unsigned char) *p;
      }

    o.fill (fill);
    o.flags (flags);
    return o;
  }

  std::string unescape(const std::string& s)
  {
    std::string ret;
    for (int i=0; i<s.length(); i++)
    {
      if (s[i] == '\\')
      {
	if (i == s.length() - 1)
	  throw std::runtime_error("invalid escape: '\\' at end of string");
	switch(s[++i])
	{
	  case 'b': ret += '\b'; break;
	  case 'f': ret += '\f'; break;
	  case 'n': ret += '\n'; break;
	  case 'r': ret += '\r'; break;
	  case 't': ret += '\t'; break;
	  case 'v': ret += '\v'; break;
	  case '\\': ret += '\\'; break;
	  case '\"': ret += '"'; break;
	  case '\'': ret += '\''; break;
	  case 'x':
	    if (s.length() < i + 3 || !isxdigit(s[i+1]) || !isxdigit(s[i+2]))
	      throw std::runtime_error("invalid escape: '\\x' not followed by two digits");
	    ret += static_cast<char>(strtol(s.substr(i+1, 2).c_str(), 0, 16));
	    i += 2;
	    break;
	  default:
	    throw std::runtime_error(std::string("invalid escape: '\\") + s[i] + "\'");
	}
      }
      else
	ret += s[i];
    }
    return ret;
  }
}
