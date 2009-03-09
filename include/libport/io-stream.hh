#ifndef LIBPORT_IO_STREAM_HH
# define LIBPORT_IO_STREAM_HH

# include <iostream>

# include <libport/export.hh>

namespace libport
{
  class StreamBuffer;

  template<typename Buffer>
  class IOStream: public std::iostream
  {
  public:
    IOStream();
    IOStream(Buffer* buffer);
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
    template <typename Buffer>
    friend class IOStream;
    virtual size_t read(char* buffer, size_t size) = 0;
    virtual void write(char* buffer, size_t size) = 0;

    virtual int underflow();
    virtual int overflow(int c);
    virtual int sync();

  private:
    char ibuf_[BUFSIZ];
    char obuf_[BUFSIZ];
  };

}

# include <libport/io-stream.hxx>

#endif
