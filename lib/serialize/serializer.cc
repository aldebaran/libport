#include <serialize/serializer.hh>

namespace libport
{
  namespace serialize
  {
    Serializer::Serializer(bool input)
      : input_(input)
    {}

    Serializer::~Serializer()
    {}

    bool
    Serializer::optional_get(const std::string&)
    {
      return false;
    }

    void
    Serializer::optional_put(const std::string&, bool)
    {}
  }
}
