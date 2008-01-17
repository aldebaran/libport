/**
 ** \file libport/symbol.cc
 ** \brief Implementation of libport::Symbol.
 */

//<<-
#include <cctype>
//->>
#include <ostream>
#include <sstream>
#include "libport/symbol.hh"

namespace libport
{
  unsigned Symbol::counter_ = 0;

  /* We chose to put these methods here because otherwise libsymbol.a
     would be empty.  That would require changing the Makefiles we
     already gave to the students.

     Anyway, `instance' is used only during the creation, which is
     fairly infrequent.  */

  //<<
  Symbol::Symbol (const std::string& s)
    : str_ (&*string_set_instance ().insert (s).first)
  {
  }

  Symbol::Symbol (const char* s)
    : str_ (&*string_set_instance ().insert (std::string (s)).first)
  {
  }

  Symbol::string_set_type&
  Symbol::string_set_instance ()
  {
    static string_set_type map;
    return map;
  }

  Symbol::string_size_type
  Symbol::string_map_size ()
  {
    return string_set_instance ().size ();
  }
  //>>

  Symbol
  Symbol::fresh ()
  {
    return fresh ("a");
  }

  Symbol
  Symbol::fresh (const Symbol& s)
  {
    std::ostringstream osstr;
    osstr << s << "_" << counter_;
    ++counter_;
    return osstr.str ();
  }

} // namespace libport
