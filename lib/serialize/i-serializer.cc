#include <serialize/i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    ISerializer::ISerializer(std::istream& input)
      : stream_(input)
      , ids_()
      , id_(0)
    {}

    ISerializer::~ISerializer()
    {}
  }
}
