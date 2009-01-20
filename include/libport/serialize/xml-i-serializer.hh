#ifndef LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH

# include <libport/serialize/i-serializer.hh>

# include <tinyxml/tinyxml.h>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API XmlISerializer: public ISerializer
    {
    public:

      XmlISerializer(const std::string& path);

      void serialize(const std::string& name, Serializable& s);
      void serialize(const std::string& name, std::string& s);

      void check_element(const std::string& name);

    private:
      action_type serialize_collection(const std::string& name);
      void serialize_collection_end(TiXmlNode* n);

      TiXmlDocument root_;
      TiXmlNode* current_;
    };
  }
}

#endif
