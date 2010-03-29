/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/separate.hh
 ** \brief Output containers with a separator between items.
 */

#ifndef LIBPORT_SEPARATE_HH
# define LIBPORT_SEPARATE_HH

# include <iosfwd>

namespace libport
{

  /// Output elements of a range with a separator.
  template <typename Container, typename Separator>
  class separator
  {
  public:
    separator(const Container& r, const Separator& s);
    /// Output the separated elements on \a o.
    std::ostream& operator() (std::ostream& o) const;
  private:
    const Container& container_;
    const Separator& separator_;
  };

  /// Shorthand to create \a separator(c, s).
  template <typename Container, typename Separator>
  separator<Container, Separator>
  separate(const Container& c, const Separator& s);

  /// Output the separator object \a s on \a ostr.
  template <typename Container, typename Separator>
  inline
  std::ostream&
  operator<< (std::ostream& o, const separator<Container, Separator>& s);

}

# include <libport/separate.hxx>

#endif // !LIBPORT_SEPARATE_HH
