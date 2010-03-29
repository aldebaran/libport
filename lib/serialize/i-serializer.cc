/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
