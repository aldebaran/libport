/**
 ** \file libport/file-system.hh
 ** \brief Provides misc functions to handle filesystem
 */

#ifndef LIBPORT_FILE_SYSTEM_HH
# define LIBPORT_FILE_SYSTEM_HH

#include <string>

#include <libport/export.hh>
#include <libport/path.hh>

namespace libport
{
  LIBPORT_API path get_current_directory();
  LIBPORT_API bool is_dir(const std::string& f);
}

#endif // !LIBPORT_FILE_SYSTEM_HH
