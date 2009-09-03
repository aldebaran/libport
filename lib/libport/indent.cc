/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** \file libport/indent.cc
 ** \brief Implementation of indentation relative functions.
 **/

#include <iomanip>
#include <ostream>
#include <cassert>

#include <libport/indent.hh>

namespace libport
{

  namespace
  {
    /// The current indentation level for \a o.
    inline long int& indent_get(std::ostream& o)
    {
      // The slot to store the current indentation level.
      static const long int indent_index = std::ios::xalloc();
      return o.iword(indent_index);
    }
  }

  std::ostream& incindent(std::ostream& o)
  {
    indent_get(o) += 2;
    return o;
  }

  std::ostream& decindent(std::ostream& o)
  {
    assert(indent_get(o));
    indent_get(o) -= 2;
    return o;
  }

  std::ostream& resetindent(std::ostream& o)
  {
    indent_get(o) = 0;
    return o;
  }

  std::ostream& indent(std::ostream& o)
  {
    // Be sure to be able to restore the stream flags.
    char fill = o.fill(' ');
    return o << std::setw(indent_get(o))
	     << ""
	     << std::setfill(fill);
  }

  std::ostream& iendl(std::ostream& o)
  {
    return o << std::endl << indent;
  }

  std::ostream& incendl(std::ostream& o)
  {
    return o << incindent << iendl;
  }

  std::ostream& decendl(std::ostream& o)
  {
    return o << decindent << iendl;
  }
}
