#include <serialize/binary-o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    BinaryOSerializer::BinaryOSerializer(std::ostream& output)
      : OSerializer<BinaryOSerializer>(output)
      , ptr_id_(0)
      , ptr_map_()
      , symbol_id_(0)
      , symbol_map_()
    {}

    BinaryOSerializer::~BinaryOSerializer()
    {}
  }
}
