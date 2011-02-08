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
 ** \file libport/symbol.cc
 ** \brief Implementation of libport::Symbol.
 */

//<<-
#include <cctype>
//->>
#include <ostream>
#include <sstream>

#include <boost/static_assert.hpp>

#include <libport/containers.hh>
#include <libport/escape.hh>
#include <libport/symbol.hh>

namespace libport
{
  // That symbols are as large as pointers is part of the
  // interface: they should be handled by copy, not by reference.
  BOOST_STATIC_ASSERT(sizeof(Symbol) == sizeof(void*));


  //<<
  Symbol::Symbol (const std::string& s)
    : str_ (&*string_set_instance ().insert (s).first)
  {
  }

  Symbol::Symbol (const char* s)
    : str_ (&*string_set_instance ().insert (s).first)
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

  std::string
  Symbol::fresh_string (const std::string& s)
  {
    // Counter for unique symbols.
    static unsigned c = 0;
    std::ostringstream o;
    do {
      o.str ("");
      o << s << "_" << c;
      ++c;
    } while (mhas(string_set_instance (), o.str ()));
    return o.str ();
  }

  Symbol
  Symbol::fresh (const Symbol& s)
  {
    return Symbol(fresh_string(s.name_get ()));
  }

  std::ostream&
  Symbol::print_escaped (std::ostream& ostr) const
  {
    aver(str_);
    const std::string& str = *str_;   // Shortcut

    // Check if the symbol is not a standard one, i.e. if it does not match
    // [A-Za-z_][A-Za-z0-9_]*
    bool needs_escaping = false;
    if (str.empty())
      needs_escaping = true;
    else if (str[0] != '_' && !isalpha(str[0]))
      needs_escaping = true;
    else
      for (std::string::size_type i = 1; i < str.length(); ++i)
	if (str[i] != '_' && !isalnum(str[i]))
	{
	  needs_escaping = true;
	  break;
	}

    if (needs_escaping)
      return ostr << '\'' << escape(str, '\'') << '\'';
    else
      return ostr << str;
  }

} // namespace libport
