/**
 ** \file libport/path.hxx
 ** \brief Path: implements inline function of libport/path.hh
 */

#ifndef LIBPORT_PATH_HXX
# define LIBPORT_PATH_HXX

# include <libport/path.hh>

namespace libport
{
  inline std::ostream&
  operator<< (std::ostream& o, const path& p)
  {
    return p.dump(o);
  }

  inline
  std::string
  path::volume_get() const
  {
    return WIN32_IF(volume_, "");
  }

  inline bool
  path::absolute_get() const
  {
    return this->absolute_;
  }
}

#endif // !LIBPORT_PATH_HXX
