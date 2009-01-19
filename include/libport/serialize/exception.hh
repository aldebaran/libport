#ifndef LIBPORT_SERIALIZE_EXCEPTION_HH
# define LIBPORT_SERIALIZE_EXCEPTION_HH

# include <stdexcept>

namespace libport
{
  namespace serialize
  {
    class Exception: public std::runtime_error
    {
    public:
      Exception(const std::string& msg);
    };
  }
}

#endif
