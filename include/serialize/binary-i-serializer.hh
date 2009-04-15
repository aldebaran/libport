#ifndef LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_BINARY_I_SERIALIZER_HH

# include <fstream>

# include <serialize/i-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API BinaryISerializer: public ISerializer
    {
    public:

      BinaryISerializer(const std::string& stream);
      ~BinaryISerializer();

      virtual void unserialize(const std::string& name, std::string& s);
      virtual void unserialize(const std::string& name, int& i);

      virtual size_t start_collection(const std::string& name);
      virtual void end_collection();
    };
  }
}

#endif
