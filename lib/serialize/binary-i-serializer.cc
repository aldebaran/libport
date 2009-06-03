#include <serialize/binary-i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    BinaryISerializer::BinaryISerializer(std::istream& input)
      : ISerializer<BinaryISerializer>(input)
      , ptr_map_()
      , sym_map_()
    {}

    BinaryISerializer::~BinaryISerializer()
    {}
  }
}
