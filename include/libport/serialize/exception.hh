#ifndef LIBPORT_SERIALIZE_EXCEPTION_HH
# define LIBPORT_SERIALIZE_EXCEPTION_HH

# include <stdexcept>

# include <libport/export.hh>

namespace libport
{
  namespace serialize
  {
    class LIBPORT_API Exception: public std::runtime_error
    {
    public:
      Exception(const std::string& msg);
    };
  }
}

#endif
