#include <serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    OSerializer::OSerializer(const std::string& path)
      : stream_(path.c_str())
    {}
  }
}
