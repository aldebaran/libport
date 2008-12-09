#ifndef LIBPORT_PID_FILE_HH
# define LIBPORT_PID_FILE_HH

# include <string>

# include <boost/optional.hpp>

# include <libport/path.hh>

namespace libport
{
  /// A class to create a pid file for a daemon
  /** The PID file isn't created at construction, but when create is
   ** called. This enable to create the file only when the daemon is
   ** functional, for instance.
   **/
  class LIBPORT_API PidFile
  {
  public:
    /// Build a PidFile
    /** \param path Path of the future PID file
     */
    PidFile(boost::optional<std::string> path);
    /// Dispose of the PID file
    ~PidFile();
    /// Create the PID file
    void create();

  private:
    libport::path path_;
  };
}

#endif
