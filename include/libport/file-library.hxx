/**
 ** \file misc/file-library.hxx
 ** \brief implements inline function of misc/file-library.hh
 */

#ifndef MISC_FILE_LIBRARY_HXX
# define MISC_FILE_LIBRARY_HXX

# include "libport/file-library.hh"

namespace libport
{
  inline std::ostream&
  operator<< (std::ostream& ostr, const file_library& l)
  {
    return l.dump (ostr);
  }
}

#endif // !MISC_FILE_LIBRARY_HXX
