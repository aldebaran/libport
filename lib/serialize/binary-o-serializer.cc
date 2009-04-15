#include <arpa/inet.h>

#include <boost/bind.hpp>

#include <serialize/serializable.hh>
#include <serialize/binary-o-serializer.hh>

namespace libport
{
  namespace serialize
  {

    static void
    put_int(std::ostream& s, int i)
    {
      int normalized = htonl(i);
      s.write(reinterpret_cast<char*>(&normalized), sizeof(int));
    }

    BinaryOSerializer::BinaryOSerializer(const std::string& path)
      : OSerializer(path)
      , stream_(path.c_str())
    {}

    BinaryOSerializer::~BinaryOSerializer()
    {}

    void
    BinaryOSerializer::serialize(const std::string&, const Serializable& s)
    {
      s.serialize(*this);
    }

    void
    BinaryOSerializer::serialize(const std::string&, const std::string& s)
    {
      int size = s.size();
      put_int(stream_, size);
      stream_.write(s.c_str(), size);
    }

    void
    BinaryOSerializer::serialize(const std::string&, int i)
    {
      put_int(stream_, i);
    }

    void
    BinaryOSerializer::start_collection(const std::string&, size_t size)
    {
      put_int(stream_, size);
    }

    void
    BinaryOSerializer::end_collection()
    {}
  }
}
