#ifndef LIBPORT_SERIALIZE_SERIALIZABLE_HH
# define LIBPORT_SERIALIZE_SERIALIZABLE_HH

# include <libport/serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class Serializable
    {
    public:
      virtual void serialize(Serializer& s) = 0;
    };
  }
}

#endif
