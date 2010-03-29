/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FIFO_HH
# define LIBPORT_FIFO_HH

# include <memory>
# include <string>

namespace libport
{

/*! Fifo is an efficient FIFO queue.

  This FIFO queue can be filled in (push) or partially emptied (pop).
  Data will be moved around only when this is necessary. It works like
  this:

  +-------------------------------------------------+
  | . . . . . . . . . X X X X X X X 0 . . . . . . . |
  +-^-----------------^-------------^---------------+
  |                 |             |
  |                 |             +--- next position to write
  |                 |
  |                 +--- first data item
  |
  +--- beginning of the storage area

  New data is entered after the existing one (marked by X), and a
  terminator item is added at the end so that the whole buffer can be
  considered as being a terminated structure when retrieved as a
  block. Data is always kept in consecutive positions and in the right
  order, making it easy to see it as an array in memory.

  If there is not enough room after the existing data, the data will
  be moved to the beginning of the buffer, after a reallocation if
  necessary.

  When data is retrieved, the position of the first item to read
  is moved forward. If the buffer happens to be emptied, the markers
  will be reset to the beginning of the buffer so that we are less
  likely to require a data relocation in the future.
*/

  template<typename T, T Terminator, typename Alloc = std::allocator<T> >
  class Fifo
  {
  public:
    typedef T value_type;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_reference const_reference;
    typedef typename Alloc::const_pointer const_pointer;
    typedef size_t size_type;
    typedef Alloc allocator_type;

    //! Fifo constructor.
    //
    // \param chunk_size The size of allocated chunks. The buffer will start
    //                   at that size and increment by multiples of the same
    //                   amount when needed.
    Fifo(size_type chunk_size = 1024);

    //! Fifo destructor.
    virtual ~Fifo();

    // ! Push a terminated array into the queue. The terminator is ignored.
    /*
      \param data The data to push.

      \sa push(const_pointer,size_type)
    */
    void push(const_pointer data);

    //! Push a counted array into the queue.
    /*
      \param data  The data to push.
      \param count The number of items to push.
    */
    void push(const_pointer buffer, size_type count);

    //! Pop objects from the queue.
    /* The returned pointer is valid until further calls to push().
       The data is not terminated unless the whole content has been
       requested.

      \param count Number of items requested.

      \return A pointer to the the data popped or 0 in case of error.
    */
    const_pointer pop(size_type count);

    //! Peek into the queue.
    /*! Behave like pop() but do not consume data. The data is not
      null-terminated unless the whole content is requested.

      Since obviously there are at least 0 items in the queue,
      using 0 for the length (the default) will give the full
      terminated content of the queue.

      \param count Number of items to retrieve.

      \return A pointer to the the data popped or 0 if there are
              not enough items in the queue.
    */
    const_pointer peek(size_type count = 0) const;

    //! Clear the queue.
    void clear();

    //! Check if the queue is empty.
    /*
      \return True if the queue is empty.
    */
    bool empty() const;

    //! Check the queue size.
    /*
      \return The number of items in the queue.
    */
    size_type size() const;

    //! Check the queue capacity.
    /*
      \return The capacity of the queue. To know what you can store in
              it, do not forget that the extra terminator is included in
	      this value.
    */
    size_type capacity() const;

    //! Change the queue capacity so that it can hold at least \b len bytes.
    void reserve(size_type len);

  private:
    /// Allocator.
    allocator_type allocator_;

    /// Chunk size.
    const size_type chunk_size_;

    /// Internal buffer.
    pointer buffer_;

    /// Reserved size.
    size_type capacity_;

    /// Address of first item in the buffer.
    pointer first_item_;

    /// Address of next item to write, following the last one
    /// in the buffer.
    pointer next_item_;
  };

} // namespace libport

# include <libport/fifo.hxx>

#endif // LIBPORT_FIFO_HH
