#ifndef LIBPORT_IO_STREAM_HXX
# define LIBPORT_IO_STREAM_HXX

namespace libport
{
  template <typename Buffer>
  IOStream<Buffer>::IOStream()
    : std::iostream(buffer_ = new Buffer)
  {}

  template <typename Buffer>
  IOStream<Buffer>::IOStream(Buffer* buffer)
    : std::iostream(buffer_ = buffer)
  {}


  template <typename Buffer>
  IOStream<Buffer>::~IOStream()
  {
    buffer_->sync();
    delete buffer_;
  }
}

#endif
