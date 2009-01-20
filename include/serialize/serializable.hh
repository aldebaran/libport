#ifndef LIBPORT_SERIALIZE_SERIALIZABLE_HH
# define LIBPORT_SERIALIZE_SERIALIZABLE_HH

# include <serialize/export.hh>
# include <serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API Serializable
    {
    public:
      virtual void serialize(Serializer& s) = 0;
      virtual ~Serializable() {}
    };
  }
}

#endif
