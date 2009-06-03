#ifndef LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HH

# include <libport/hash.hh>
# include <serialize/i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API BinaryISerializer: public ISerializer<BinaryISerializer>
    {
    public:
      BinaryISerializer(std::istream& input);
      ~BinaryISerializer();
      template <typename T>
      struct Impl;

    private:
      template <typename T>
      struct PCImpl;
      template <typename T>
      struct PHImpl;

      typedef libport::hash_map<unsigned, void*> ptr_map_type;
      unsigned ptr_id_;
      ptr_map_type ptr_map_;

      typedef std::vector<libport::Symbol> symbol_map_type;
      symbol_map_type sym_map_;

      template <typename T>
      friend class PCImpl;
      template <typename T>
      friend class PHImpl;
    };
  }
}

# include <serialize/binary-i-serializer.hxx>

#endif
