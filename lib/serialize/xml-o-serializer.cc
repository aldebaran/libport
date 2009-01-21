#include <boost/bind.hpp>

#include <serialize/serializable.hh>
#include <serialize/xml-o-serializer.hh>

namespace libport
{
  namespace serialize
  {

    XmlOSerializer::XmlOSerializer(const std::string& path)
      : OSerializer(path)
      , stream_(path.c_str())
    {
      current_ = &doc_;
    }

    XmlOSerializer::~XmlOSerializer()
    {
      doc_.SaveFile(path_.c_str());
    }

    void XmlOSerializer::serialize(const std::string& name, Serializable& s)
    {
      TiXmlNode* node = current_;

      current_ = new TiXmlElement(name);
      node->LinkEndChild(current_);
      s.serialize(*this);
      current_ = node;
    }

    void XmlOSerializer::serialize(const std::string& name, std::string& s)
    {
      TiXmlElement* elt = new TiXmlElement(name);
      current_->LinkEndChild(elt);

      TiXmlText* txt = new TiXmlText(s);
      elt->LinkEndChild(txt);
    }

    void XmlOSerializer::serialize_collection_end(TiXmlNode* node)
    {
      current_ = node;
    }

    XmlOSerializer::action_type XmlOSerializer::serialize_collection(const std::string& name)
    {
      TiXmlNode* node = current_;
      current_ = new TiXmlElement(name);

      return boost::bind(&XmlOSerializer::serialize_collection_end, this, node);
    }

  }
}
