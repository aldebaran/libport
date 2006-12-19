// Hash maps, depending on the environment

#ifndef LIBPORT_HASH_HH
# define LIBPORT_HASH_HH

# if defined __GNUG__ // G++
#  include <ext/hash_map>
# elif defined _MSC_VER
#  if (_MSC_VER == 1400)
#    pragma warning( disable : 4355 4996)
#  endif
#  include <hash_map>
# else
#  error Do not know where hash_map is.
# endif

# include <string>
# include "libport/cstring"


// A quick hack to be able to use hash_map with string easily

# ifndef _MSC_VER

#  if (__GNUC__ == 2)
  __STL_BEGIN_NAMESPACE
#  else
  namespace __gnu_cxx
  {
#  endif

template<>
struct hash<std::string>
{
  size_t operator() (const std::string& x) const
  {
    return hash<const char*>() (x.c_str());
  }
};

#  if (__GNUC__ == 2)
  __STL_END_NAMESPACE
#  else
  }
#  endif

namespace libport
{

  //! Used in the hash_map object to define equality of two variable names
  struct eqStr
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return STREQ(s1, s2);
    }
  };

  template<class K, class V>
  class hash_map_type
  {
  public:
    typedef __gnu_cxx::hash_map<K, V> type;
  };

  template<class V>
  class hash_map_type<const char *, V>
  {
  public:
    typedef __gnu_cxx::hash_map<const char *, V,
				__gnu_cxx::hash<const char *>,
				eqStr> type;
  };
}

# else //_MSC_VER

#  if (_MSC_VER == 1400)
#   define HASH_NS stdext
_STDEXT_BEGIN
#  else
#   define HASH_NS std
_STD_BEGIN
#  endif
//msc does not define a hash function for hash_compare

template<>
class hash_compare<const char*>
{
 public:
  enum
  {
    // parameters for hash table
    bucket_size = 4,	// 0 < bucket_size
    min_buckets = 8
  };	// min_buckets = 2 ^^ N, 0 < N

  size_t operator ()(const char *c) const
  {
    size_t r = 0;
    while (*c!=0)
      {
	r = (*c)+31*r;
	c++;
      }
    return r;
  }
  bool operator()(const char* _Keyval1, const char* _Keyval2) const
  {
    // Whether _Keyval1 < _Keyval2.
    return strcmp(_Keyval1, _Keyval2) < 0;
  }
};

template<>
class hash_compare<std::string>
{
 public:
  enum
  {
    // parameters for hash table
    bucket_size = 4,	// 0 < bucket_size
    min_buckets = 8
  };	// min_buckets = 2 ^^ N, 0 < N

  size_t
  operator()( const std::string& x ) const
  { return hash_compare<const char*>()( x.c_str() );}

  bool
  operator()(const std::string& _Keyval1, const std::string& _Keyval2) const
  {
    return _Keyval1 < _Keyval2;
  }
};

#  if (_MSC_VER == 1400)
_STDEXT_END
#  else
_STD_END
#  endif
namespace libport
{
  template<class K, class V>
  class hash_map_type
  {
  public:
    typedef ::HASH_NS::hash_map<K, V> type;
  };

}
#  undef HASH_NS
# endif // _MSC_VER

#endif // !LIBPORT_HASH_HH
