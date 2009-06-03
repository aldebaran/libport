#ifndef LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HXX

# include <arpa/inet.h>

# include <vector>

# include <libport/contract.hh>
# include <libport/foreach.hh>
# include <libport/hierarchy.hh>
# include <libport/meta.hh>
# include <serialize/fwd.hh>


namespace libport
{
  namespace serialize
  {
    /*--------------.
    | Generic class |
    `--------------*/
    template <typename T>
    struct ICImpl
    {
      static T
      get(const std::string&, std::istream&, BinaryISerializer& ser)
      {
        return T(ser);
      }
    };

    /*----------.
    | Hierarchy |
    `----------*/
    template <typename T>
    struct ISerialize
    {
      static void res(const std::pair<typename T::root**, BinaryISerializer*>& c)
      {
        *c.first = reinterpret_cast<typename T::root*>(new T(*c.second));
      }
    };

    template <typename T>
    struct IHImpl
    {
      static T*
      get(const std::string&,
          std::istream&, BinaryISerializer& ser)
      {
        unsigned id = ser.unserialize<unsigned char>("id");
        typename T::root* res;
        typedef std::pair<typename T::root**, BinaryISerializer*> Cookie;
        Cookie c(&res, &ser);
        T::template dispatch<ISerialize, Cookie>(id, c);
        return reinterpret_cast<T*>(res);
      }
    };

    /*---------.
    | Fallback |
    `---------*/
    template <typename T>
    struct BinaryISerializer::Impl
    {
      static typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res,
                               T*, T>::res
      get(const std::string& name,
          std::istream& output, BinaryISerializer& ser)
      {
        return meta::If<meta::Inherits<T, meta::BaseHierarchy>::res,
          IHImpl<T>, ICImpl<T> >::res::get(name, output, ser);
      }
    };

    /*-----.
    | char |
    `-----*/
    template <>
    struct BinaryISerializer::Impl<char>
    {
      static char get(const std::string&, std::istream& input,
                      BinaryISerializer&)
      {
        char res;
        input.read(&res, sizeof(char));
        return res;
      }
    };

    /*----.
    | int |
    `----*/
    template <>
    struct BinaryISerializer::Impl<int>
    {
      static int get(const std::string&, std::istream& input,
                      BinaryISerializer&)
      {
        int normalized;
        input.read(reinterpret_cast<char*>(&normalized), sizeof(int));
        return ntohl(normalized);
      }
    };

    /*------.
    | short |
    `------*/
    template <>
    struct BinaryISerializer::Impl<short>
    {
      static short get(const std::string&, std::istream& input,
                       BinaryISerializer&)
      {
        short normalized;
        input.read(reinterpret_cast<char*>(&normalized), sizeof(short));
        return ntohs(normalized);
      }
    };

    /*-------.
    | double |
    `-------*/
    template <>
    struct BinaryISerializer::Impl<double>
    {
      static double get(const std::string&, std::istream& input,
                        BinaryISerializer&)
      {
        // FIXME: non-portable
        double res;
        input.read(reinterpret_cast<char*>(&res), sizeof(double));
        return res;
      }
    };

#define BOUNCE(From, To)                                                \
    template <>                                                         \
    struct BinaryISerializer::Impl<From>                                \
    {                                                                   \
      static From get(const std::string& name,                          \
                      std::istream& input, BinaryISerializer& ser)      \
      {                                                                 \
        return static_cast<To>(Impl<To>::get(name, input, ser));        \
      }                                                                 \
    };                                                                  \

    BOUNCE(unsigned, int);
    BOUNCE(unsigned short, short);
    BOUNCE(unsigned char, char);
    BOUNCE(bool, char);

#undef BOUNCE

