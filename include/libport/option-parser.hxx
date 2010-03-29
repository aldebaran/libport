/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef OPTION_PARSER_HXX
# define OPTION_PARSER_HXX

# include <libport/cassert>
# include <libport/lexical-cast.hh>

namespace libport
{
  template <typename T>
  inline
  T OptionValue::get(const boost::optional<T>& def) const
  {
    if (!filled_)
    {
      aver(def);
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
