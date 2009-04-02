#include <libport/assert.hh>
#include <libport/foreach.hh>
#include <libport/backtrace.hh>

namespace libport
{

  void
  abort()
  {
    foreach(const std::string& str, libport::backtrace())
      std::cerr << str << std::endl;
# ifdef _MSC_VER
    if (getenv("_DEBUG_VCXX"))
      _CrtDbgBreak();
    else
# endif
      std::abort();
  }

}
