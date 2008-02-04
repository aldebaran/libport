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
#include "libport/escape.hh"

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
    return fresh (Symbol("a"));
  }

  Symbol
  Symbol::fresh (const Symbol& s)
  {
    std::ostringstream o;
    o << s << "_" << counter_;
    ++counter_;
    return Symbol(o.str ());
  }

  std::ostream&
  Symbol::print_escaped (std::ostream& ostr) const
  {
    assert (str_);

    // Check if the symbol is not a standard one, i.e. if it does not match
    // [A-Za-z_][A-Za-z0-9_]*
    unsigned i = 0;
    if (str_->length() && ((*str_)[0] == '_' || isalpha((*str_)[0])))
    {
      i++;
      while (i < str_->length() && ((*str_)[i] == '_' || isalnum((*str_)[i])))
	i++;
    }

    // If the symbol contains special characters, escape them, and print the
    // symbol between single quotes.
    if (!str_->length() || i < str_->length())
      return ostr << '\'' << escape(*str_) << '\'';
    else
      return ostr << *str_;
  }

} // namespace libport
