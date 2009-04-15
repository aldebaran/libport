#ifndef O_SERIALIZER_HXX
# define O_SERIALIZER_HXX

# include <libport/foreach.hh>

namespace libport
{
  namespace serialize
  {
    template <template <typename, typename> class Cont, typename T, typename A>
    void OSerializer::serialize(const std::string& name, const Cont<T, A>& collection)
    {
      size_t size = collection.size();
      start_collection(name, size);
      foreach (const T& elt, collection)
        serialize("item", elt);
      end_collection();
    }
  }
}

#endif
