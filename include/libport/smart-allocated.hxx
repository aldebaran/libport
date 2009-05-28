#ifndef LIBPORT_SMART_ALLOCATED_HXX
# define LIBPORT_SMART_ALLOCATED_HXX

# include <exception>

namespace libport
{
  template <typename T, size_t Max>
  inline void*
  SmartAllocated<T, Max>::operator new(size_t)
  {
    static Initializer i;
//     if (free_ == 0)
//       throw std::bad_alloc();
    --free_;
    current_ = (current_ + 1) % Max;
    return map_[current_];
  }

  template <typename T, size_t Max>
  inline void
  SmartAllocated<T, Max>::operator delete(void* ptr)
  {
    map_[(current_ + free_) % Max] = (T*)ptr;
    ++free_;
  }

  template <typename T, size_t Max>
  SmartAllocated<T, Max>::Initializer::Initializer()
  {
    current_ = 0;
    free_ = Max;
    T* pool = (T*)new char[sizeof(T) * Max];
    for (unsigned i = 0; i < Max; ++i)
      map_[i] = pool + i;
  }

  template <typename T, size_t Max>
  unsigned SmartAllocated<T, Max>::current_ = Max - 1;

  template <typename T, size_t Max>
  unsigned SmartAllocated<T, Max>::free_ = Max;

  template <typename T, size_t Max>
  T* SmartAllocated<T, Max>::map_[Max];
}

#endif
