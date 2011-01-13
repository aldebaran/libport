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

#include <serialize/binary-o-serializer.hh>

GD_CATEGORY(Serialize.Output.Binary);

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
    {
      GD_PUSH_TRACE("New binary output serializer");
      GD_FINFO_DEBUG("short     size: %s", sizeof(short));
      GD_FINFO_DEBUG("int       size: %s", sizeof(int));
      GD_FINFO_DEBUG("long      size: %s", sizeof(long));
      GD_FINFO_DEBUG("long long size: %s", sizeof(long long));

      serialize<unsigned char>("short size", sizeof(short));
      serialize<unsigned char>("int size", sizeof(int));
      serialize<unsigned char>("long size", sizeof(long));
      serialize<unsigned char>("long long size", sizeof(long long));
    }

    BinaryOSerializer::~BinaryOSerializer()
    {}
  }
}
