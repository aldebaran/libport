#ifndef LIBPORT_XLTDL_HH
# define LIBPORT_XLTDL_HH

# include <ltdl.h>
# include <libport/file-library.hh>

namespace libport
{

  class xlt_dladvise
  {
  public:
    xlt_dladvise();
    ~xlt_dladvise();

    xlt_dladvise& global(bool global);
    xlt_dladvise& ext();

    /// Exit status on dlopen failures.
    xlt_dladvise& exit_failure(int s);
    const file_library& path() const;
    file_library& path();
    xlt_dladvise& verbose(bool verbose);

    /// Looks in the path_ if defined.
    /// Always passes an absolute path to ltdl, so that its
    /// own search path is never used.
    lt_dlhandle xdlopen(const std::string& s) const;

  private:
    /// Does not use the search path.  Can return 0.
    lt_dlhandle dlopen_(const std::string& s) const;

    lt_dladvise advise_;
    int exit_failure_;
    file_library path_;
    bool verbose_;
  };

  /// Wrapper around lt_dlopenext that exits on failures.
  lt_dlhandle
  xlt_dlopenext(const std::string& s, bool global, int exit_failure = 1,
                bool verbose = false);

  /// Wrapper around lt_dlsym that exits on failures.
  template <typename T>
  static
  T
  xlt_dlsym(lt_dlhandle h, const std::string& s);

}

# include <libport/xltdl.hxx>

#endif // ! LIBPORT_XLTDL_HH
