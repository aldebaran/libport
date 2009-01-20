#ifndef LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH

# include <fstream>

# include <libport/serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API XmlOSerializer: public OSerializer
    {
    public:

      XmlOSerializer(const std::string& stream);

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);

    private:
      action_type serialize_collection(const std::string& name);
      void serialize_collection_end(const std::string& name);

      void indent();
      std::ofstream stream_;
      int indent_;
    };
  }
}

#endif
