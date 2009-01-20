#ifndef LIBPORT_SERIALIZE_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_O_SERIALIZER_HH

# include <serialize/serializer.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API OSerializer: public Serializer
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
