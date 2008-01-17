/**
 ** \file  libport/escape.hh
 ** \brief Escape: escaping special characters for output.
 **/

#ifndef LIBPORT_ESCAPE_HH
# define LIBPORT_ESCAPE_HH

# include <iosfwd>
# include <string>

namespace libport
{

  class Escape
  {
  public:
    std::ostream& print (std::ostream& ostr) const;

  protected:
    template <class T>
    Escape (const T&);

    template <class T>
    friend Escape escape (const T&);

  protected:
    std::string  pobj_str_;

  private:
    std::ostream&
    escape_ (std::ostream& o, const std::string& es) const;
  };

  template <class T>
  Escape escape (const T&);

  std::ostream& operator<< (std::ostream& o, const Escape&);

}

# include "libport/escape.hxx"

#endif // !LIBPORT_ESCAPE_HH
