/**
 ** \file libport/separator.hh
 ** \brief Output containers with a separator between items.
 */

#ifndef LIBPORT_SEPARATOR_HH
# define LIBPORT_SEPARATOR_HH

# include <iosfwd>

namespace libport
{

  template <typename C, typename S>
  class separator
  {
  public:
    separator (const C& c, const S& s);
    std::ostream& operator() (std::ostream& o) const;
  private:
    const C& container_;
    const S& separator_;
  };

  /// Shorthand to create separator (c, s).
  template <typename C, typename S>
  separator<C, S>
  separate (const C& c, const S& s);

#if 0
  /// Shorthand to create separator (c, '\n').
  template <typename C>
  separator<C, char>
  separate (const C& c);
#endif

  /// Output the separator object \a s on \a ostr.
  template <typename C, typename S>
  inline std::ostream&
  operator<< (std::ostream& ostr, const separator<C, S>& s);

}

# include "libport/separator.hxx"

#endif // !LIBPORT_SEPARATOR_HH
