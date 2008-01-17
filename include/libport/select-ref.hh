/**
 ** \file libport/select-ref.hh
 ** \brief Select between a non ref or a ref type.
 */

#ifndef LIBPORT_SELECT_REF_HH
# define LIBPORT_SELECT_REF_HH

namespace libport
{

  /*---------------.
  | ref addition.  |
  `---------------*/

  /// Return \a T&.
  template <typename T>
  struct ref_traits
  {
    typedef T& type;
  };

  // Do not form reference to references
  template <typename T>
  struct ref_traits<T&>
  {
    typedef T& type;
  };


  /*--------------.
  | ref removal.  |
  `--------------*/

  /// Return \a T without any reference.
  template <typename T>
  struct unref_traits
  {
    typedef T type;
  };

  template <typename T>
  struct unref_traits<T&>
  {
    typedef T type;
  };

} //namespace libport

#endif // !LIBPORT_SELECT_REF_HH
