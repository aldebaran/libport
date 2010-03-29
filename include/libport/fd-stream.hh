/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FD_STREAM_HH
# define LIBPORT_FD_STREAM_HH

# include <iostream>
# include <streambuf>

# include <libport/attributes.hh>
# include <libport/export.hh>
# include <libport/io-stream.hh>

namespace libport
{
  class LIBPORT_API FdBuf: public libport::StreamBuffer
  {
  public:
    typedef int fd_type;
    FdBuf(fd_type write, fd_type read);
    ~FdBuf();

    ATTRIBUTE_R (fd_type, fd_write);
    ATTRIBUTE_R (fd_type, fd_read);
    ATTRIBUTE_RW(bool,    own_fd);

  protected:
    virtual size_t read(char* buffer, size_t max);
    virtual void write(char* buffer, size_t size);
  };

  /// Standard stream that reads from and writes to file descriptors.
  class LIBPORT_API FdStream: public libport::IOStream
  {
  public:
    typedef int fd_type;
    /// Create a FdStream that writes to fd \a write and reads from fd \a read
    FdStream(fd_type write, fd_type read);
    /// Destroy a FdStream
    virtual ~FdStream();
    /// Set whether the FdStream should close the fds upon destruction
    void own_fd(bool v);
    bool own_fd() const;
    /// Get write fd.
    fd_type fd_write() const;
    /// Get read fd.
    fd_type fd_read() const;

  private:
    FdBuf* buf_;
  };
}

#endif
