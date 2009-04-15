#ifndef LIBPORT_SERIALIZE_O_SERIALIZER_HH
# define LIBPORT_SERIALIZE_O_SERIALIZER_HH

# include <fstream>

# include <serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API OSerializer
    {
    public:
      // FIXME: This should be an ostream. See ISerialier::ISerialier comment.
      OSerializer(const std::string& path);
      virtual void serialize(const std::string& name, const Serializable& value) = 0;
      virtual void serialize(const std::string& name, const std::string& value) = 0;
      virtual void serialize(const std::string& name, int value) = 0;

      template <template <typename, typename> class Cont, typename T, typename A>
        void serialize(const std::string& name, const Cont<T, A>& collection);
      virtual void start_collection(const std::string& name, size_t size) = 0;
      virtual void end_collection() = 0;

    protected:
      std::ofstream stream_;
    };
  }
}

# include <serialize/o-serializer.hxx>

#endif
