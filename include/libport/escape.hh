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
 ** \file  libport/escape.hh
 ** \brief Escape: escaping special characters for output.
 **/

#ifndef LIBPORT_ESCAPE_HH
# define LIBPORT_ESCAPE_HH

# include <iosfwd>
# include <string>

# include <libport/export.hh>

namespace libport
{

  class LIBPORT_API Escape
  {
  public:
    std::ostream& print(std::ostream& ostr) const;

  protected:
    template <class T>
      Escape(const T&, char delimiter);
    Escape(const std::string&, char delimiter);

    template <class T>
      friend Escape escape(const T&, char delimiter);

  protected:
    std::string str_;

  private:
    std::ostream& escape_(std::ostream& o, const std::string& es) const;
    char delimiter_;
  };

  /// Wrap \a t so that once output non printable characters are
  /// converted in \-based escapes.
  template <class T>
  Escape escape(const T& t, char delimiter);

  // Same as escape(), but with a delimiter set to '"'.
  //
  // Of course it would be simpler to use a default argument, but in
  // that case, if I don't declare the optional argument here, GCC
  // (4.0.1 on OSX) does not honor the ("real") declaration that
  // follows with the default value: callers with a single argument do
  // not match the right function.
  //
  // We used to declare it twice with the right prototype, including
  // the default argument.  But then... VC++ broke, complaining about
  // the friendship declaration with a default argument.
  //
  // Workaround using overloaded.
  template <class T>
  Escape escape(const T& t);

  /// Helper function.
  std::ostream& operator<< (std::ostream& o, const Escape&);

  /// Process \-based escapes.
  LIBPORT_API std::string unescape(const std::string& s);
}

# include <libport/escape.hxx>

#endif // !LIBPORT_ESCAPE_HH
