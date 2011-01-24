/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>

namespace libport
{
  template<class T, int R>
  inline
  ReservedVector<T, R>::ReservedVector()
  {
    this->reserve(R);
  }

  template<class T, int R>
  inline
  ReservedVector<T, R>::ReservedVector(const self_type& other)
    : super_type()
  {
    this->reserve(R);
    insert(this->end(), other.begin(), other.end());
  }

  template<class T, int R>
  template<typename I1, typename I2>
  inline
  ReservedVector<T, R>::ReservedVector(I1 b, I2 e)
    : super_type()
  {
    this->reserve(R);
    insert(this->end(), b, e);
  }

  template<class T, int R>
  template<int R2>
  inline
  ReservedVector<T, R>::ReservedVector(const ReservedVector<T, R2>& b)
    : super_type(b)
  {
    this->reserve(R);
  }

  template<typename T>
  inline
  void pop_front(T& v)
  {
    T v2(++v.begin(), v.end());
    v = v2;
  }

  template<typename T, typename U>
  inline
  void push_front(T& v, const U& e)
  {
    if (v.empty())
      v.push_back(e);
    else
    {
      T v2;
      v2.push_back(e);
      v2.insert(v2.end(), v.begin(), v.end());
      v = v2;
    }
  }

  template<typename T, int R>
  class ReservedVectorAllocator: public std::allocator<T>
  {
  /*-----------.
  | Typedefs.  |
  `-----------*/
  public :
    typedef ReservedVectorAllocator self_type;
    typedef std::allocator<T> super_type;
    typedef typename super_type::pointer pointer;
    typedef typename super_type::size_type size_type;
    template<typename U>
    struct rebind
    {
      typedef ReservedVectorAllocator<U, R> other;
    };

  /*-------------.
  | Construction |
  `-------------*/
  public:
    ReservedVectorAllocator()
#ifndef NDEBUG
      : local_(false)
#endif
    {

    }

  /*--------------------.
  | Memory allocation.  |
  `--------------------*/
  public:
    inline pointer
    allocate(size_type count, typename std::allocator<void>::const_pointer = 0)
    {
      if (count <= R)
      {
#ifndef NDEBUG
        assert(!local_);
        local_ = true;
#endif
        return reinterpret_cast<pointer>(pool_);
      }
      else
      {
#ifndef NDEBUG
        local_ = false;
#endif
        return reinterpret_cast<pointer>(malloc(count * sizeof(T)));
      }
    }

    inline void
    deallocate(pointer p, size_type count)
    {
      if (count > R)
      {
        assert(reinterpret_cast<pointer>(p) != reinterpret_cast<pointer>(pool_));
        free(p);
      }
      else
        assert(reinterpret_cast<pointer>(p) == reinterpret_cast<pointer>(pool_));
    }

  private:
    typedef long long align_type;
    enum { chunk_size = (sizeof(T) - 1) / sizeof(align_type) + 1 };
    align_type pool_[R * chunk_size];
#ifndef NDEBUG
    bool local_;
#endif
  };
}
