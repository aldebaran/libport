/**
 ** \file libport/file-library.cc
 ** \brief Implements libport::file_library.
 */

#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "libport/config.h"

#ifdef LIBPORT_HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#include "libport/contract.hh"
#include "libport/file-library.hh"
#include "libport/foreach.hh"
#include "libport/unistd.h"

#ifndef MAXPATHLEN
# define MAXPATHLEN 4096
#endif

namespace libport
{

  void
  file_library::push_cwd ()
  {
    // Store the working directory
    char cwd[MAXPATHLEN + 1];

    if (0 == getcwd (cwd, MAXPATHLEN + 1))
      throw std::runtime_error ("working directory name too long");

    push_current_directory (cwd);
  }


  file_library::file_library ()
  {
    push_cwd ();
  }

  file_library::file_library (path p)
  {
    push_cwd ();
    // Then only process given path.
    search_path_.push_back (p);
  }

  void
  file_library::append_dir_list (std::string path_list)
  {
    std::string::size_type pos;

    while ((pos = path_list.find (':')) != std::string::npos)
      {
	append_dir (path_list.substr (0, pos));
	path_list.erase (0, pos + 1);
      }
    append_dir (path_list);
  }


  path
  file_library::ensure_absolute_path (path p) const
  {
    if (p.absolute_get ())
      return p;
    else
      return current_directory_get () / p;
  }

  void
  file_library::push_back (path p)
  {
    search_path_.push_back (ensure_absolute_path (p));
  }

  void
  file_library::push_front (path p)
  {
    search_path_.push_front (ensure_absolute_path (p));
  }

  void
  file_library::push_current_directory (path p)
  {
    // Ensure that path is absolute.
    if (!p.absolute_get ())
      p = this->current_directory_get () / p;

    current_directory_.push_front (p);
  }

  void
  file_library::pop_current_directory ()
  {
    precondition (!current_directory_.empty ());

    current_directory_.pop_front ();
  }

  path
  file_library::current_directory_get () const
  {
    precondition (!current_directory_.empty ());

    return *current_directory_.begin ();
  }


  path
  file_library::find_file (const path& file)
  {
    // Split file in two components, basename and basedir.
    path directory = file.dirname();

    if (directory.absolute_get ())
      {
	// If file is absolute, just check that it exists.
	if (!file.exists())
	  return path ();
	else
	  return directory;
      }
    else
      {
	// Does the file can be found in current directory?
	if (find_in_directory (current_directory_get (), file))
	  return (current_directory_get () / file).dirname();
	else
	  return find_in_search_path (directory, file.basename());
      }
  }

  bool
  file_library::find_in_directory (const path& dir,
				   const std::string& file) const
  {
    return (dir / file).exists();
  }

  path
  file_library::find_in_search_path (const path& relative_path,
				     const std::string& filename) const
  {
    path checked_dir;

    // Otherwise start scanning the search path.
    foreach (const path& p, search_path_)
      {
	if (p.absolute_get ())
	  checked_dir = p;
	else
	  checked_dir = current_directory_get () / p;

	checked_dir /= relative_path;

	if (find_in_directory (checked_dir, filename))
	  return checked_dir;
      }

    // File not found in search path.
    return path ();
  }

  std::ostream&
  file_library::dump (std::ostream& ostr) const
  {
    ostr << ".";

    for (path_list_type::const_iterator it = search_path_.begin ();
	 it != search_path_.end ();
	 ++it)
      ostr << ":" << *it;

    return ostr;
  }
}
