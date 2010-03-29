/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
