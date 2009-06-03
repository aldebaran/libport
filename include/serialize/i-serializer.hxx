#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_I_SERIALIZER_HXX

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    ISerializer<Exact>::ISerializer(std::istream& input)
      : stream_(input)
    {}

    template <class Exact>
    ISerializer<Exact>::~ISerializer()
    {}

    template <class Exact>
    template <typename T>
    typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res, T*, T>::res
    ISerializer<Exact>::unserialize(const std::string& name)
    {
      return Exact::template Impl<T>::get(name, stream_, static_cast<Exact&>(*this));
    }
  }
}

#endif
