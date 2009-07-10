#include <boost/static_assert.hpp>
#include <libport/format.hh>

namespace libport
{

  /*---------------.
  | xlt::details.  |
  `---------------*/

  namespace xlt
  {
    namespace details
    {
      LIBPORT_API void init();
    }
  }

  /*-------------.
  | xlt_handle.  |
  `-------------*/

  template <typename T>
  inline
  T
  xlt_handle::sym(const std::string& s)
    throw (xlt_handle::exception)
  {
    assert(handle);
    void* res = lt_dlsym(handle, s.c_str());
    if (!res)
      fail(libport::format("failed to dlsym %s", s));
    // GCC 3.4.6 on x86_64 at least requires that we go through a
    // scalar type. It doesn't support casting a void* into a function
    // pointer directly. Later GCC versions do not have this
    // problem. We use a BOOST_STATIC_ASSERT to ensure that "void*"
    // and "unsigned long" have the same size.
    BOOST_STATIC_ASSERT(sizeof(unsigned long) == sizeof(void*));
    return reinterpret_cast<T>(reinterpret_cast<unsigned long>(res));
  }
}
