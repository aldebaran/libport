#ifndef LIBPORT_XLTDL_HH
# define LIBPORT_XLTDL_HH

# include <stdexcept>

# include <libport/fwd.hh>
# include <libport/compiler.hh>
# include <libport/export.hh>
# include <libport/file-library.hh>

# include <ltdl.h>

namespace libport
{

  class LIBPORT_API xlt_advise
  {
  public:
    /// Exceptions thrown on errors.
    struct exception : std::runtime_error
    {
      exception(const std::string& msg);
    };


    xlt_advise() throw (exception);
    ~xlt_advise() throw (exception);

    xlt_advise& global(bool global) throw (exception);
    xlt_advise& ext() throw (exception);

    const file_library& path() const throw ();
    file_library& path() throw ();

    xlt_handle open(const std::string& s) throw(exception);

    /// Throw an exception, or exit with exit_status_ if nonnull.
    static void fail(std::string msg) throw (exception) ATTRIBUTE_NORETURN;

  private:
    /// Does not use the search path.  Can return 0.
    lt_dlhandle dlopen_(const std::string& s) const throw (exception);

    lt_dladvise advise_;
    file_library path_;
  };


  class LIBPORT_API xlt_handle
  {
  public:
    typedef xlt_advise::exception exception;

    xlt_handle(lt_dlhandle h = 0);
    ~xlt_handle();

    /// Close the handle.
    void close() throw (exception);

    /// Detach so that destruction does not close.
    void detach();

    /// Detach so that destruction does not close.
    void attach(lt_dlhandle h);

    /// Wrapper around lt_dlsym that exits on failures.
    template <typename T>
    T sym(const std::string& s) throw (exception);

    /// Bounce to xlt_advise::fail.
    static void fail(const std::string& msg) throw (exception)
      ATTRIBUTE_NORETURN;

    /// The handle.
    /// Exposed, as currently we don't cover the whole lt_ interface.
    lt_dlhandle handle;
  };

  /// Wrapper around lt_dlopenext that exits on failures.
  LIBPORT_API
  xlt_handle
  xlt_openext(const std::string& s, bool global, int exit_failure = 1)
    throw (xlt_advise::exception);

}

# include <libport/xltdl.hxx>

#endif // ! LIBPORT_XLTDL_HH
