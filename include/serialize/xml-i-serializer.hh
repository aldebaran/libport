/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_I_SERIALIZER_HH

# include <serialize/export.hh>

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
      virtual bool optional_get(const std::string& name);

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
