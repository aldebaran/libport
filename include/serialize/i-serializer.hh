/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_I_SERIALIZER_HH

# include <istream>
# include <string>

# include <libport/hierarchy.hh>

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    class ISerializer
    {
    public:
      ISerializer(std::istream& input);
      ~ISerializer();
      template <typename T>
      typename meta::If<meta::Inherits<T, meta::BaseHierarchy>::res, T*, T>::res
      unserialize(const std::string& name);

    private:
      std::istream& stream_;
    };
  }
}

# include <serialize/i-serializer.hxx>

#endif
