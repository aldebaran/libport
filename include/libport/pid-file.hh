/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PID_FILE_HH
# define LIBPORT_PID_FILE_HH

# include <string>

# include <libport/export.hh>
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
    /// Build a PidFile with specified path
    /** \param path Path of the future PID file.
     */
    PidFile(const std::string& path = default_filename());
    /// Build a PidFile, parsing option line for the file name.
    /* Extracts --pid-file option to determine the PID file name. The
       option is removed from the given command line.

       @param argc The number of arguments
       @param argc The arguments
       @param def  The default if no option is found
       @param opt  The option name to look for */
    PidFile(int& argc,
            const char* argv[],
            const std::string& path = default_filename(),
            const std::string& opt = "--pid-file");
    /// Dispose of the PID file
    ~PidFile();
    /// Create the PID file
    void create();
    /// Get a reasonable default PID filename: /var/run/$argv[0].pid
    static std::string default_filename();

  private:
    libport::path path_;
  };
}

#endif
