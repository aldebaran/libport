/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/symbol.hxx
 ** \brief Inline implementation of libport::Symbol.
 */

#ifndef LIBPORT_SYMBOL_HXX
# define LIBPORT_SYMBOL_HXX

# ifdef WITH_BOOST_SERIALIZATION
#  include <boost/serialization/string.hpp>
# endif

# include <ostream>
# include <libport/symbol.hh>
# include <libport/cassert>

namespace libport
{
  //<<
  inline Symbol::Symbol()
  {
    static const Symbol s("");
    str_ = s.str_;
  }

  inline const std::string&
  Symbol::name_get () const
  {
    aver(str_);
    return *str_;
  }

  inline Symbol::operator const std::string& () const
  {
    aver(str_);
    return *str_;
  }

  inline Symbol&
  Symbol::operator= (const Symbol& rhs)
  {
    if (this != &rhs)
      str_ = rhs.str_;
    return *this;
  }

  inline bool
  Symbol::operator== (const Symbol& rhs) const
  {
    return str_ == rhs.str_;
  }

  inline bool
  Symbol::operator!= (const Symbol& rhs) const
  {
    return !operator== (rhs);
  }
  //>>

  // The value inserted in a `set' container is only duplicated once:
  // at insertion time. So for sake of speed, we could safely compare
  // the addresses of the strings.  Nevertheless, in order to produce
  // stable results, we sort the actual string values.
  inline bool
  Symbol::operator< (const Symbol& rhs) const
  {
    //<<
    aver(str_);
    aver(rhs.str_);
    return *str_ < *rhs.str_;
    //>>
  }

  inline
  bool
  Symbol::empty() const
  {
    aver(str_);
    return str_->empty();
  }

  //<<
  inline std::ostream&
  operator<< (std::ostream& ostr, const Symbol& the)
  {
    return ostr << the.name_get ();
  }
  //>>

  inline Symbol
  Symbol::make_empty()
  {
    static Symbol empty_symbol = Symbol("");
    return empty_symbol;
  }

  inline std::size_t
  hash_value(libport::Symbol s)
  {
    const std::string* v = &s.name_get();
    std::size_t x =
      static_cast<std::size_t>(reinterpret_cast<std::ptrdiff_t>(v));
    return x + (x >> 3);
  }

#ifdef WITH_BOOST_SERIALIZATION

  template <typename Archive>
  void Symbol::save(Archive& ar, const unsigned int /* version */) const
  {
    ar & *const_cast<std::string*>(str_);
  }

  template <typename Archive>
  void Symbol::load(Archive& ar, const unsigned int /* version */)
  {
    std::string s;
    ar & s;
    str_ = &*string_set_instance ().insert (s).first;
  }

#endif // WITH_BOOST_SERIALIZATION

}

#endif // !LIBPORT_SYMBOL_HXX
