/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_VECTOR_HH
# define LIBPORT_VECTOR_HH

# include <libport/cstdlib>
# include <iostream>
# include <string>

# include <libport/cassert>
# include <libport/cast.hh>

namespace libport
{

  template <typename T>
  class Allocator
  {
    public:
      ATTRIBUTE_ALWAYS_INLINE
      ATTRIBUTE_UNUSED_RESULT
      ATTRIBUTE_MALLOC
      T* allocate(unsigned count)
      {
        return reinterpret_cast<T*>(malloc(count * sizeof(T)));
      }

      ATTRIBUTE_ALWAYS_INLINE
      void deallocate(void* p, unsigned)
      {
        free(p);
      }

      ATTRIBUTE_ALWAYS_INLINE
      T* grow(void*, unsigned count)
      {
        // FIXME: we could realloc, but only if it doesn't move ...
        return reinterpret_cast<T*>(malloc(count * sizeof(T)));
      }

      ATTRIBUTE_ALWAYS_INLINE
      T* shrink(void*, unsigned count)
      {
        // FIXME: we could realloc, but only if it doesn't move ...
        return reinterpret_cast<T*>(malloc(count * sizeof(T)));
      }
  };

  template <typename T, unsigned Floor>
  class FlooredAllocator: public Allocator<T>
  {
    public:
      typedef Allocator<T> super_type;

      ATTRIBUTE_ALWAYS_INLINE
      ATTRIBUTE_UNUSED_RESULT
      ATTRIBUTE_MALLOC
      T* allocate(unsigned count)
      {
        if (count <= Floor)
          return union_cast<Align*, T*>(buffer_);
        return super_type::allocate(count);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void deallocate(void* p, unsigned count)
      {
        if (count > Floor)
          super_type::deallocate(p, count);
      }

      ATTRIBUTE_ALWAYS_INLINE
      T* grow(void* p, unsigned count)
      {
        if (count <= Floor)
          return 0;
        return super_type::grow(p, count);
      }

      ATTRIBUTE_ALWAYS_INLINE
      T* shrink(void* p, unsigned count)
      {
        if (count <= Floor)
        {
          if (p == buffer_)
            return 0;
          else
            return union_cast<Align*, T*>(buffer_);
        }
        return super_type::shrink(p, count);
      }

    private:
      typedef long long Align;
      Align buffer_[((sizeof(T) - 1) / sizeof(Align) + 1) * Floor];
  };

  template <typename T>
  class Constructor
  {
    public:
      ATTRIBUTE_ALWAYS_INLINE
      void construct(void* m)
      {
        new (m) T();
      }

