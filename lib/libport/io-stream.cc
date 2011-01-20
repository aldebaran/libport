/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <streambuf>

#include <libport/io-stream.hh>
#include <libport/unistd.h>

namespace libport
{

  /*-----------.
  | IOStream.  |
  `-----------*/

  IOStream::IOStream(StreamBuffer* buffer)
    : std::iostream(buffer_ = buffer)
  {}

  IOStream::~IOStream()
  {
    buffer_->sync();
    delete buffer_;
  }


  /*---------------.
  | StreamBuffer.  |
  `---------------*/
  StreamBuffer::StreamBuffer()
    : ibuf_()
    , obuf_()
  {
    setg(ibuf_, ibuf_, ibuf_);
    setp(obuf_, obuf_ + LIBPORT_BUFSIZ - 1);
  }

  StreamBuffer::~StreamBuffer()
  {}

  int StreamBuffer::underflow()
  {
    ssize_t c = read(ibuf_, LIBPORT_BUFSIZ);
    if (c == 0)
    {
      setg(ibuf_, ibuf_, ibuf_);
      return EOF;
    }
    setg(ibuf_, ibuf_, ibuf_ + c);
    return static_cast<unsigned char>(ibuf_[0]);
  }

  int StreamBuffer::overflow(int c)
  {
    obuf_[LIBPORT_BUFSIZ - 1] = static_cast<char>(c);
    setp(obuf_ + LIBPORT_BUFSIZ, 0);
    sync();
    return EOF + 1; // "A value different from EOF"
  }

  int StreamBuffer::sync()
  {
    if (pptr() - obuf_)
    {
      write(obuf_, pptr() - obuf_);
      setp(obuf_, obuf_ + LIBPORT_BUFSIZ - 1);
    }
    return 0; // Success
  }

}
