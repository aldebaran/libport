#ifndef LIBPORT_SERIALIZE_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_O_SERIALIZER_HH

# include <ostream>
# include <string>

# include <libport/traits.hh>
# include <serialize/export.hh>

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    class SERIALIZE_API OSerializer
    {
    public:
      OSerializer(std::ostream& output);
      ~OSerializer();
      template <typename T>
      void serialize(const std::string& name, typename traits::Arg<T>::res v);

    private:
      std::ostream& stream_;
    };
  }
}

# include <serialize/o-serializer.hxx>

#endif
