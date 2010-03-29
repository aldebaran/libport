/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
  /// Return the whole content of a file.
  LIBPORT_API std::string file_content(const std::string& name);
}

#endif // !LIBPORT_FILE_SYSTEM_HH
