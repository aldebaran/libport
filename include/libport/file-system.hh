/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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

namespace libport
{
  LIBPORT_API std::string get_current_directory();

  /// Whether exists and is a directory.
  LIBPORT_API bool is_dir(const std::string& f);

  /// Whether exists and is a regular file.
  LIBPORT_API bool is_reg(const std::string& f);

  /// The whole content of a file.
  LIBPORT_API std::string file_content(const std::string& name);
}

#endif // !LIBPORT_FILE_SYSTEM_HH
