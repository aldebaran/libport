/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_SERIALIZE_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_O_SERIALIZER_HH

# include <ostream>
# include <string>

# include <libport/traits.hh>

namespace libport
{
  namespace serialize
  {
    template <class Exact>
    class OSerializer
    {
    public:
      OSerializer(std::ostream& output);
      ~OSerializer();
      template <typename T>
      void serialize(const std::string& name, typename traits::Arg<T>::res v);

    private:
      std::ostream& stream_;
    };
  }
}

# include <serialize/o-serializer.hxx>

#endif
