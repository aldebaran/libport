/**
 ** \file libport/file-library.hxx
 ** \brief implements inline function of libport/file-library.hh
 */

#ifndef LIBPORT_FILE_LIBRARY_HXX
# define LIBPORT_FILE_LIBRARY_HXX

namespace libport
{
  inline void
  file_library::append (const path& p)
  {
    push_back(p);
  }

  inline void
  file_library::prepend (const path& p)
  {
    push_front(p);
  }

  inline std::ostream&
  operator<< (std::ostream& ostr, const file_library& l)
  {
    return l.dump (ostr);
  }
}

#endif // !LIBPORT_FILE_LIBRARY_HXX
