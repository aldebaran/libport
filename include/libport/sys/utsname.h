/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_UTSNAME_H
# define LIBPORT_SYS_UTSNAME_H

# include <libport/config.h>

# ifdef LIBPORT_HAVE_SYS_UTSNAME_H
#  include <sys/utsname.h>
# endif

# include <string>

namespace libport
{

  class utsname
  {
  public:
    utsname();
    /// Name of the operating system implementation.
    /// Corresponds to sysname.
    std::string system() const;

    /// Network name of this machine.
    /// nodename.
    std::string node() const;

    /// Release level of the operating system.
    /// release.
    std::string release() const;

    /// Decompose release into major, minor, and patchlevel.
    /// The GNU Libc defines macros named major and minor...
    unsigned release_major() const;
    unsigned release_minor() const;
    unsigned release_patchlevel() const;

    /// Version level of the operating system.
    /// version.
    std::string version() const;

    /// Machine hardware platform.
    std::string machine() const;

  private:
    struct ::utsname utsname_;
    unsigned major_;
    unsigned minor_;
    unsigned patchlevel_;
  };

}

#endif
