#ifndef LIBPORT_SERIALIZE_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_SERIALIZER_HXX

# include <libport/foreach.hh>
# include <serialize/exception.hh>

namespace libport
{
  namespace serialize
  {
    template <template <typename, typename> class C, typename T, typename A>
    void Serializer::serialize(const std::string& name, C<T, A>& collection)
    {
      action_type a;

      if (input_)
      {
        int size = -1;
        a = serialize_collection(name, size);
        if (size > 0)
          for (int i = 0; i < size; ++i)
          {
            T item;
            serialize("item", item);
            collection.push_back(item);
          }
        else
          while (true)
          {
            try
            {
              T item;
              serialize("item", item);
              collection.push_back(item);
            }
            catch (Exception&)
            {
              break;
            }
          }
      }
      else
      {
        int size = collection.size();
        a = serialize_collection(name, size);
        foreach (T& t, collection)
        {
          serialize("item", t);
        }
      }
      if (a)
        a();
    }

    template <typename T>
    void Serializer::serialize(const std::string& name, boost::optional<T>& option)
    {
      if (input_)
      {
        T res;
        try
        {
          serialize(name, res);
          option = res;
        }
        catch (Exception&)
        { /* nothing */ }
      }
      else
        if (option)
          serialize(name, option.get());
    }
  }
}

#endif
