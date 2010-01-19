/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_XML_O_SERIALIZER_HH

# include <fstream>

# include <tinyxml/tinyxml.h>

# include <serialize/export.hh>
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
      void serialize(const std::string& name, int& i);

    private:
      action_type serialize_collection(const std::string& name, int&);
      void serialize_collection_end(TiXmlNode* node);

      TiXmlDocument doc_;
      TiXmlNode* current_;
    };
  }
}

#endif