    /*---------.
    | Pointers |
    `---------*/
    template <typename T>
    struct BinaryISerializer::PCImpl
    {
      static T*
      res(BinaryISerializer& ser, std::istream& input)
      {
        unsigned id = ser.ptr_id_++;
        T* res = reinterpret_cast<T*>(new char[sizeof(T)]);
        ser.ptr_map_[id] = res;
        // FIXME: copy ctor
        new (res) T(BinaryISerializer::Impl<T>::get("value", input, ser));
        return res;
      }
    };

    template <typename T>
    struct BinaryISerializer::PHImpl
    {
      static T*
      res(BinaryISerializer& ser, std::istream& input)
      {
        unsigned id = ser.ptr_id_++;
        // FIXME: loops
        T* res = BinaryISerializer::Impl<T>::get("value", input, ser);
        ser.ptr_map_[id] = res;
        return res;
      }
    };

    template <typename T>
    struct BinaryISerializer::Impl<T*>
    {
      static T* get(const std::string&, std::istream& input,
                    BinaryISerializer& ser)
      {
        pointer_status status =
          static_cast<pointer_status>(Impl<char>::get("opt", input, ser));
        switch (status)
        {
          case null:
            return 0;
          case cached:
          {
            unsigned id = Impl<unsigned>::get("id", input, ser);
            assert(libport::mhas(ser.ptr_map_, id));
            return reinterpret_cast<T*>(ser.ptr_map_[id]);
          }
          case serialized:
          {
            return meta::If<meta::Inherits<T, meta::BaseHierarchy>::res,
              PHImpl<T>, PCImpl<T> >::res
              ::res(ser, input);
          }
        }
        unreached();
      }
    };

    /*------------.
    | std::string |
    `------------*/
    template <>
    struct BinaryISerializer::Impl<std::string>
    {
      static std::string get(const std::string& name, std::istream& input,
                             BinaryISerializer& ser)
      {
        size_t l = Impl<unsigned short>::get(name, input, ser);
        // FIXME: alloca
        char* buf = new char[l];
        input.read(buf, l);
        std::string res(buf, l);
        delete [] buf;
        return res;
      }
    };

    /*------------.
    | std::vector |
    `------------*/
    template <typename T, typename A>
    struct BinaryISerializer::Impl<std::vector<T, A> >
    {
      static std::vector<T, A>
      get(const std::string& name,
          std::istream& input, BinaryISerializer& ser)
      {
        unsigned short size = Impl<unsigned short>::get(name, input, ser);
        std::vector<T, A> res;
        for (unsigned i = 0; i < size; ++i)
          res.push_back(Impl<T>::get(name, input, ser));
        return res;
      }
    };


    // Hash and Symbol serialization is defined here because of
    // serialization/hash/symbol dependency loop.

    /*----------------.
    | libport::Symbol |
    `----------------*/
    template <>
    struct BinaryISerializer::Impl<libport::Symbol>
    {
      static libport::Symbol
      get(const std::string& name, std::istream& input,
          BinaryISerializer& ser)
      {
        bool cached = Impl<bool>::get("opt", input, ser);
        if (cached)
        {
          unsigned id = Impl<unsigned>::get("id", input, ser);
          return ser.sym_map_[id];
        }
        Symbol res(Impl<std::string>::get(name, input, ser));
        ser.sym_map_.push_back(res);
        return res;
      }
    };

    /*--------------.
    | libport::hash |
    `--------------*/
    template <typename K, typename V>
    struct BinaryISerializer::Impl<libport::hash_map<K, V> >
    {
      typedef libport::hash_map<K, V> result_type;
      static result_type
      get(const std::string&, std::istream&, BinaryISerializer& ser)
      {
        typedef typename result_type::value_type Value;
        size_t size = ser.unserialize<unsigned short>("size");
        result_type res;
        for (unsigned i = 0; i < size; ++i)
        {
          K k = ser.template unserialize<K>("key");
          V v = ser.template unserialize<V>("value");
          res[k] = v;
        }
        return res;
      }
    };
  }
}

#endif
