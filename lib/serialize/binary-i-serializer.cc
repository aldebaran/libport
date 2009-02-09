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
      return res;
    }

    BinaryISerializer::BinaryISerializer(const std::string& path)
      : ISerializer(path)
      , stream_(path.c_str())
    {}

    BinaryISerializer::~BinaryISerializer()
    {}

    void
    BinaryISerializer::serialize(const std::string&, Serializable& s)
    {
      s.serialize(*this);
    }

    void
    BinaryISerializer::serialize(const std::string&, std::string& s)
    {
      int size = get_int(stream_);
      char* buf = new char[size + 1];
      buf[size] = 0;
      stream_.read(buf, size);
      s = buf;
    }

    void
    BinaryISerializer::serialize(const std::string&, int& i)
    {
      i = get_int(stream_);
    }

    BinaryISerializer::action_type
    BinaryISerializer::serialize_collection(const std::string&, int& size)
    {
      size = get_int(stream_);
      return action_type();
    }

  }
}
