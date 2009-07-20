/**
 ** \file libport/deref.cc
 ** \brief Implementation for libport/deref.hh.
 */

# include <libport/deref.hh>

namespace libport
{

  Deref::Deref (std::ostream& o)
    : ostr_(o)
  {
  }

  std::ostream&
  Deref::operator<< (const char* t) const
  {
    return ostr_ << (t ? t : "NULL");
  }

}
