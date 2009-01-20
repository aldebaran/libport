#include <serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    OSerializer::OSerializer(const std::string& path)
      : Serializer(false)
      , path_(path)
    {}
  }
}
