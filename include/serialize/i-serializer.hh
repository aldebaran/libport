#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_I_SERIALIZER_HH

# include <istream>
# include <string>

# include <libport/hierarchy.hh>
# include <serialize/export.hh>

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    class SERIALIZE_API ISerializer
    {
    public:
      ISerializer(std::istream& input);
      ~ISerializer();
      template <typename T>
      typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res, T*, T>::res
      unserialize(const std::string& name);

    private:
      std::istream& stream_;
    };
  }
}

# include <serialize/i-serializer.hxx>

#endif
