/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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
#include <libport/cassert>

#include <libport/indent.hh>
#include <libport/xalloc.hh>

namespace libport
{

  namespace
  {
    typedef xalloc<long int>indentation_type;
    indentation_type indentation;
  }

  std::ostream& incindent(std::ostream& o)
  {
    indentation(o) += 2;
    return o;
  }

  std::ostream& decindent(std::ostream& o)
  {
    aver(indentation(o));
    indentation(o) -= 2;
    return o;
  }

  std::ostream& resetindent(std::ostream& o)
  {
    indentation(o) = 0;
    return o;
  }

  std::ostream& indent(std::ostream& o)
  {
    // Be sure to be able to restore the stream flags.
    char fill = o.fill(' ');
    return o << std::setw(indentation(o))
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
