/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/path.cc
 ** \brief Implement libport::path.
 */

#include <cctype>
#include <iostream>
#include <libport/contract.hh>
#include <libport/cstdio>
#include <libport/debug.hh>
#include <libport/detect-win32.h>
#include <libport/exception.hh>
#include <libport/fcntl.h>
#include <libport/foreach.hh>
#include <libport/format.hh>
#include <libport/path.hh>
#include <libport/sys/stat.h>
#include <libport/sys/types.h>
#include <libport/unistd.h>

GD_ADD_CATEGORY(path);


// Implementation detail: if path_ is empty and not absolute
// then the path is '.'.

namespace libport
{
  path::path(const std::string& p)
    : boost_path_(p)
  {
    init();
  }

  path::path(const char* p)
    : boost_path_(p)
  {
    init();
  }

  void
  path::init()
  {
    if (boost_path_.empty())
      throw invalid_path("Path can't be empty.");

#if defined WIN32
    volume_ = boost_path_.root_name();
    // root_name() returns "//" for shares.
    if (volume_.find("//") == 0)
      volume_.replace(0,2,"\\\\");
#endif

    // Cut directories on / and \.
    foreach (const std::string& component, boost_path_)
      if (component != "/")
        append_dir(component);
  }

  path&
  path::operator=(const path& rhs)
  {
    boost_path_ = rhs.boost_path_;
    path_ = rhs.path_;
#if defined WIN32
    volume_ = rhs.volume_;
#endif
    return *this;
  }

  path&
  path::operator/=(const path& rhs)
  {
    if (rhs.absolute_get())
      throw invalid_path(
        "Rhs of concatenation is absolute: " + rhs.to_string());
#ifdef WIN32
    if (!rhs.volume_.empty())
      throw invalid_path("concatenation of path with volume: " +
			 rhs.to_string());
#endif
    boost_path_ /= rhs.boost_path_;
    foreach (const std::string& dir, rhs.path_)
      append_dir(dir);

    return *this;
  }

  path
  path::operator/(const path& rhs) const
  {
    path res = *this;
    return res /= rhs;
  }

  std::string
  path::to_string() const
  {
    if (!boost_path_.is_complete() && path_.empty())
      return WIN32_IF(volume_.empty() ? "." : volume_, ".");

    return boost_path_.file_string();
  }

  bool
  path::operator==(const path& rhs) const
  {
    return (volume_get() == rhs.volume_get()
            && absolute_get() == rhs.absolute_get()
            && components() == rhs.components());
  }

  std::ostream&
  path::dump(std::ostream& o) const
  {
    return o << operator std::string();
  }

  void
  path::append_dir(const std::string& dir)
  {
    precondition(dir.find(separator_) == std::string::npos);

    if (!dir.empty() && dir != ".")
    {
      // The following does not work properly with symlinks.  One
      // cannot expect that removing "foo/../" inside a valid path
      // results in the same path.  We should check whether "foo" is
      // *not* a symlink before playing this kind of trick.
#if 0
      if (dir == ".."
          && !path_.empty() && *path_.rbegin() != "..")
        path_.pop_back();
      else
#endif
	path_.push_back(dir);
    }
  }

  path
  path::dirname() const
  {
    // dirname of / is /, dirname of . is .
    if (path_.empty())
      return *this;

    std::string res = boost_path_.branch_path().directory_string();
    return path(res.empty() ? "." : res);
  }

  bool path::exists() const
  {
    bool res = fs::exists(boost_path_);
    GD_CATEGORY(path);
    GD_FINFO_DUMP("exists: %-5s: %s", res ? "true" : "false", to_string());
    return res;
  }

  bool path::create() const
  {
    int fd = open(to_string().c_str(),
                  O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
      return false;
    close(fd);
    return true;
  }

#ifndef WIN32
  path
  path::temporary_file()
  {
    char tmpfile[] = "/tmp/tmp.XXXXXX";
    int fd = mkstemp(tmpfile);
    close (fd);
    return path(tmpfile);
  }
#else
  // Implementation pieces for windows

  // // Get the path to the temporary folder
  // char tmp_path[MAX_PATH];
  // memset (tmp_path, 0, MAX_PATH);
  // GetTempPath(MAX_PATH, tmp_path);
  // char tmpfile[MAX_PATH];
  // memset (tmpfile, 0, MAX_PATH);
  // GetTempFileName (tmp_path, "", 0, tmpfile);
#endif

  void
  path::remove() const
  {
    if (!fs::remove(boost_path_))
      throw Exception(libport::format("unable to unlink file: %s", *this));
  }

  void
  path::rename(const std::string& dst)
  {
    try {
      fs::rename(boost_path_, dst);
    }
    catch (Exception& e) {
      throw Exception(libport::format(
        "unable to rename file %s (%s)", *this, e.what()));
    }

    *this = dst;
  }
}
