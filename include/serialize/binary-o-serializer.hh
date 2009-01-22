#ifndef LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_BINARY_O_SERIALIZER_HH

# include <fstream>

# include <serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API BinaryOSerializer: public OSerializer
    {
    public:

      BinaryOSerializer(const std::string& stream);
      ~BinaryOSerializer();

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);

    private:
      action_type serialize_collection(const std::string& name, int& size);

      std::ofstream stream_;
    };
  }
}

#endif
