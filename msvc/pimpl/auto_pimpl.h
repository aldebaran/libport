#include <memory>

#if defined _MSC_VER
# if defined BUILD
#  define API __declspec(dllexport)
# else
#  define API __declspec(dllimport)
# endif
#else
# define API __attribute__((visibility("default")))
#endif

// forward declaration of implementation type
class pimpl_impl;

// full declaration of facade type
class API pimpl
{
  public:
    pimpl () ;
    ~pimpl () ;

    size_t get_value () const;
  private:
    std::auto_ptr<pimpl_impl>  impl;
};
