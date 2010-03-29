/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_TYPE_INFO_HXX
# define LIBPORT_TYPE_INFO_HXX

# include <typeinfo>

namespace libport
{
  inline
  TypeInfo::TypeInfo(const ::std::type_info& info)
    : info_(&info)
  {}

  template <typename T>
  inline
  TypeInfo::TypeInfo(const T& obj)
    : info_(&typeid(obj))
  {}

  inline bool
  TypeInfo::operator <(const TypeInfo& other) const
  {
    return info_->before(*other.info_);
  }

  inline bool
  TypeInfo::operator==(const TypeInfo& other) const
  {
    return info_ == other.info_;
  }

  inline std::string
  TypeInfo::name() const
  {
    return info_->name();
  }

  inline std::ostream&
  operator << (std::ostream& out, const TypeInfo& info)
  {
    out << info.name();
    return out;
  }
}


#endif
