/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HXX

# include <vector>

# include <boost/format.hpp>
# include <boost/optional.hpp>

# include <libport/arpa/inet.h>
# include <libport/foreach.hh>
# include <libport/hierarchy.hh>
# include <libport/meta.hh>
# include <libport/symbol.hh>
# include <serialize/exception.hh>
# include <serialize/fwd.hh>


namespace libport
{
  namespace serialize
  {
    template <typename T>
    typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res, T*, T>::res
    BinaryISerializer::unserialize()
    {
      // Name is ignored anyway.
      return unserialize<T>("");
    }

    /*----------------.
    | Generic class.  |
    `----------------*/
    template <typename T>
    struct ICImpl
    {
      static T
      get(const std::string&, std::istream&, BinaryISerializer& ser)
      {
        return T(ser);
      }
    };

    /*------------.
    | Hierarchy.  |
    `------------*/
    template <typename T>
    struct ISerialize
    {
      static void
      res(const std::pair<typename T::root**, BinaryISerializer*>& c)
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
        typename T::root* res = 0;
        typedef std::pair<typename T::root**, BinaryISerializer*> Cookie;
        Cookie c(&res, &ser);
        T::template dispatch<ISerialize, Cookie>(id, c);
        return reinterpret_cast<T*>(res);
      }
    };

    /*-----------.
    | Fallback.  |
    `-----------*/
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

    /*-------.
    | char.  |
    `-------*/
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

