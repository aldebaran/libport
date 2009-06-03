#include <serialize/binary-i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    BinaryISerializer::BinaryISerializer(std::istream& input)
      : ISerializer<BinaryISerializer>(input)
      , ptr_id_(0)
      , ptr_map_()
    {}

    BinaryISerializer::~BinaryISerializer()
    {}
  }
}
