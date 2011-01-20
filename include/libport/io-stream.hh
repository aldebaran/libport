/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_IO_STREAM_HH
# define LIBPORT_IO_STREAM_HH

# include <libport/cstdio>
# include <libport/export.hh>
# include <libport/iostream>

namespace libport
{
  class StreamBuffer;

  class LIBPORT_API IOStream: public std::iostream
  {
  public:
    IOStream(StreamBuffer* buffer);
    ~IOStream();

  protected:
    friend class StreamBuffer;

  private:
    StreamBuffer* buffer_;
  };

  class LIBPORT_API StreamBuffer: public std::streambuf
  {
  public:
    typedef int fd_type;
    StreamBuffer();
    ~StreamBuffer();

  protected:
    friend class IOStream;
    virtual size_t read(char* buffer, size_t size) = 0;
    virtual void write(char* buffer, size_t size) = 0;

    virtual int underflow();
    virtual int overflow(int c);
    virtual int sync();

  private:
    char ibuf_[LIBPORT_BUFSIZ];
    char obuf_[LIBPORT_BUFSIZ];
  };

}

#endif
