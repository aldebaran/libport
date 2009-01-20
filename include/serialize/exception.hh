#ifndef LIBPORT_SERIALIZE_EXCEPTION_HH
# define LIBPORT_SERIALIZE_EXCEPTION_HH

# include <stdexcept>

# include <serialize/export.hh>

namespace libport
{
  namespace serialize
  {
    class SERIALIZE_API Exception: public std::runtime_error
    {
    public:
      Exception(const std::string& msg);
    };
  }
}

#endif
