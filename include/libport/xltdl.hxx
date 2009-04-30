#include <boost/static_assert.hpp>
#include <libport/path.hxx>

namespace libport
{

# define LIBPORT_XLTDL_MESSAGE(Message)         \
  std::cerr << program_name()                   \
            << ": " << Message                  \
            << std::endl

# define LIBPORT_XLTDL_ERROR(Status, Message)                   \
  LIBPORT_XLTDL_MESSAGE(Message << ": " << lt_dlerror())        \
  << libport::exit(Status)

  /*---------------.
  | xlt_dladvise.  |
  `---------------*/

  inline
  xlt_dladvise::xlt_dladvise()
    : exit_failure_(1)
    , path_()
    , verbose_(false)
  {
    if (lt_dladvise_init(&advise_))
      LIBPORT_XLTDL_ERROR(1, "failed to initialize dladvise");
  }

  inline
  xlt_dladvise::~xlt_dladvise()
  {
    if (lt_dladvise_destroy(&advise_))
      LIBPORT_XLTDL_ERROR(1, "failed to destroy dladvise");
  }

  inline
  xlt_dladvise::xlt_dladvise&
  xlt_dladvise::global(bool global)
  {
    if (global ? lt_dladvise_global(&advise_) : lt_dladvise_local(&advise_))
      LIBPORT_XLTDL_ERROR(1, "failed to set dladvise to "
                          << (global ? "global" : "local"));
    return *this;
  }

  inline
  xlt_dladvise::xlt_dladvise&
  xlt_dladvise::exit_failure(int s)
  {
    exit_failure_ = s;
    return *this;
  }

  inline
  xlt_dladvise::xlt_dladvise&
  xlt_dladvise::ext()
  {
    if (lt_dladvise_ext(&advise_))
      LIBPORT_XLTDL_ERROR(1, "failed to set dladvise to ext");
    return *this;
  }

  inline
  const file_library&
  xlt_dladvise::path() const
  {
    return path_;
  }

  inline
  file_library&
  xlt_dladvise::path()
  {
    return path_;
  }

  inline
  xlt_dladvise::xlt_dladvise&
  xlt_dladvise::verbose(bool v)
  {
    verbose_ = v;
    return *this;
  }

  inline
  lt_dlhandle
  xlt_dladvise::dlopen_(const std::string& s) const
  {
    return lt_dlopenadvise(s.c_str(), advise_);
  }

  inline
  lt_dlhandle
  xlt_dladvise::xdlopen(const std::string& s) const
  {
    if (verbose_)
      LIBPORT_XLTDL_MESSAGE("loading " << s);
    // We cannot simply use search_file in file_library, because we
    // don't know the extension of the file we are looking for (*.la,
    // *.so, *.dyld etc.).  That's an implementation detail that ltdl
    // saves us from.
    lt_dlhandle res = 0;
    if (libport::path(s).absolute_get())
      res = dlopen_(s);
    else
      foreach (const libport::path& p, path_.search_path_get())
        if ((res = dlopen_(p / s)))
        {
          if (verbose_)
            LIBPORT_XLTDL_MESSAGE("found " << s << " in " << p);
          break;
        }

    if (!res)
      LIBPORT_XLTDL_ERROR(exit_failure_, "failed to load " << s);
    return res;
  }


  /*-------------.
  | Standalone.  |
  `-------------*/

  lt_dlhandle
  xlt_dlopenext(const std::string& s, bool global, int exit_failure,
                bool verbose)
  {
    return (xlt_dladvise()
            .global(global)
            .ext()
            .exit_failure(exit_failure)
            .verbose(verbose)
            .xdlopen(s));
  }


  template <typename T>
  inline
  T
  xlt_dlsym(lt_dlhandle h, const std::string& s)
  {
    void* res = lt_dlsym(h, s.c_str());
    if (!res)
      LIBPORT_XLTDL_ERROR(1, "failed to dlsym " << s);
    // GCC 3.4.6 on x86_64 at least requires that we go through a
    // scalar type. It doesn't support casting a void* into a function
    // pointer directly. Later GCC versions do not have this
    // problem. We use a BOOST_STATIC_ASSERT to ensure that "void*"
    // and "unsigned long" have the same size.
    BOOST_STATIC_ASSERT(sizeof(unsigned long) == sizeof(void*));
    return reinterpret_cast<T>(reinterpret_cast<unsigned long>(res));
  }

#undef LIBPORT_XLTDL_ERROR
#undef LIBPORT_XLTDL_MESSAGE

}
