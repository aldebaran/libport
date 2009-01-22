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
      s.write(reinterpret_cast<char*>(&i), sizeof(int));
    }

    BinaryOSerializer::BinaryOSerializer(const std::string& path)
      : OSerializer(path)
      , stream_(path.c_str())
    {}

    BinaryOSerializer::~BinaryOSerializer()
    {}

    void
    BinaryOSerializer::serialize(const std::string&, Serializable& s)
    {
      s.serialize(*this);
    }

    void
    BinaryOSerializer::serialize(const std::string&, std::string& s)
    {
      int size = s.size();
      put_int(stream_, size);
      stream_.write(s.c_str(), size);
    }

    BinaryOSerializer::action_type
    BinaryOSerializer::serialize_collection(const std::string&, int& size)
    {
      put_int(stream_, size);
      return action_type();
    }

  }
}
