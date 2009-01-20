#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_I_SERIALIZER_HH

# include <libport/serialize/serializer.hh>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API ISerializer: public Serializer
    {
    public:
      // FIXME: This should be an istream. However, tinyxml can only parse files ...
      // For now, support files only too.
      ISerializer(const std::string& path);

    protected:
      std::string path_;
    };
  }
}

#endif
