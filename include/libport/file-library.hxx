/**
 ** \file libport/file-library.hxx
 ** \brief implements inline function of libport/file-library.hh
 */

#ifndef LIBPORT_FILE_LIBRARY_HXX
# define LIBPORT_FILE_LIBRARY_HXX

# include "libport/file-library.hh"

namespace libport
{
  inline std::ostream&
  operator<< (std::ostream& ostr, const file_library& l)
  {
    return l.dump (ostr);
  }
}

#endif // !LIBPORT_FILE_LIBRARY_HXX
