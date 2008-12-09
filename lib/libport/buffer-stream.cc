#include "libport/buffer-stream.hh"

namespace libport
{
  namespace
  {
    class BufferBuf: public std::streambuf
    {
    public:
      BufferBuf(const char* data, size_t length);

    protected:
      virtual std::streamsize showmanyc()
        {
          return egptr() - gptr();
        }
      virtual std::streampos seekoff(std::streamoff off,
                                     std::ios_base::seekdir way,
                                     std::ios_base::openmode which)
        {
          char* cur = gptr();
          switch(way)
          {
          case std::ios_base::beg:
            cur = eback() + off;
            break;
          case std::ios_base::cur:
            cur += off;
            break;
          case std::ios_base::end:
            cur = egptr() - off;
            break;
          }
          if (cur < eback())
            cur = eback();
          if (cur > egptr())
            cur = egptr();
          setg(eback(), cur, egptr());
          return cur - eback();
        }

      virtual std::streampos seekpos(std::streampos pos,
                                     std::ios_base::openmode which)
        {
          return seekoff(pos, std::ios_base::beg, which);
        }
    };
  }

  BufferBuf::BufferBuf(const char* data, size_t length)
  {
    char* cdata = const_cast<char*>(data);
    setg(cdata, cdata, cdata+length);
    setp(0,0);
  }

  BufferStream::BufferStream(const char* data, size_t length)
    : std::iostream(new BufferBuf(data, length))
  {
  }

  BufferStream::~BufferStream()
  {
    delete rdbuf();
  }
}
