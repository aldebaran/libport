/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

// Hash maps, depending on the environment

#ifndef LIBPORT_HASH_HH
# define LIBPORT_HASH_HH

// Define LIBPORT_HASH_NAMESPACE to the namespace name that contains
// hash_map, and include the right header.

# include <string>
# include <libport/cstring>

# include <libport/symbol.hh>

/*-----------.
| GCC part.  |
`-----------*/

# if defined __GNUC__

#  if (__GNUC__ == 2)
#    define LIBPORT_HASH_NAMESPACE stl
#  else
#    define LIBPORT_HASH_NAMESPACE __gnu_cxx
#  endif
#  ifdef __DEPRECATED
// Do not warn that ext/hash_map is deprecated
#   undef __DEPRECATED
#   include <ext/hash_map>
#   include <ext/hash_set>
#   define __DEPRECATED
#  else
#   include <ext/hash_map>
#   include <ext/hash_set>
#  endif

namespace LIBPORT_HASH_NAMESPACE
{

  // Be able to use hash_map with string easily.
  template<>
  struct hash<std::string>
  {
    size_t operator() (const std::string& x) const
    {
      return hash<const char*>() (x.c_str());
    }
  };

  template<>
  struct hash<libport::Symbol>
  {
    size_t operator() (const libport::Symbol& x) const
    {
      // The address of the string is unique, hash on it.
      return reinterpret_cast<size_t>(&x.name_get());
    }
  };

} // namespace LIBPORT_HASH_NAMESPACE

namespace std
{
  /// Used in the hash_map object to define equality of two variable names.
  template <>
  struct equal_to<const char *>
  {
    bool operator()(const char* s1, const char* s2) const
    {
      return ::libport::streq(s1, s2);
    }
  };

} // namespace std

# elif defined _MSC_VER

/*------------.
| VC++ part.  |
`------------*/

#  if (_MSC_VER >= 1400)
#   define LIBPORT_HASH_NAMESPACE stdext
#   pragma warning( disable : 4355 4996)
#  else
#   define LIBPORT_HASH_NAMESPACE std
#  endif
#  include <hash_map>
#  include <hash_set>

namespace LIBPORT_HASH_NAMESPACE
{
  //msc does not define a hash function for hash_compare

  /*--------.
  | char*.  |
  `--------*/
  template<typename T>
  class hash_compare<const char*, T>
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

  /*--------------.
  | std::string.  |
  `--------------*/
  template<typename T>
  class hash_compare<std::string, T>
  {
    public:
    enum
    {
      // parameters for hash table
      bucket_size = 4,	// 0 < bucket_size
      min_buckets = 8
    };	// min_buckets = 2 ^^ N, 0 < N

    size_t
    operator() (const std::string& x) const
    {
      return hash_compare<const char*>()(x.c_str());
    }

    bool
    operator()(const std::string& _Keyval1, const std::string& _Keyval2) const
    {
      return _Keyval1 < _Keyval2;
    }
  };


  /*------------------.
  | libport::Symbol.  |
  `------------------*/
  template<typename T>
  class hash_compare<libport::Symbol, T>
  {
    public:
    enum
    {
      // parameters for hash table
      bucket_size = 4,	// 0 < bucket_size
      min_buckets = 8
    };	// min_buckets = 2 ^^ N, 0 < N

    size_t
    operator() (const libport::Symbol& x) const
    {
      // The address of the string is unique, hash on it.
      return reinterpret_cast<size_t>(&x.name_get());
    }

    bool
    operator()(const libport::Symbol& _Keyval1,
	       const libport::Symbol& _Keyval2) const
    {
      return _Keyval1 < _Keyval2;
    }
  };

} // namespace LIBPORT_HASH_NAMESPACE

# else
#  error "Do not know where hash_map/hash_set are."
# endif


/*----------------------------.
| Compiler independent part.  |
`----------------------------*/

namespace libport
{
  using LIBPORT_HASH_NAMESPACE::hash_map;

  using LIBPORT_HASH_NAMESPACE::hash_set;

}

# undef LIBPORT_HASH_NAMESPACE

#endif // !LIBPORT_HASH_HH
