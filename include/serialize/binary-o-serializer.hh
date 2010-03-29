/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HH

# include <libport/hash.hh>
# include <libport/symbol.hh>
# include <serialize/export.hh>
# include <serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API BinaryOSerializer
      : public OSerializer<BinaryOSerializer>
    {
    public:
      BinaryOSerializer(std::ostream& output);
      ~BinaryOSerializer();
      template <typename T>
      struct Impl;

    private:
      typedef boost::unordered_map<long, unsigned> ptr_map_type;
      unsigned ptr_id_;
      ptr_map_type ptr_map_;

      typedef boost::unordered_map<Symbol, unsigned> symbol_map_type;
      unsigned symbol_id_;
      symbol_map_type symbol_map_;
    };
  }
}

# include <serialize/binary-o-serializer.hxx>

#endif
