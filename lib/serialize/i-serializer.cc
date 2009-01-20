#include <serialize/i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    ISerializer::ISerializer(const std::string& path)
      : Serializer(true)
      , path_(path)
    {}
  }
}
