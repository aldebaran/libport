#include <iostream>
#include <libport/fd-stream.hh>

namespace libport
{
  FdStream::FdStream(unsigned fd)
    : buf_(new FdBuf(fd))
  {
    rdbuf(buf_);
  }

  FdStream::~FdStream()
  {
    delete buf_;
  }

  FdBuf::FdBuf(unsigned fd)
    : fd_(fd)
  {
    setg(ibuf_, ibuf_, ibuf_);
    setp(obuf_, obuf_ + BUFSIZ - 1);
  }

  FdBuf::~FdBuf()
  {
    sync();
  }

  int FdBuf::underflow()
  {
    int c = read(fd_, ibuf_, BUFSIZ);
    if (c == 0)
    {
      setg(ibuf_, ibuf_, ibuf_);
      return EOF;
    }
    setg(ibuf_, ibuf_, ibuf_ + c);
    return 'x';
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
    write(fd_, obuf_, pptr() - obuf_);
    setp(obuf_, obuf_ + BUFSIZ - 1);
    return 0; // Success
  }
}
