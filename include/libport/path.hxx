/**
 ** \file misc/path.hxx
 ** \brief Path: implements inline function of misc/path.hh
 */

#ifndef MISC_PATH_HXX
# define MISC_PATH_HXX

# include "libport/path.hh"

namespace libport
{
  inline std::ostream&
  operator<< (std::ostream& ostr, const path& p)
  {
    return p.dump (ostr);
  }

  inline bool
  path::absolute_get () const
  {
    return this->absolute_;
  }
}

#endif // !MISC_PATH_HXX
