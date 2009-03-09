#include <streambuf>

#include <libport/io-stream.hh>
#include <libport/unistd.h>

namespace libport
{

  StreamBuffer::StreamBuffer()
    : ibuf_()
    , obuf_()
  {
    setg(ibuf_, ibuf_, ibuf_);
    setp(obuf_, obuf_ + BUFSIZ - 1);
  }

  StreamBuffer::~StreamBuffer()
  {  }

  int StreamBuffer::underflow()
  {
    ssize_t c = read(ibuf_, BUFSIZ);
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
    obuf_[BUFSIZ - 1] = static_cast<char>(c);
    setp(obuf_ + BUFSIZ, 0);
    sync();
    return EOF + 1; // "A value different from EOF"
  }

  int StreamBuffer::sync()
  {
    if (pptr() - obuf_)
    {
      write(obuf_, pptr() - obuf_);
      setp(obuf_, obuf_ + BUFSIZ - 1);
    }
    return 0; // Success
  }

}
