/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/pair.hh
 ** \brief An std::pair with support for printing.
 */

#ifndef LIBPORT_PAIR_HH
# define LIBPORT_PAIR_HH

# include <iosfwd>

namespace libport
{

  template <class Fst, typename Snd>
  struct pair : public std::pair<Fst, Snd>
  {
    pair (const Fst& fst, const Snd& snd);
  };

  /// Shorthand to create pair (fst, snd).
  template <class Fst, typename Snd>
  pair<Fst, Snd>
  make_pair (Fst fst, Snd snd);

  /// Output the pair object \a s on \a ostr.
  template <class Fst, typename Snd>
  inline std::ostream&
  operator<< (std::ostream& ostr, const pair<Fst, Snd>& p);

}

# include <libport/pair.hxx>

#endif // !LIBPORT_PAIR_HH
