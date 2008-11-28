#include <iostream>
#include <libport/fd-stream.hh>
#include <libport/cstdio>

namespace libport
{
  FdStream::FdStream(unsigned write, unsigned read)
    : std::iostream(buf_ = new FdBuf(write, read))
  {
    rdbuf(buf_);
  }

  FdStream::~FdStream()
  {
    delete buf_;
  }

  void
  FdStream::own_fd(bool v)
  {
    buf_->own_fd(v);
  }

  FdBuf::FdBuf(unsigned write, unsigned read)
    : write_(write)
    , read_(read)
    , own_(false)
  {
    setg(ibuf_, ibuf_, ibuf_);
    setp(obuf_, obuf_ + BUFSIZ - 1);
  }

  FdBuf::~FdBuf()
  {
    sync();
    if (own_)
    {
      close(write_);
      close(read_);
    }
  }

  int FdBuf::underflow()
  {
    int c = read(read_, ibuf_, BUFSIZ);
    if (c == 0)
    {
      setg(ibuf_, ibuf_, ibuf_);
      return EOF;
    }
    setg(ibuf_, ibuf_, ibuf_ + c);
    return ibuf_[0];
  }

  int FdBuf::overflow(int c)
  {
    obuf_[BUFSIZ - 1] = c;
    setp(obuf_ + BUFSIZ, 0);
    sync();
    return EOF + 1; // "A value different from EOF"
  }

  int FdBuf::sync()
  {
    if (pptr() - obuf_)
    {
      write(write_, obuf_, pptr() - obuf_);
      setp(obuf_, obuf_ + BUFSIZ - 1);
    }
    return 0; // Success
  }

  void
  FdBuf::own_fd(bool v)
  {
    own_ = v;
  }

  unsigned FdStream::fd_read()
  {
    return buf_->fd_read();
  }

  unsigned FdStream::fd_write()
  {
    return buf_->fd_write();
  }

  unsigned FdBuf::fd_read()
  {
    return read_;
  }

  unsigned FdBuf::fd_write()
  {
    return write_;
  }
}
