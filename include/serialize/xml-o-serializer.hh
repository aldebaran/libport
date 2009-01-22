#ifndef LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH

# include <fstream>

# include <tinyxml/tinyxml.h>

# include <serialize/o-serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API XmlOSerializer: public OSerializer
    {
    public:

      XmlOSerializer(const std::string& stream);
      ~XmlOSerializer();

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);

    private:
      action_type serialize_collection(const std::string& name, int&);
      void serialize_collection_end(TiXmlNode* node);

      TiXmlDocument doc_;
      TiXmlNode* current_;
    };
  }
}

#endif
