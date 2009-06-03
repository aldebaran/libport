#ifndef O_SERIALIZER_HXX
# define O_SERIALIZER_HXX

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    OSerializer<Exact>::OSerializer(std::ostream& output)
      : stream_(output)
    {}

    template <class Exact>
    OSerializer<Exact>::~OSerializer()
    {}

    template <class Exact>
    template <typename T>
    void
    OSerializer<Exact>::serialize(const std::string& name, typename traits::Arg<T>::res v)
    {
      Exact::template Impl<T>::put(name, v, stream_, static_cast<Exact&>(*this));
    }
  }
}

#endif
