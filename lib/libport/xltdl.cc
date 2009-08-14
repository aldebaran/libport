#include <iostream>
#include <libport/debug.hh>
#include <libport/format.hh>
#include <libport/path.hxx>
#include <libport/program-name.hh>
#include <libport/sysexits.hh>
#include <libport/xltdl.hh>

GD_ADD_CATEGORY(XLTDL);

namespace libport
{
  namespace xlt
  {
    namespace details
    {

      /// \param verbosity  the current debug level.
      /// \param level      the level of this message.
      static
      int
      ltdebug(unsigned /* verbosity */,
              unsigned level, const char* format, va_list args)
      {
        GD_CATEGORY(XLTDL);
        int errors = 0;
        char* msg = 0;
        errors += vasprintf(&msg, format, args) < 0;
        if (!errors && msg)
        {
          GD_FINFO_DEBUG("%s%s", (std::string(level * 2, ' '))(msg));
          free(msg);
        }
        return errors;
      }

      void init()
      {
        static bool tail = false;
        if (!tail++)
        {
          lt_dlinit();
          lt_dladd_log_function((lt_dllog_function*) &ltdebug, 0);
        }
      }
    }
  }


  /*------------------------.
  | xlt_advise::exception.  |
  `------------------------*/

  xlt_advise::exception::exception(const std::string& msg)
    : std::runtime_error(msg)
  {}


  /*-------------.
  | xlt_advise.  |
  `-------------*/

  xlt_advise::xlt_advise()
    throw(xlt_advise::exception)
    : path_()
  {
    xlt::details::init();
    if (lt_dladvise_init(&advise_))
      fail("failed to initialize dladvise");
  }

  // FIXME: Bad: dtors must not throw.
  xlt_advise::~xlt_advise()
    throw(xlt_advise::exception)
  {
    // FIXME: lt_dlexit when we refcount.
    if (lt_dladvise_destroy(&advise_))
      fail("failed to destroy dladvise");
  }

  xlt_advise::xlt_advise&
  xlt_advise::global(bool global)
    throw(xlt_advise::exception)
  {
    if (global ? lt_dladvise_global(&advise_) : lt_dladvise_local(&advise_))
      fail(libport::format("failed to set dladvise to %s",
                           global ? "global" : "local"));
    return *this;
  }

  xlt_advise::xlt_advise&
  xlt_advise::ext()
    throw(xlt_advise::exception)
  {
    if (lt_dladvise_ext(&advise_))
      fail("failed to set dladvise to ext");
    return *this;
  }

  const file_library&
  xlt_advise::path() const throw()
  {
    return path_;
  }

  file_library&
  xlt_advise::path() throw()
  {
    return path_;
  }

  lt_dlhandle
  xlt_advise::dlopen_(const std::string& s) const
    throw(xlt_advise::exception)
  {
    GD_CATEGORY(XLTDL);
    lt_dlhandle res = lt_dlopenadvise(s.c_str(), advise_);
    GD_FINFO("loading %s: %s", (s)((res ? "pass" : "fail")));
    return res;
  }

  xlt_handle
  xlt_advise::open(const std::string& s)
    throw(xlt_advise::exception)
  {
    GD_CATEGORY(XLTDL);
    GD_FINFO("loading %s", (s));
    lt_dlhandle res = 0;
    // We cannot simply use search_file in file_library, because we
    // don't know the extension of the file we are looking for (*.la,
    // *.so, *.dyld etc.).  That's an implementation detail that ltdl
    // saves us from.
    if (path_.search_path_get().empty() || libport::path(s).absolute_get())
      res = dlopen_(s);
    else
      foreach (const libport::path& p, path_.search_path_get())
        if ((res = dlopen_(p / s)))
          break;

    if (!res)
      fail(libport::format("failed to open %s", s));

    return res;
  }


  /// Throw an exception, or exit with exit_failure_ if nonnull.
  void
  xlt_advise::fail(std::string msg)
    throw(xlt_advise::exception)
  {
    msg += ": ";
    msg += lt_dlerror();
    throw exception(msg);
  }



  /*-------------.
  | xlt_handle.  |
  `-------------*/

  xlt_handle::xlt_handle(lt_dlhandle h)
    : handle(h)
  {}

  xlt_handle::~xlt_handle()
  {
    // FIXME: We can't close -- yet.  We need to keep track of the
    // number of trackers.  Otherwise a simple "handle h =
    // advise.open" will close the handle when cleaning the temporary
    // made by "advise.open".

    // close();
  }

  void
  xlt_handle::close()
    throw(xlt_handle::exception)
  {
    if (handle)
    {
      int errors = lt_dlclose(handle);
      handle = 0;
      if (errors)
        fail("cannot close");
    }
  }

  void
  xlt_handle::detach()
  {
    assert(handle);
    handle = 0;
  }

  void
  xlt_handle::attach(lt_dlhandle h)
  {
    assert(!handle);
    handle = h;
  }

  /// Throw an exception, or exit with exit_failure_ if nonnull.
  void
  xlt_handle::fail(const std::string& msg)
    throw(xlt_advise::exception)
  {
    xlt_advise::fail(msg);
  }


  /*-------------.
  | Standalone.  |
  `-------------*/

  xlt_handle
  xlt_openext(const std::string& s, bool global, int exit_failure)
    throw (xlt_advise::exception)
  {
    try
    {
      return xlt_advise().global(global).ext().open(s);
    }
    catch (std::exception& s)
    {
      std::cerr << libport::program_name()
                << ": "
                << s.what()
                << std::endl
                << libport::exit(exit_failure);
    }
  }
}
