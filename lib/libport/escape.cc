/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file  libport/escape.cc
 ** \brief Implementation for libport/escape.hh.
 **/

#include <cctype>
#include <libport/cstdlib>
#include <iomanip>
#include <ios>
#include <stdexcept>
#include <libport/escape.hh>
#include <libport/format.hh>

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
    std::ios_base::fmtflags flags = o.flags(std::ios_base::hex);
    char fill = o.fill('0');

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
	default:
	  if (*p == delimiter_)
	    o << '\\' << *p;
	  else if (std::isprint((unsigned char) *p))
	    o << *p;
	  else
	    o << "\\x" << std::setw(2) << (int) (unsigned char) *p;
      }

    o.fill(fill);
    o.flags(flags);
    return o;
  }

#define FRAISE(...)                             \
  throw std::runtime_error(format(__VA_ARGS__))

  std::string
  unescape(const std::string& s)
  {
    std::string res;
    for (size_t i = 0; i < s.size(); ++i)
    {
      if (s[i] == '\\')
      {
	if (i == s.size() - 1)
	  FRAISE("invalid escape: '\\' at end of string");
	switch(s[++i])
	{
	  case 'b': res += '\b'; break;
	  case 'f': res += '\f'; break;
	  case 'n': res += '\n'; break;
	  case 'r': res += '\r'; break;
	  case 't': res += '\t'; break;
	  case 'v': res += '\v'; break;
	  case '\\': res += '\\'; break;
	  case '\"': res += '"'; break;
	  case '\'': res += '\''; break;
	  case 'x':
	    if (s.length() < i + 3 || !isxdigit(s[i+1]) || !isxdigit(s[i+2]))
	      FRAISE("invalid escape: '\\x' not followed by two digits");
	    res += strtol(s.substr(i+1, 2).c_str(), 0, 16);
	    i += 2;
	    break;
	  default:
	    FRAISE("invalid escape: '\\%s'", s[i]);
	}
      }
      else
	res += s[i];
    }
    return res;
  }
}
