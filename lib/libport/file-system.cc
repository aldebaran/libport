/**
 ** \file libport/file-system.cc
 ** \brief Implementation for file-system.hh
 */

#include <stdexcept>
#include <fstream>
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

  std::string
  file_content(const std::string& n)
  {
    std::ifstream s(n.c_str());
    if (!s.good())
      throw std::runtime_error("Failed to open " + n +" for reading");
    std::string res;
    char buf[BUFSIZ + 1];
    while (!s.eof())
    {
      s.read(buf, sizeof buf - 1);
      res += std::string(buf, s.gcount());
    }
    return res;
  }
}
