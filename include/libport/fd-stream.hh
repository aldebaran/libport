#ifndef LIBPORT_FD_STREAM_HH
# define LIBPORT_FD_STREAM_HH

# include <iostream>
# include <streambuf>

namespace libport
{
  class FdBuf: public std::streambuf
  {
  public:
    FdBuf(unsigned write, unsigned read);
    ~FdBuf();
    void own_fd(bool v);
    unsigned fd_write();
    unsigned fd_read();

  protected:
    virtual int underflow();
    virtual int overflow(int c = EOF );
    virtual int sync();
  private:
    unsigned write_, read_;
    bool own_;
    char ibuf_[BUFSIZ];
    char obuf_[BUFSIZ];
  };

  class FdStream: public std::iostream
  {
  public:
    FdStream(unsigned write, unsigned read);
    virtual ~FdStream();
    void own_fd(bool v);
    unsigned fd_write();
    unsigned fd_read();

  private:
    FdBuf* buf_;
  };
}

#endif
