#ifndef LIBPORT_META_ERRORS_HH
# define LIBPORT_META_ERRORS_HH

namespace libport
{
  namespace meta
  {
    /// Dummy type
    class Null
    {};

    template <typename>
    struct Error;

    template <bool test, typename E>
    struct Assert: public Error<E>
    {};

    template <typename E>
    struct Assert<true, E>
    {};


    /*----------------.
    | Useful errors.  |
    `----------------*/

    template <int I>
    struct Must_be_positive {};

  }
}


#endif
