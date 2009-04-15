#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_I_SERIALIZER_HXX

namespace libport
{
  namespace serialize
  {
    template <template <typename, typename> class Cont, typename T, typename A>
    void ISerializer::unserialize
    (const std::string& name, Cont<T, A>& collection)
    {
      size_t size = start_collection(name);
      for (unsigned i = 0; i < size; ++i)
      {
        T elt;
        unserialize("item", elt);
        collection.push_back(elt);
      }
      end_collection();
    }
  }
}

#endif
