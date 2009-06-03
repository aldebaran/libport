#ifndef LIBPORT_TYPE_INFO_HH
# define LIBPORT_TYPE_INFO_HH

namespace libport
{
  class TypeInfo
  {
  public:
    TypeInfo(const ::std::type_info& info);
    template <typename T>
    explicit TypeInfo(const T& obj);

    bool operator <(const TypeInfo& other) const;
    bool operator==(const TypeInfo& other) const;

    std::string name() const;

  private:
    const ::std::type_info* info_;
  };

  std::ostream& operator << (std::ostream& out, const TypeInfo& info);
}

# include <libport/type-info.hxx>

#endif
