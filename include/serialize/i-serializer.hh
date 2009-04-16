#ifndef LIBPORT_SERIALIZE_I_SERIALIZER_HH
# define LIBPORT_SERIALIZE_I_SERIALIZER_HH

# include <fstream>
# include <vector>

# include <serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API ISerializer
    {
    public:
      // FIXME: This should be an istream. However, tinyxml can only parse files ...
      // For now, support files only too.
      ISerializer(const std::string& path);

      virtual void
        unserialize(const std::string& name, std::string& value) = 0;
      virtual void
        unserialize(const std::string& name, int& value) = 0;

      template <template <typename, typename> class Cont, typename T, typename A>
        void unserialize(const std::string& name, Cont<T, A>& collection);

      virtual size_t start_collection(const std::string& name) = 0;
      virtual void end_collection() = 0;

    protected:
      std::ifstream stream_;
    };
  }
}

# include <serialize/i-serializer.hxx>

#endif
