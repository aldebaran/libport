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
 ** \file libport/indent.hh
 ** \brief Indentation relative functions.
 **/

#ifndef LIBPORT_INDENT_HH
# define LIBPORT_INDENT_HH

# include <iosfwd>
# include <libport/export.hh>

namespace libport
{
  /// Increment the indentation.
  LIBPORT_API std::ostream& incindent(std::ostream& o);

  /// Decrement the indentation.
  LIBPORT_API std::ostream& decindent(std::ostream& o);

  /// Reset the indentation.
  LIBPORT_API std::ostream& resetindent(std::ostream& o);

  /// Set the indentation.
  LIBPORT_API std::ostream& indent(std::ostream& o);

  /// Print an end of line, then set the indentation.
  LIBPORT_API std::ostream& iendl(std::ostream& o);

  /// Increment the indentation, print an end of line, and set the indentation.
  LIBPORT_API std::ostream& incendl(std::ostream& o);

  /// Decrement the indentation, print an end of line, and set the indentation.
  LIBPORT_API std::ostream& decendl(std::ostream& o);
}

#endif // !LIBPORT_INDENT_HH
