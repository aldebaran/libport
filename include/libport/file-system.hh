/**
 ** \file libport/file-system.hh
 ** \brief Provides misc functions to handle filesystem
 */

#ifndef LIBPORT_FILE_SYSTEM_HH
# define LIBPORT_FILE_SYSTEM_HH

#include <string>

namespace libport
{
  bool is_dir (const std::string& f);
}

#endif // !LIBPORT_FILE_SYSTEM_HH
