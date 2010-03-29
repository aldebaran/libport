/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <libport/unistd.h>
#include <libport/cstdio>

#include <libport/fd-stream.hh>

namespace libport
{
  FdStream::FdStream(fd_type write, fd_type read)
    : libport::IOStream(buf_ = new FdBuf(write, read))
  {}

  FdStream::~FdStream()
  {}

  void
  FdStream::own_fd(bool v)
  {
    buf_->own_fd_set(v);
  }

  bool
  FdStream::own_fd() const
  {
    return buf_->own_fd_get();
  }

  FdStream::fd_type FdStream::fd_read() const
  {
    return buf_->fd_read_get();
  }

  FdStream::fd_type FdStream::fd_write() const
  {
    return buf_->fd_write_get();
  }

  /*------.
  | FdBuf |
  `------*/

  FdBuf::FdBuf(fd_type write, fd_type read)
    : fd_write_(write)
    , fd_read_(read)
    , own_fd_(false)
  {}

  FdBuf::~FdBuf()
  {
    if (own_fd_)
    {
      close(fd_write_);
      close(fd_read_);
    }
  }

  size_t FdBuf::read(char* buffer, size_t max)
  {
    return ::read(fd_read_, buffer, max);
  }

  void FdBuf::write(char* buffer, size_t size)
  {
    size_t written = 0;
    while (written < size)
      written += ::write(fd_write_, buffer, size);
  }
}
