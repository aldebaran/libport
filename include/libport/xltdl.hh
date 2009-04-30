#ifndef LIBPORT_XLTDL_HXX
# define LIBPORT_XLTDL_HXX

# include <libltdl/ltdl.h>

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
    xlt_dladvise& verbose(bool verbose);

    lt_dlhandle xdlopen(const std::string& s) const;

  private:
    lt_dladvise advise_;
    int exit_failure_;
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

#endif // ! LIBPORT_XLTDL_HXX
