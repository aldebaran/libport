/**
 ** \file libport/symbol.hxx
 ** \brief Inline implementation of libport::Symbol.
 */

#ifndef LIBPORT_SYMBOL_HXX
# define LIBPORT_SYMBOL_HXX

# include <boost/serialization/string.hpp>

# include <libport/symbol.hh>
# include <libport/assert.hh>

namespace libport
{
  //<<
  inline const std::string&
  Symbol::name_get () const
  {
    assert (str_);
    return *str_;
  }

  inline Symbol::operator const std::string& () const
  {
    assert (str_);
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
    assert (str_);
    assert (rhs.str_);
    return *str_ < *rhs.str_;
    //>>
  }

  //<<
  inline std::ostream&
  operator<< (std::ostream& ostr, const Symbol& the)
  {
    return ostr << the.name_get ();
  }
  //>>

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

}

#endif // !LIBPORT_SYMBOL_HXX
