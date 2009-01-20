#ifndef LIBPORT_SERIALIZE_SERIALIZABLE_HH
# define LIBPORT_SERIALIZE_SERIALIZABLE_HH

# include <libport/export.hh>
# include <libport/serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API Serializable
    {
    public:
      virtual void serialize(Serializer& s) = 0;
    };
  }
}

#endif
