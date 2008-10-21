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
    std::ostream& print (std::ostream& ostr) const;

  protected:
    template <class T>
      Escape (const T&, char delimeter);

    template <class T>
      friend Escape escape (const T&, char delimeter);

  protected:
    std::string  str_;

  private:
    std::ostream&
    escape_ (std::ostream& o, const std::string& es) const;
    char delimeter_;
  };

  template <class T>
  Escape escape (const T&, char delimeter);

  std::ostream& operator<< (std::ostream& o, const Escape&);

  std::string unescape(const std::string& s);
}

# include <libport/escape.hxx>

#endif // !LIBPORT_ESCAPE_HH
