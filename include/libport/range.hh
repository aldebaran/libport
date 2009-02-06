#ifndef LIBPORT_RANGE_HH
# define LIBPORT_RANGE_HH

# include <boost/range/iterator_range.hpp>
# include <boost/range.hpp>
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
