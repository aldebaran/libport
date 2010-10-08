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

# include <iosfwd>

# include <libport/config.h>
# include <libport/export.hh>

# if defined LIBPORT_HAVE_SYS_UTSNAME_H
#  include <sys/utsname.h>
# else

#  define _SYS_NAMELEN 40
struct utsname
{
  const char *sysname;
  const char *nodename;
  const char *release;
  const char *version;
  const char *machine;
};

LIBPORT_API
int uname(utsname* u);

# endif

# include <string>

namespace libport
{

  class LIBPORT_API utsname
  {
  public:
    utsname();
    /// Name of the operating system implementation.
    /// Corresponds to sysname.
    /// "Darwin" on OS X.
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

    /// Mostly for debugging.
    std::ostream& dump(std::ostream& o) const;

  private:
    struct ::utsname utsname_;
    unsigned major_;
    unsigned minor_;
    unsigned patchlevel_;
  };

  /// Mostly for debugging.
  std::ostream& operator<<(std::ostream& o, const utsname& u);

}

# include <libport/sys/utsname.hxx>

#endif
