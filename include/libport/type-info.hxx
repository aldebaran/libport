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
