#include <serialize/exception.hh>

namespace libport
{
  namespace serialize
  {
    Exception::Exception(const std::string& msg)
      : runtime_error(msg)
    {}
  }
}
