#ifndef LIBPORT_SERIALIZE_SERIALIZER_HH
# define LIBPORT_SERIALIZE_SERIALIZER_HH

# include <vector>

# include <boost/function.hpp>
# include <boost/optional.hpp>

# include <libport/export.hh>
# include <libport/serialize/fwd.hh>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API Serializer
    {
    public:

      Serializer(bool input);

      template <template <typename, typename> class C, typename T, typename A>
      void serialize(const std::string& name, C<T, A>& collection);
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
