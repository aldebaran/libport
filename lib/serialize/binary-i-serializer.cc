/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/debug.hh>

#include <serialize/binary-i-serializer.hh>

GD_CATEGORY(Serialize.Input.Binary);

namespace libport
{
  namespace serialize
  {
    BinaryISerializer::BinaryISerializer(std::istream& input)
      : ISerializer<BinaryISerializer>(input)
      , ptr_map_()
      , sym_map_()
    {
      GD_PUSH_TRACE("New binary input serializer");
      size_short_ = unserialize<unsigned char>("short size");
      size_int_ = unserialize<unsigned char>("int size");
      size_long_ = unserialize<unsigned char>("long size");
      size_long_long_ = unserialize<unsigned char>("long long size");
      GD_FINFO_DEBUG("short     size: %d", (int) size_short_);
      GD_FINFO_DEBUG("int       size: %d", (int) size_int_);
      GD_FINFO_DEBUG("long      size: %d", (int) size_long_);
      GD_FINFO_DEBUG("long long size: %d", (int) size_long_long_);
    }

    BinaryISerializer::~BinaryISerializer()
    {
      GD_INFO_TRACE("Delete binary input serializer");
    }
  }
}
