#ifndef LIBPORT_FD_STREAM_HH
# define LIBPORT_FD_STREAM_HH

# include <iostream>
# include <streambuf>

# include <libport/export.hh>

namespace libport
{
  class LIBPORT_API FdBuf: public std::streambuf
  {
  public:
    FdBuf(unsigned write, unsigned read);
    ~FdBuf();
    void own_fd(bool v);
    bool own_fd() const;
    unsigned fd_write();
    unsigned fd_read();

  protected:
    virtual int underflow();
    virtual int overflow(int c = EOF);
    virtual int sync();
  private:
    unsigned write_, read_;
    bool own_;
    char ibuf_[BUFSIZ];
    char obuf_[BUFSIZ];
  };

  /// Standard stream that reads from and writes to file descriptors.
  class LIBPORT_API FdStream: public std::iostream
  {
  public:
    /// Create a FdStream that writes to fd \a write and reads from fd \a read
    FdStream(unsigned write, unsigned read);
    /// Destroy a FdStream
    virtual ~FdStream();
    /// Set whether the FdStream should close the fds upon destruction
    void own_fd(bool v);
    bool own_fd() const;
    /// Get write fd
    unsigned fd_write();
    /// Get read fd
    unsigned fd_read();

  private:
    FdBuf* buf_;
  };
}

#endif
