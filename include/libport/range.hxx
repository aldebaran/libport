#ifndef LIBPORT_RANGE_HXX
# define LIBPORT_RANGE_HXX

# include <libport/range.hh>

namespace libport
{

  template< class ForwardRange >
  boost::iterator_range< typename boost::range_reverse_iterator<ForwardRange>::type >
  make_reverse_iterator_range(ForwardRange& r)
  {
    return boost::make_iterator_range(boost::rbegin(r), boost::rend(r));
  }
  
  template< class ForwardRange >
  boost::iterator_range< typename boost::range_reverse_iterator<const ForwardRange>::type >
  make_reverse_iterator_range(const ForwardRange& r)
  {
    return boost::make_iterator_range(boost::const_rbegin(r), boost::const_rend(r));
  }

  template< class ForwardRange >
  boost::iterator_range<typename boost::range_iterator<ForwardRange>::type>
  skip_first( ForwardRange& r)
  {
    typename boost::range_iterator<ForwardRange>::type i = boost::begin(r);
    typename boost::range_iterator<ForwardRange>::type end = boost::end(r);
    if (i != end)
      ++i;
    return  boost::make_iterator_range(i, end);
  }
}
#endif // !LIBPORT_RANGE_HXX
