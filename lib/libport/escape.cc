/**
 ** \file  libport/escape.cc
 ** \brief Implementation for libport/escape.hh.
 **/

#include <ios>
#include <iomanip>
#include <cctype>
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
}
