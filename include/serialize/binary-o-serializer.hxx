/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HXX

# include <vector>

# include <boost/optional.hpp>
# include <boost/static_assert.hpp>

# include <libport/arpa/inet.h>
# include <libport/meta.hh>
# include <libport/foreach.hh>
# include <libport/hierarchy.hh>
# include <serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    template <typename T>
    void
    BinaryOSerializer::serialize(typename traits::Arg<T>::res v)
    {
      // Name is ignored anyway.
      serialize<T>("", v);
    }

    /*----------------.
    | Generic class.  |
    `----------------*/
    template <typename T>
    struct CImpl
    {
      static void
      put(const std::string&, const T& v, std::ostream&, BinaryOSerializer& ser)
      {
        v.serialize(ser);
      }
    };

    /*------------.
    | Hierarchy.  |
    `------------*/
    typedef std::pair<const meta::BaseHierarchy*, BinaryOSerializer*> ICookie;
    template <typename T>
    struct Serialize
    {
      static void res(const ICookie& c)
      {
        const T* v = static_cast<const T*>(c.first);
        // FIXME: triple lookup
        // FIXME: 256 max classes?
        c.second->serialize<unsigned char>("id", v->id());
        v->serialize(*c.second);
      }
    };

    template <typename T>
    struct HImpl
    {
      static void
      put(const std::string&,
          const T& v,
          std::ostream&, BinaryOSerializer& ser)
      {
        ICookie c(&v, &ser);
        // FIXME: double lookup
        v.template dispatch<Serialize, ICookie>(v.id(), c);
      }
    };

    /*-----------.
    | Fallback.  |
    `-----------*/
    template <typename T>
    struct BinaryOSerializer::Impl
    {
      static void
      put(const std::string& name, const T& v,
          std::ostream& output, BinaryOSerializer& ser)
      {
        meta::If<meta::Inherits<T, meta::BaseHierarchy>::res,
          HImpl<T>, CImpl<T> >::res::put(name, v, output, ser);
      }
    };

    /*-------.
    | char.  |
    `-------*/
    template <>
    struct BinaryOSerializer::Impl<char>
    {
      static void put(const std::string&, char c, std::ostream& output,
                      BinaryOSerializer&)
      {
        write_(output, c);
      }
    };

    /*---------------------.
    | unsigned int/short.  |
    `---------------------*/

# define SERIALIZE_NET_INTEGRAL(Type)                           \
    template <>                                                 \
    struct BinaryOSerializer::Impl<Type>                        \
    {                                                           \
      static void                                               \
      put(const std::string&,                                   \
          Type i, std::ostream& output,                         \
          BinaryOSerializer& s)                                 \
      {                                                         \
        GD_CATEGORY(Serialize.Output.Binary);                   \
        GD_FINFO_DUMP("Value:      0x%x (%d)", i, i);           \
        switch (sizeof(Type))                                   \
        {                                                       \
          case 2:                                               \
            i = htons(i);                                       \
            break;                                              \
          case 4:                                               \
            i = htonl(i);                                       \
            break;                                              \
          case 8:                                               \
          {                                                     \
            union                                               \
            {                                                   \
              unsigned long long in;                            \
              struct { uint32_t high; uint32_t low; } out;      \
            } res;                                              \
            res.in = i;                                         \
            s.serialize<uint32_t>(res.out.high);                \
            s.serialize<uint32_t>(res.out.low);                 \
            return;                                             \
          }                                                     \
          default:                                              \
            unreachable();                                      \
        }                                                       \
        GD_FINFO_DUMP("Normalized: 0x%x", i);                   \
        write_(output, i);                                      \
      }                                                         \
    }

    SERIALIZE_NET_INTEGRAL(unsigned int);
    SERIALIZE_NET_INTEGRAL(unsigned long);
    SERIALIZE_NET_INTEGRAL(unsigned long long);
    SERIALIZE_NET_INTEGRAL(unsigned short);

#undef SERIALIZE_NET_INTEGRAL

    /*---------.
    | double.  |
    `---------*/
    template <>
    struct BinaryOSerializer::Impl<double>
    {
      static void put(const std::string&, double d, std::ostream& output,
                      BinaryOSerializer&)
      {
        // FIXME: non-portable
        write_(output, d);
      }
    };

    template <>
    struct BinaryOSerializer::Impl<float>
    {
      static void put(const std::string&, float d, std::ostream& output,
                      BinaryOSerializer&)
      {
        // FIXME: non-portable
        write_(output, d);
      }
    };

#define BOUNCE(From, To)                                                \
    template <>                                                         \
    struct BinaryOSerializer::Impl<From>                                \
    {                                                                   \
      static void put(const std::string& name, From v,                  \
                      std::ostream& output, BinaryOSerializer& ser)     \
      {                                                                 \
        Impl<To>::put(name, static_cast<To>(v), output, ser);           \
      }                                                                 \
    };                                                                  \

    BOUNCE(bool,           char);
    BOUNCE(unsigned char,  char);
    BOUNCE(int,            unsigned int);
    BOUNCE(long,           unsigned long);
    BOUNCE(long long,      unsigned long long);
    BOUNCE(short,          unsigned short);

