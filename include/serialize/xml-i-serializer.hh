#ifndef LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH

# include <serialize/i-serializer.hh>

# include <tinyxml/tinyxml.h>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API XmlISerializer: public ISerializer
    {
    public:

      XmlISerializer(const std::string& path);

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);
      void serialize(const std::string& name, int& i);

      void check_element(const std::string& name);

    private:
      action_type serialize_collection(const std::string& name, int& size);
      void serialize_collection_end(TiXmlNode* n);

      TiXmlDocument root_;
      TiXmlNode* current_;
    };
  }
}

#endif
