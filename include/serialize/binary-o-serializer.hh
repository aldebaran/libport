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

      virtual void serialize(const std::string& name, const Serializable& value);
      virtual void serialize(const std::string& name, const std::string& value);
      virtual void serialize(const std::string& name, int value);

    private:
      virtual void start_collection(const std::string& name, size_t size);
      virtual void end_collection();
      std::ofstream stream_;
    };
  }
}

#endif
