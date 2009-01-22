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

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);

    private:
      action_type serialize_collection(const std::string& name, int& size);

      std::ifstream stream_;
    };
  }
}

#endif
