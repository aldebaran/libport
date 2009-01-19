#ifndef LIBPORT_SERIALIZE_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_SERIALIZER_HXX

# include <libport/foreach.hh>
# include <libport/serialize/exception.hh>

namespace libport
{
  namespace serialize
  {
    template <typename T>
    void Serializer::serialize(const std::string& name, std::vector<T>& collection)
    {
      action_type a = serialize_collection(name);
      if (input_)
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
      else
        foreach (T& t, collection)
          serialize("item", t);
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
