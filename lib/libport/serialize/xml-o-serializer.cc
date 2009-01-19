#include <boost/bind.hpp>

#include <libport/serialize/serializable.hh>
#include <libport/serialize/xml-o-serializer.hh>

namespace libport
{
  namespace serialize
  {

    XmlOSerializer::XmlOSerializer(const std::string& path)
      : OSerializer(path)
      , stream_(path.c_str())
      , indent_(0)
    {}

    void XmlOSerializer::serialize(const std::string& name, Serializable& s)
    {
      indent();
      stream_ << "<" << name << ">" << std::endl;
      ++indent_;
      s.serialize(*this);
      --indent_;
      indent();
      stream_ << "</" << name << ">" << std::endl;
    }

    void XmlOSerializer::serialize(const std::string& name, std::string& s)
    {
      indent();
      stream_ << "<" << name << ">";
      stream_ << s;
      stream_ << "</" << name << ">" << std::endl;
    }

    void XmlOSerializer::serialize_collection_end(const std::string& name)
    {
      --indent_;
      indent();
      stream_ << "</" << name << ">" << std::endl;
    }

    XmlOSerializer::action_type XmlOSerializer::serialize_collection(const std::string& name)
    {
      indent();
      stream_ << "<" << name << ">" << std::endl;
      ++indent_;
      return boost::bind(&XmlOSerializer::serialize_collection_end, this, name);
    }

    void XmlOSerializer::indent()
    {
      for (int i = 0; i < indent_; ++i)
        stream_ << "  ";
    }

  }
}
