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

    // I (Akim) don't know if this is a bug in G++, but if I don't
    // declare the optional argument here, the ("real") declaration
    // that follows with the default value is not honored: callers
    // with a single argument do not match the right function.
    //
    // So declare it twice with the right prototype, including the
    // default argument.
    template <class T>
      friend Escape escape(const T&, char delimiter = '"');

  protected:
    std::string  str_;

  private:
    std::ostream& escape_(std::ostream& o, const std::string& es) const;
    char delimiter_;
  };

  /// Wrap \a t so that once output non printable characters are
  /// converted in \-based escapes.
  template <class T>
  Escape escape(const T& t, char delimiter = '"');

  /// Helper function.
  std::ostream& operator<< (std::ostream& o, const Escape&);

  /// Process \-based escapes.
  std::string unescape(const std::string& s);
}

# include <libport/escape.hxx>

#endif // !LIBPORT_ESCAPE_HH
