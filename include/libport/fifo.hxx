/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FIFO_HXX
# define LIBPORT_FIFO_HXX

#include <libport/compiler.hh>

#include <libport/cassert>
#include <libport/cstring>

namespace libport
{

  template<typename T, T Terminator, typename Alloc>
  inline
  Fifo<T, Terminator, Alloc>::Fifo(size_type chunk_size)
    : allocator_()
    , chunk_size_(chunk_size)
    , buffer_(allocator_.allocate(chunk_size_ * sizeof(value_type)))
    , capacity_(chunk_size_)
    , first_item_(buffer_)
    , next_item_(buffer_)
  {
    passert(buffer_, buffer_);
    *buffer_ = Terminator;
  }

  template<typename T, T Terminator, typename Alloc>
  inline
  Fifo<T, Terminator, Alloc>::~Fifo()
  {
    allocator_.deallocate(buffer_, capacity_);
  }

  template<typename T, T Terminator, typename Alloc>
  inline typename Fifo<T, Terminator, Alloc>::size_type
  Fifo<T, Terminator, Alloc>::size() const
  {
    return next_item_ - first_item_;
  }

  template<typename T, T Terminator, typename Alloc>
  inline typename Fifo<T, Terminator, Alloc>::size_type
  Fifo<T, Terminator, Alloc>::capacity() const
  {
    return capacity_;
  }

  template<typename T, T Terminator, typename Alloc>
  inline bool
  Fifo<T, Terminator, Alloc>::empty() const
  {
    return !size();
  }

  template<typename T, T Terminator, typename Alloc>
  inline void
  Fifo<T, Terminator, Alloc>::clear()
  {
    first_item_ = next_item_ = buffer_;
    *first_item_ = Terminator;
  }

  template<typename T, T Terminator, typename Alloc>
  inline void
  Fifo<T, Terminator, Alloc>::push(const_pointer data)
  {
    size_type count;
    for (count = 0; data[count] != Terminator; count++)
      ;
    push(data, count);
  }

  // C strings using the default allocator deserve a specialization,
  // as strlen() is likely to be more efficient than the hand-crafted
  // version above when the terminator is the NUL character.
  template<>
  inline void
  Fifo<char, '\0', std::allocator<char> >::push(const_pointer data)
  {
    push(data, strlen(data));
  }

  template<typename T, T Terminator, typename Alloc>
  inline void
  Fifo<T, Terminator, Alloc>::reserve(size_type nsz)
  {
    // Check whether we need to move data around, along with a possible
    // larger buffer if needed.
    if (first_item_ + nsz > buffer_ + capacity_)
    {
      // If the existing buffer is already large enough, move data
      // to the beginning, otherwise increase capacity and reallocate.
      if (nsz <= capacity_)
	memmove(buffer_, first_item_, size() * sizeof(value_type));
      else
      {
	// Increase the buffer capacity by multiples of chunk_size.
	capacity_ = chunk_size_ * (1 + (nsz - 1) / chunk_size_);

	// Rather than using realloc(), allocate a new buffer so that
	// we do not copy useless data located before first_item_.
	pointer old_buffer = buffer_;
	size_type old_capacity = capacity_;
	buffer_ = allocator_.allocate(capacity_ * sizeof(value_type));
	passert(buffer_, buffer_);
	memcpy(buffer_, first_item_, size() * sizeof(value_type));
	allocator_.deallocate(old_buffer, old_capacity * sizeof(value_type));
      }

      // Data has been moved, recompute the bounds.
      next_item_ += buffer_ - first_item_;
      first_item_ = buffer_;
    }
  }

  template<typename T, T Terminator, typename Alloc>
  inline void
  Fifo<T, Terminator, Alloc>::push(const_pointer data, size_type count)
  {
    // Compute the new size of the stored data and a null item.
    size_type nsz = size() + count + 1;
    reserve(nsz);

    // Append the new data and adjust the next item address.
    memcpy(next_item_, data, count * sizeof(value_type));
    next_item_ += count;

    // Null-terminate the stored data.
    *next_item_ = Terminator;
  }

  template<typename T, T Terminator, typename Alloc>
  inline typename Fifo<T, Terminator, Alloc>::const_pointer
  Fifo<T, Terminator, Alloc>::peek(size_type count) const
  {
    // If we want 0 items and have none, return a constant pointer
    // onto a terminator.
    static const T terminator = Terminator;
    if (!count && !size())
      return &terminator;

    // Return the data if we have enough, 0 otherwise.
    return size() < count ? 0 : first_item_;
  }

  template<typename T, T Terminator, typename Alloc>
  inline typename Fifo<T, Terminator, Alloc>::const_pointer
  Fifo<T, Terminator, Alloc>::pop(size_type count)
  {
    const_pointer res = peek(count);
    if (res)
    {
      first_item_ += count;

      // If we have just emptied the buffer, start again at the
      // beginning so that no reallocation will be needed in the near
      // future.
      if (empty())
	first_item_ = next_item_ = buffer_;
    }
    return res;
  }

} // namespace libport

#endif // LIBPORT_FIFO_HXX
