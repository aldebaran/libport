#ifndef LIBPORT_SERIALIZE_SERIALIZER_HH
# define LIBPORT_SERIALIZE_SERIALIZER_HH

# include <vector>

# include <boost/function.hpp>
# include <boost/optional.hpp>

# include <libport/serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class Serializer
    {
    public:

      Serializer(bool input);

      template <typename T>
      void serialize(const std::string& name, std::vector<T>& collection);
      template <typename T>
      void serialize(const std::string& name, boost::optional<T>& option);

      virtual void serialize(const std::string& name, Serializable& s)  = 0;
      virtual void serialize(const std::string& name, std::string& s)   = 0;

    protected:
      typedef boost::function0<void> action_type;
      virtual action_type serialize_collection(const std::string& name) = 0;
      bool input_;
    };
  }
}

#include <libport/serialize/serializer.hxx>

#endif
