#include <serialize/i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    ISerializer::ISerializer(const std::string& path)
      : stream_(path.c_str())
    {}
  }
}
