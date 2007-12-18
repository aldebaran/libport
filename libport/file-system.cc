/** 
 ** \file libport/file-system.cc
 ** \brief Implementation for file-system.hh
 */

#include "libport/sys/stat.h"
#include "libport/file-system.hh"

namespace libport
{
  bool is_dir (const std::string& f)
  {
    struct stat st;
    stat(f.c_str(), &st);
# ifdef _MSC_VER
    return st.st_mode & _S_IFDIR;
# else
    return S_ISDIR(st.st_mode);
# endif
  }
}
