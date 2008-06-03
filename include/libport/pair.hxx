/**
 ** \file libport/pair.hxx
 ** \brief An std::pair with support for printing.
 */

#ifndef LIBPORT_PAIR_HXX
# define LIBPORT_PAIR_HXX

# include <ostream>
# include <libport/deref.hh>
# include <libport/pair.hh>

namespace libport
{

  template <class Fst, class Snd>
  inline
  pair<Fst, Snd>::pair (const Fst& fst, const Snd& snd)
    : std::pair<Fst, Snd> (fst, snd)
  {
  }

  template <class Fst, typename Snd>
  pair<Fst, Snd>
  make_pair (Fst fst, Snd snd)
  {
    return pair<Fst, Snd> (fst, snd);
  }

  template <class Fst, typename Snd>
  inline std::ostream&
  operator<< (std::ostream& o, const pair<Fst, Snd>& p)
  {
    return o << p.first << p.second;
  }

}

#endif // !LIBPORT_PAIR_HXX
