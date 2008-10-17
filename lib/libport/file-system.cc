/**
 ** \file libport/file-system.cc
 ** \brief Implementation for file-system.hh
 */

#include <stdexcept>
#include <libport/file-system.hh>
#include <libport/sys/param.h>
#include <libport/sys/stat.h>
#include <libport/unistd.h>

namespace libport
{
  path
  get_current_directory()
  {
    // The various getcwd() documentations are not clear on whether the
    // NUL string terminator is accounted for in the length or not. To err
    // on the safe side, we assume it is not.
    char cwd[MAXPATHLEN+1];
    if (!getcwd(cwd, MAXPATHLEN))
      throw std::runtime_error("working directory name too long");
    return cwd;
  }

  bool
 is_dir(const std::string& f)
  {
    struct stat st;
    if (stat(f.c_str(), &st))
      return false;
# ifdef _MSC_VER
    return st.st_mode & _S_IFDIR;
# else
    return S_ISDIR(st.st_mode);
# endif
  }
}