    /*---------------------.
    | unsigned int/short.  |
    `---------------------*/

# define SERIALIZE_NET_INTEGRAL_CASE(Type, Function, Size,      \
                                     LSize, LType)              \
    case LSize:                                                 \
    {                                                           \
      GD_CATEGORY(Serialize.Input.Binary);                      \
                                                                \
      LType val;                                                \
      input.read(reinterpret_cast<char*>(&val), s.Size);        \
      if (input.gcount() != s.Size)                             \
        throw Exception("Insufficient data to unserialize "     \
                        #Type);                                 \
      GD_FINFO_DUMP("Normalized: 0x%x", val);                   \
      res = val = Function(val);                                \
      GD_FINFO_DUMP("Long Value: 0x%x", res);                   \
      GD_FINFO_DUMP("Value:      0x%x (%d)", val, val);         \
      if (val != res)                                           \
        throw Exception(str(error                               \
                            % static_cast<int>(s.Size)          \
                            % #Type                             \
                            % val                               \
                            % sizeof(Type)                      \
                            % res));                            \
      return res;                                               \
    }                                                           \

# define SERIALIZE_NET_INTEGRAL(Type, Size)                     \
    template <>                                                 \
    struct BinaryISerializer::Impl<Type>                        \
    {                                                           \
      static Type                                               \
        get(const std::string&, std::istream& input,            \
            BinaryISerializer& s)                               \
      {                                                         \
        Type res;                                               \
        static boost::format error                              \
          ("overflow error: %u bytes long %s"                   \
           " with value %u doesn't fit in %u bytes"             \
           " (truncated: %u).");                                \
        switch (s.Size)                                         \
        {                                                       \
          SERIALIZE_NET_INTEGRAL_CASE(Type, ntohs, Size,        \
                                      2, uint16_t);             \
          SERIALIZE_NET_INTEGRAL_CASE(Type, ntohl, Size,        \
                                      4, uint32_t);             \
          case 8:                                               \
          {                                                     \
            union                                               \
            {                                                   \
              struct { uint32_t high; uint32_t low; } in;       \
              unsigned long long out;                           \
            } res;                                              \
            res.in.high = s.unserialize<uint32_t>();            \
            res.in.low = s.unserialize<uint32_t>();             \
            return res.out;                                     \
          }                                                     \
          default:                                              \
          {                                                     \
            GD_CATEGORY(Serialize.Input.Binary);                \
            GD_FERROR("Unexpected size %s for %s", (int)s.Size, #Type);\
            unreachable();                                      \
          }                                                     \
        }                                                       \
      }                                                         \
    };                                                          \

    SERIALIZE_NET_INTEGRAL(unsigned int,        size_int_);
    SERIALIZE_NET_INTEGRAL(unsigned long,       size_long_);
    SERIALIZE_NET_INTEGRAL(unsigned long long,  size_long_long_);
    SERIALIZE_NET_INTEGRAL(unsigned short,      size_short_);
#undef SERIALIZE_NET_INTEGRAL

    /*---------.
    | double.  |
    `---------*/
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
    template <>
    struct BinaryISerializer::Impl<float>
    {
      static float get(const std::string&, std::istream& input,
                        BinaryISerializer&)
      {
        // FIXME: non-portable
        float res;
        input.read(reinterpret_cast<char*>(&res), sizeof(float));
        return res;
      }
    };

/// Define the handling of To using the implementation for From.
#define BOUNCE(From, To)                                                \
    template <>                                                         \
    struct BinaryISerializer::Impl<From>                                \
    {                                                                   \
      static From get(const std::string& name,                          \
                      std::istream& input, BinaryISerializer& ser)      \
      {                                                                 \
        return static_cast<From>(Impl<To>::get(name, input, ser));      \
      }                                                                 \
    }

    BOUNCE(bool,          char);
    BOUNCE(unsigned char, char);
    BOUNCE(int,           unsigned);
    BOUNCE(long,          unsigned long);
    BOUNCE(long long,     unsigned long long);
    BOUNCE(short,         unsigned short);
#undef BOUNCE

    /*-----------.
    | Pointers.  |
    `-----------*/
    template <typename T>
    struct BinaryISerializer::PCImpl
    {
      static T*
      res(BinaryISerializer& ser, std::istream& input)
      {
        T* res = reinterpret_cast<T*>(new char[sizeof(T)]);
        ser.ptr_map_.push_back(res);
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
        unsigned id = ser.ptr_map_.size();
        ser.ptr_map_.push_back(0);
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
            assert_lt(id, ser.ptr_map_.size());
            return reinterpret_cast<T*>(ser.ptr_map_[id]);
          }
          case serialized:
          {
            return meta::If<meta::Inherits<T, meta::BaseHierarchy>::res,
              PHImpl<T>, PCImpl<T> >::res
              ::res(ser, input);
          }
        }
        // GCC bug prevents this: unreachable();
        // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=44580
        abort();
      }
    };

    /*-----------------.
    | Boost optional.  |
    `-----------------*/
    template <typename T>
    struct BinaryISerializer::Impl<boost::optional<T> >
    {
      typedef boost::optional<T> type;
      static type get(const std::string&, std::istream& input,
                      BinaryISerializer& ser)
      {
        pointer_status status =
          static_cast<pointer_status>(Impl<char>::get("opt", input, ser));
        switch (status)
        {
          case null:
            return 0;
          case cached:
            assert(!"Impossible 'cached' value "
                   "for a serialized boost::optional.");
          case serialized:
            return BinaryISerializer::Impl<T>::get("value", input, ser);
        }
        abort();
      }
    };

    /*------------.
    | std::pair.  |
    `------------*/
    template <typename A, typename B>
    struct BinaryISerializer::Impl<std::pair<A, B> >
    {
      typedef std::pair<A, B> type;
      static type get(const std::string&, std::istream& input,
                      BinaryISerializer& ser)
      {
        A first  = BinaryISerializer::Impl<A>::get("first",  input, ser);
        B second = BinaryISerializer::Impl<B>::get("second", input, ser);
        return type(first, second);
      }
    };

    /*--------------.
    | std::string.  |
    `--------------*/
    template <>
    struct BinaryISerializer::Impl<std::string>
    {
      static std::string get(const std::string& name, std::istream& input,
                             BinaryISerializer& ser)
      {
        size_t l = Impl<unsigned short>::get(name, input, ser);
        // FIXME: alloca
        char* buf = new char[l];
        input.read(buf, std::streamsize(l));
        if (input.gcount() != std::streamsize(l))
          throw Exception("Insufficient data to unserialize string");
        std::string res(buf, l);
        delete [] buf;
        return res;
      }
    };

    /*--------------.
    | std::vector.  |
    `--------------*/
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

    /*------------------.
    | libport::Symbol.  |
    `------------------*/
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

    /*----------------.
    | libport::hash.  |
    `----------------*/
    template <typename K, typename V>
    struct BinaryISerializer::Impl<boost::unordered_map<K, V> >
    {
      typedef boost::unordered_map<K, V> result_type;
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
    template<typename T>
    BinaryISerializer& BinaryISerializer::operator >>(T& v)
      {
        v = unserialize<T>("");
        return *this;
      }
  }
}

#endif
