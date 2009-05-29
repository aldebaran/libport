#ifndef LIBPORT_SMART_ALLOCATED_HXX
# define LIBPORT_SMART_ALLOCATED_HXX

# include <exception>
# include <iostream>
namespace libport
{
  static const size_t ISZ = sizeof(int);
  template <size_t Max>
  inline void*
  MultiSmartAllocated<Max>::operator new(size_t s)
  {
    #define CHECK(maxs) else if (s<maxs) \
      res = SmartAllocated<char[maxs+ISZ], Max>::operator new(s)

    void* res;
    if (false) {}
    CHECK(16);
    CHECK(32);
    CHECK(64);
    CHECK(128);
    CHECK(256);
    CHECK(512);
    CHECK(1024);
    CHECK(2048);
    else
    {
      std::cerr <<"new: size of " << s <<" too big" << std::endl;
      abort();
    }
    #undef CHECK
    *(int*)res = s;
    return (char*)res + ISZ;
  }

  template <size_t Max>
  inline void
  MultiSmartAllocated<Max>::operator delete(void* ptr)
  {
    void* adjptr = (char*)ptr - ISZ;
    size_t s = *(int*)adjptr;
    //((MultiSmartAllocated<Max>*)ptr)->object_size_;
    #define CHECK(maxs) else if (s<maxs) \
      SmartAllocated<char[maxs+ISZ], Max>::operator delete(adjptr)
     if (false) {}
    CHECK(16);
    CHECK(32);
    CHECK(64);
    CHECK(128);
    CHECK(256);
    CHECK(512);
    CHECK(1024);
    CHECK(2048);
    else
    {
      std::cerr <<"delete: size of " << s <<" too big" << std::endl;
      abort();
    }
    #undef CHECK
  }

  template <typename T, size_t Max>
  inline void*
  SmartAllocated<T, Max>::operator new(size_t s)
  {
    if (sizeof(T) < s)
    {
      std::cerr <<"Requested " << s<<", blocks of " << sizeof(T) << std::endl;
      abort();
    }
    static Initializer i;
    if (current_ == 0)
    {
      std::cerr <<"empty map, alloc " << Max << std::endl;
      T* pool = (T*)malloc(sizeof(T) * Max);
      // will hold in map since its empty
      for (unsigned i = 0; i < Max; ++i)
        map_[i] = pool + i;
      current_ = Max;
    }
    --current_;
    return map_[current_];
  }

  template <typename T, size_t Max>
  inline void
  SmartAllocated<T, Max>::operator delete(void* ptr)
  {
    if (!ptr)
      return;
    if (current_ == size_)
    {
      // map is full, incredibible
      std::cerr <<"full map of " << size_ << std::endl;
      size_ += Max;
      T** new_map = (T**)malloc(sizeof(void*)*size_);
      memcpy(new_map, map_, sizeof(void*)*(size_-Max));
      free(map_);
      map_ = new_map;
      //map_ = (T**)realloc(map_, sizeof(void*)*(size_));
    }
    map_[current_++] = (T*)ptr;
  }

  template <typename T, size_t Max>
  SmartAllocated<T, Max>::Initializer::Initializer()
  {
    T* pool = (T*)malloc(sizeof(T) * Max);
    map_ = (T**)malloc(sizeof(void*) * Max);
    for (unsigned i = 0; i < Max; ++i)
      map_[i] = pool + i;
    current_ = Max; //non-existing slot, not a problem
    size_ = Max;
  }

  template <typename T, size_t Max>
  unsigned SmartAllocated<T, Max>::current_;

  template <typename T, size_t Max>
  unsigned SmartAllocated<T, Max>::size_;

  template <typename T, size_t Max>
  T** SmartAllocated<T, Max>::map_;
}

#endif
