/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_RANGE_HH
# define LIBPORT_RANGE_HH

# include <libport/system-warning-push.hh>
#  include <boost/range/iterator_range.hpp>
#  include <boost/range.hpp>
# include <libport/system-warning-pop.hh>

namespace libport
{

  template< class ForwardRange >
  boost::iterator_range< typename boost::range_reverse_iterator<ForwardRange>::type >
  make_reverse_iterator_range(ForwardRange& r);

  template< class ForwardRange >
  boost::iterator_range< typename boost::range_reverse_iterator<const ForwardRange>::type >
  make_reverse_iterator_range(const ForwardRange& r);

  template< class ForwardRange >
  boost::iterator_range<typename boost::range_iterator<ForwardRange>::type>
  skip_first( ForwardRange& r);
}

# include <libport/range.hxx>

#endif // !LIBPORT_RANGE_HH
