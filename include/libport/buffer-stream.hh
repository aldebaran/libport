#ifndef LIBPORT_BUFFER_STREAM_HH
# define LIBPORT_BUFFER_STREAM_HH

# include <iostream>
# include <streambuf>

# include <libport/export.hh>

namespace libport
{
  /// Input stream similar to stringstream, without copying its data source.
  class LIBPORT_API BufferStream: public std::iostream
  {
    public:
    BufferStream(const char* data, size_t length);
    virtual ~BufferStream();
  };
}

#endif
