#ifndef LIBPORT_SERIALIZE_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_O_SERIALIZER_HH

# include <libport/serialize/serializer.hh>

namespace libport
{
  namespace serialize
  {
    class OSerializer: public Serializer
    {
    public:
      // FIXME: This should be an ostream. See ISerialier::ISerialier comment.
      OSerializer(const std::string& path);

    protected:
      std::string path_;
    };
  }
}

#endif
