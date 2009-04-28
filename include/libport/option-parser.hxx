#ifndef OPTION_PARSER_HXX
# define OPTION_PARSER_HXX

# include <boost/lexical_cast.hpp>

namespace libport
{
  template <typename T>
  inline
  T OptionValue::get(const boost::optional<T>& def) const
  {
    if (!filled_)
    {
      assert(def);
      return def.get();
    }
    return boost::lexical_cast<T>(value_);
  }

  template <typename T>
  inline
  T OptionValue::get() const
  {
    return get(boost::optional<T>());
  }

  inline
  std::ostream&
  operator<<(std::ostream& o, const OptionParser& p)
  {
    p.options_doc(o);
    return o;
  }

}

#endif
