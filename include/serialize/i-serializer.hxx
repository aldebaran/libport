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
#include <libport/escape.hh>

#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HXX
# define LIBPORT_SERIALIZE_I_SERIALIZER_HXX

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    ISerializer<Exact>::ISerializer(std::istream& input)
      : stream_(input)
    {}

    template <class Exact>
    ISerializer<Exact>::~ISerializer()
    {}

    template <class Exact>
    template <typename T>
    typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res, T*, T>::res
    ISerializer<Exact>::unserialize(const std::string& name)
    {
      GD_CATEGORY(Serialize.Input);
      GD_FINFO_TRACE("Unserialize %s with name \"%s\"",
                     typeid(T).name(), libport::escape(name));
      return Exact::template Impl<T>::get(name, stream_,
                                          static_cast<Exact&>(*this));
    }
  }
}

#endif
