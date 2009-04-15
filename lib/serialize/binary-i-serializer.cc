#include <arpa/inet.h>

#include <boost/bind.hpp>

#include <serialize/serializable.hh>
#include <serialize/binary-i-serializer.hh>

namespace libport
{
  namespace serialize
  {

    static int
    get_int(std::istream& s)
    {
      int res;
      s.read(reinterpret_cast<char*>(&res), sizeof(int));
      return ntohl(res);
    }

    BinaryISerializer::BinaryISerializer(const std::string& path)
      : ISerializer(path)
    {}

    BinaryISerializer::~BinaryISerializer()
    {}

    void
    BinaryISerializer::unserialize(const std::string&, std::string& s)
    {
      int size = get_int(stream_);
      char* buf = new char[size + 1];
      buf[size] = 0;
      stream_.read(buf, size);
      s = buf;
    }

    void
    BinaryISerializer::unserialize(const std::string&, int& i)
    {
      i = get_int(stream_);
    }

    size_t
    BinaryISerializer::start_collection(const std::string&)
    {
      return get_int(stream_);
    }

    void
    BinaryISerializer::end_collection()
    {}
  }
}
