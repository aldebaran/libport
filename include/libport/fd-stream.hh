#ifndef LIBPORT_FD_STREAM_HH
# define LIBPORT_FD_STREAM_HH

# include <iostream>
# include <streambuf>

namespace libport
{
  class FdBuf: public std::streambuf
  {
  public:
    FdBuf(unsigned fd);
    ~FdBuf();

  protected:
    virtual int underflow();
    virtual int overflow(int c = EOF );
    virtual int sync();
  private:
    unsigned fd_;
    char ibuf_[BUFSIZ];
    char obuf_[BUFSIZ];
  };

  class FdStream: public std::iostream
  {
  public:
    FdStream(unsigned fd);
    virtual ~FdStream();
  private:
    FdBuf* buf_;
  };
}

#endif