      ATTRIBUTE_ALWAYS_INLINE
      void construct(void* m, unsigned count)
      {
        for (unsigned i = 0; i < count; ++i)
          construct(reinterpret_cast<T*>(m) + i);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void construct(void* m, const T& val)
      {
        new (m) T(val);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void construct(void* m, unsigned count, const T& val)
      {
        for (unsigned i = 0; i < count; ++i)
          construct(reinterpret_cast<T*>(m) + i, val);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void destroy(T* v)
      {
        v->~T();
      }

      ATTRIBUTE_ALWAYS_INLINE
      void destroy(void* m, unsigned count)
      {
        for (unsigned i = 0; i < count; ++i)
          destroy(reinterpret_cast<T*>(m) + i);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void move(T* from, void* to)
      {
        construct(to, *from);
        destroy(from);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void move(T* from, void* to, unsigned count)
      {
        for (unsigned i = 0; i < count; ++i)
          move(from + i, reinterpret_cast<T*>(to) + i);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void rmove(T* from, void* to, unsigned count)
      {
        for (unsigned i = count - 1; i < count; --i)
          move(from + i, reinterpret_cast<T*>(to) + i);
      }
  };

  class ExponentialCapacity
  {
    public:
      ATTRIBUTE_ALWAYS_INLINE
      ExponentialCapacity(unsigned size)
        : size_(0)
      {
        grow(size);
      }

      ATTRIBUTE_ALWAYS_INLINE
      void
      grow(unsigned size)
      {
        if (!size_)
          size_ = 1;
        while (size_ < size)
          size_ <<= 1;
      }

      ATTRIBUTE_ALWAYS_INLINE
      bool
      shrink(unsigned size)
      {
        bool res = false;
        while (size_ && size_ >> 1 >= size)
        {
          size_ >>= 1;
          res = true;
        }
        return res;
      }

      ATTRIBUTE_ALWAYS_INLINE
      unsigned
      size() const
      {
        return size_;
      }

    protected:
      unsigned size_;
  };

  template <unsigned Floor>
  class FlooredExponentialCapacity: public ExponentialCapacity
  {
    public:
      ATTRIBUTE_ALWAYS_INLINE
      FlooredExponentialCapacity(unsigned size)
        : ExponentialCapacity(size > Floor ? size : Floor)
      {}

      ATTRIBUTE_ALWAYS_INLINE
      void
      grow(unsigned size)
      {
        aver_ge(size, Floor);
        ExponentialCapacity::grow(size);
      }

      ATTRIBUTE_ALWAYS_INLINE
      bool
      shrink(unsigned size)
      {
        return ExponentialCapacity::shrink(size >= Floor ? size : Floor);
      }
  };

  template <typename T,
            typename AllocationPolicy = Allocator<T>,
            typename ConstructionPolicy = Constructor<T>,
            typename CapacityPolicy = ExponentialCapacity>
  class Vector
  {
    public:
      typedef Vector<T, AllocationPolicy, ConstructionPolicy, CapacityPolicy>
      self_type;
      typedef T value_type;
      typedef T* iterator;
      typedef const T* const_iterator;
      typedef T& reference;
      typedef const T& const_reference;
      typedef size_t size_type;

      Vector()
        : allocation_()
        , construction_()
        , capacity_(0)
        , size_(0)
        , data_(allocation_.allocate(capacity_.size()))
      {}

      template <typename It>
      Vector(It begin, It end)
        : allocation_()
        , construction_()
        , capacity_(end - begin)
        , size_(end - begin)
        , data_(allocation_.allocate(capacity_.size()))
      {
        aver_le(begin, end);
        T* data = data_;
        for (; begin != end; ++begin)
          construction_.construct(data++, *begin);
      }

      Vector(const self_type& model)
        : allocation_()
        , construction_()
        , capacity_(model.size())
        , size_(0)
        , data_(allocation_.allocate(capacity_.size()))
      {
        T* data = data_;
        size_ = model.size();
        for (const_iterator it = model.begin(); it != model.end(); ++it)
          construction_.construct(data++, *it);
      }

      ~Vector()
      {
        clear();
      }

      void push_back(const_reference v)
      {
        if (size_ == capacity_.size())
        {
          capacity_.grow(size_ + 1);
          if (void* data = allocation_.grow(data_, capacity_.size()))
          {
            construction_.move(data_, data, size_);
            allocation_.deallocate(data_, size_);
            data_ = reinterpret_cast<T*>(data);
          }
        }
        construction_.construct(data_ + size_, v);
        ++size_;
      }

      void push_front(const_reference v)
      {
        insert(begin(), v);
      }

      self_type&
      operator <<(const_reference v)
      {
        push_back(v);
        return *this;
      }

      void pop_back()
      {
        --size_;
        construction_.destroy(data_ + size_);
        if (capacity_.shrink(size_))
          if (void* data = allocation_.shrink(data_, capacity_.size()))
          {
            construction_.move(data_, data, size_);
            allocation_.deallocate(data_, size_);
            data_ = reinterpret_cast<T*>(data);
          }
      }

      void pop_front()
      {
        assert(size_);
        erase(begin());
      }

      void clear()
      {
        construction_.destroy(data_, size_);
        if (capacity_.shrink(0))
          if (void* data = allocation_.shrink(data_, capacity_.size()))
          {
            allocation_.deallocate(data_, size_);
            data_ = reinterpret_cast<T*>(data);
          }
        size_ = 0;
      }

      unsigned size() const
      {
        return size_;
      }

      bool empty() const
      {
        return size_ == 0;
      }

      reference
      front()
      {
        assert(size_);
        return data_[0];
      }

      const_reference
      front() const
      {
        return data_[0];
      }

      reference
      back()
      {
        assert(size_);
        return data_[size_ - 1];
      }

      const_reference
      back() const
      {
        return data_[size_ - 1];
      }

      iterator begin()
      {
        return data_;
      }

      const_iterator begin() const
      {
        return const_cast<self_type*>(this)->begin();
      }

      iterator end()
      {
        return data_ + size_;
      }

      const_iterator end() const
      {
        return const_cast<self_type*>(this)->end();
      }

      iterator erase(iterator it)
      {
        aver_le_lt(data_, it, data_ + size_);
        construction_.destroy(it);
        const unsigned before = it - data_;
        const unsigned after  = size_ - before - 1;
        if (capacity_.shrink(size_ - 1))
        {
          if (void* data = allocation_.shrink(data_, capacity_.size()))
          {
            construction_.move(data_, data, before);
            construction_.move(data_ + before + 1,
                               reinterpret_cast<T*>(data) + before, after);
            allocation_.deallocate(data_, size_);
            --size_;
            data_ = reinterpret_cast<T*>(data);
            return data_ + before;
          }
          else
          {
            // FIXME: last was free'd!
            construction_.move(data_ + before + 1, data_ + before, after);
            --size_;
            return it;
          }
        }
        else
        {
          construction_.move(it + 1, it, after);
          --size_;
          return it;
        }
      }

      void insert(iterator it, const_reference v)
      {
        const unsigned before = it - data_;
        const unsigned after  = size_ - before;

        if (size_ == capacity_.size())
        {
          capacity_.grow(size_ + 1);
          if (T* data = allocation_.grow(data_, capacity_.size()))
          {
            construction_.move(data_, data, before);
            construction_.move(data_ + before, data + before + 1, after);
            allocation_.deallocate(data_, size_);
            data_ = reinterpret_cast<T*>(data);
            construction_.construct(data_ + before, v);
            ++size_;
            return;
          }
        }
        construction_.rmove(it, it + 1, after);
        construction_.construct(it, v);
        ++size_;
      }

      reference operator[] (unsigned idx)
      {
        assert_lt(idx, size_);
        return reinterpret_cast<T*>(data_)[idx];
      }

      const_reference operator[] (unsigned idx) const
      {
        return (*const_cast<self_type*>(this))[idx];
      }
    private:
      AllocationPolicy allocation_;
      ConstructionPolicy construction_;
      CapacityPolicy capacity_;
      unsigned size_;
      T*       data_;
  };
}

#endif