#undef BOUNCE

    /*-----------.
    | Pointers.  |
    `-----------*/
    template <typename T>
    struct BinaryOSerializer::Impl<T*>
    {
      static void
      put(const std::string&, const T* ptr, std::ostream& output,
          BinaryOSerializer& ser)
      {
        if (!ptr)
        {
          Impl<char>::put("opt", null, output, ser);
          return;
        }
        ptr_map_type::iterator it =
          ser.ptr_map_.find(reinterpret_cast<long>(ptr));
        if (it != ser.ptr_map_.end())
        {
          Impl<char>::put("opt", cached, output, ser);
          Impl<unsigned>::put("id", it->second, output, ser);
        }
        else
        {
          unsigned id = ser.ptr_id_++;
          ser.ptr_map_[reinterpret_cast<long>(ptr)] = id;
          Impl<char>::put("opt", serialized, output, ser);
          Impl<T>::put("value", *ptr, output, ser);
        }
      }
    };

    /*-----------------.
    | Boost optional.  |
    `-----------------*/
    template <typename T>
    struct BinaryOSerializer::Impl<boost::optional<T> >
    {
      typedef boost::optional<T> type;
      static void
      put(const std::string&, const type& v, std::ostream& output,
          BinaryOSerializer& ser)
      {
        if (!v)
        {
          Impl<char>::put("opt", null, output, ser);
          return;
        }
        Impl<char>::put("opt", serialized, output, ser);
        Impl<type>::put("value", v.get(), output, ser);
      }
    };

    /*------------.
    | std::pair.  |
    `------------*/
    template <typename A, typename B>
    struct BinaryOSerializer::Impl<std::pair<A, B> >
    {
      typedef std::pair<A, B> type;
      static void
      put(const std::string&, const type& v, std::ostream& output,
          BinaryOSerializer& ser)
      {
        Impl<A>::put("first",  v.first,  output, ser);
        Impl<B>::put("second", v.second, output, ser);
      }
    };

    /*--------------.
    | std::string.  |
    `--------------*/
    template <>
    struct BinaryOSerializer::Impl<std::string>
    {
      static void
      put(const std::string& name,
          const std::string& s, std::ostream& output,
          BinaryOSerializer& ser)
      {
        size_t size = s.size();
        // FIXME: throw in case of overflow
        Impl<unsigned short>::put(name, size, output, ser);
        output.write(s.c_str(), size);
      }
    };

    /*--------------.
    | std::vector.  |
    `--------------*/
    template <typename T, typename A>
    struct BinaryOSerializer::Impl<std::vector<T, A> >
    {
      static void
      put(const std::string& name,
          const std::vector<T, A>& v, std::ostream& output,
          BinaryOSerializer& ser)
      {
        // FIXME: raise if overflow
        Impl<unsigned short>::put(name, v.size(), output, ser);
        foreach (const T& elt, v)
          Impl<T>::put(name, elt, output, ser);
      }
    };

    // Hash and Symbol serialization is defined here because of
    // serialization/hash/symbol dependency loop.

    /*------------------.
    | libport::Symbol.  |
    `------------------*/
    template <>
    struct BinaryOSerializer::Impl<libport::Symbol>
    {
      static void
      put(const std::string& name,
          libport::Symbol s, std::ostream& output,
          BinaryOSerializer& ser)
      {
        symbol_map_type::iterator it = ser.symbol_map_.find(s);
        if (it == ser.symbol_map_.end())
        {
          Impl<bool>::put("opt", false, output, ser);
          Impl<std::string>::put(name, s.name_get(), output, ser);
          ser.symbol_map_[s] = ser.symbol_id_++;
        }
        else
        {
          Impl<bool>::put("opt", true, output, ser);
          Impl<unsigned>::put("id", it->second, output, ser);
        }
      }
    };

    /*----------------.
    | libport::hash.  |
    `----------------*/
    template <typename K, typename V>
    struct BinaryOSerializer::Impl<boost::unordered_map<K, V> >
    {
      static void
      put(const std::string&,
          const boost::unordered_map<K, V>& m, std::ostream&,
          BinaryOSerializer& ser)
      {
        typedef typename boost::unordered_map<K, V>::value_type Value;
        // FIXME: raise if overflow
        ser.serialize<unsigned short>("size", m.size());
        foreach (const Value& elt, m)
        {
          ser.template serialize<K>("key", elt.first);
          ser.template serialize<V>("value", elt.second);
        }
      }
    };
    template<typename T>
    BinaryOSerializer& BinaryOSerializer::operator <<(T& v)
    {
      static const std::string empty("");
      serialize<typename boost::remove_const<T>::type>(empty, v);
      return *this;
    }
  }
}

#endif
