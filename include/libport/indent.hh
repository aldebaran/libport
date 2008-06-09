/**
 ** \file libport/indent.hh
 ** \brief Indentation relative functions.
 **/

#ifndef LIBPORT_INDENT_HH
# define LIBPORT_INDENT_HH

# include <iosfwd>

namespace libport
{
  /// Increment the indentation.
  std::ostream& incindent (std::ostream& o);

  /// Decrement the indentation.
  std::ostream& decindent (std::ostream& o);

  /// Reset the indentation.
  std::ostream& resetindent (std::ostream& o);

  /// Set the indentation.
  std::ostream& indent (std::ostream& o);

  /// Print an end of line, then set the indentation.
  std::ostream& iendl (std::ostream& o);

  /// Increment the indentation, print an end of line, and set the indentation.
  std::ostream& incendl (std::ostream& o);

  /// Decrement the indentation, print an end of line, and set the indentation.
  std::ostream& decendl (std::ostream& o);
}

#endif // !LIBPORT_INDENT_HH
