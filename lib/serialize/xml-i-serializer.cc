/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/format.hh>
#include <libport/lexical-cast.hh>

#include <serialize/xml-i-serializer.hh>
#include <serialize/serializable.hh>

namespace libport
{
  namespace serialize
  {
    XmlISerializer::XmlISerializer(const std::string& path)
      : ISerializer(path)
      , current_(0)
    {
      int res = root_.LoadFile(path_.c_str());
      if (!res)
        throw Exception(str(boost::format("Error while loading xml file: %s") % path));
      current_ = root_.FirstChild();
    }

    void XmlISerializer::serialize(const std::string& name, Serializable& s)
    {
      check_element(name);

      TiXmlNode* node = current_;
      current_ = node->FirstChild();
      s.serialize(*this);
      current_ = node->NextSiblingElement();
    }

    void XmlISerializer::serialize(const std::string& name, std::string& s)
    {
      check_element(name);
      TiXmlNode* node = current_->FirstChild();

      if (!node)
        throw Exception(str(boost::format("Missing text in `%s' element") % name));
      if (node->Type() != TiXmlNode::TEXT || node->NextSiblingElement())
        throw Exception(str(boost::format("Expected text only in `%s' element") % name));
      s = node->Value();
      current_ = current_->NextSiblingElement();
    }

    void XmlISerializer::serialize(const std::string& name, int& i)
    {
      check_element(name);
      TiXmlNode* node = current_->FirstChild();

      if (!node)
        throw Exception(str(boost::format("Missing integer in `%s' element") % name));
      if (node->Type() != TiXmlNode::TEXT || node->NextSiblingElement())
        throw Exception(str(boost::format("Expected integer only in `%s' element") % name));
      i = boost::lexical_cast<int>(node->Value());
      current_ = current_->NextSiblingElement();
    }

    void XmlISerializer::serialize_collection_end(TiXmlNode* n)
    {
      current_ = n;
    }

    XmlISerializer::action_type
    XmlISerializer::serialize_collection(const std::string& name, int& size)
    {
      check_element(name);
      TiXmlNode* node = current_->NextSiblingElement();
      current_ = current_->FirstChild();

      size = 0;
      TiXmlNode* iter = current_;
      for (; iter; iter = iter->NextSiblingElement())
        ++size;

      return boost::bind(&XmlISerializer::serialize_collection_end, this, node);
    }

    void XmlISerializer::check_element(const std::string& name)
    {
      if (!current_)
        throw Exception(str(boost::format("Missing `%s' element") % name));
      if (current_->Type() != TiXmlNode::ELEMENT)
        throw Exception(str(boost::format("Expected `%s' element") % name));
      std::string value(current_->Value());
      if (value != name)
        throw Exception(str(boost::format("Expected `%s' element, got `%s'")
                            % name % value));
    }

    bool XmlISerializer::optional_get(const std::string& name)
    {
      try
      {
        check_element(name);
        return true;
      }
      catch (Exception&)
      {
        return false;
      }
    }
  }
}
