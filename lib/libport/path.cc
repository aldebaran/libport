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
 ** \file libport/path.cc
 ** \brief Implement libport::path.
 */

#include <cctype>
#include <iostream>
#include <libport/cstdio>
#include <libport/debug.hh>
#include <libport/detect-win32.h>
#include <libport/exception.hh>
#include <libport/fcntl.h>
#include <libport/format.hh>
#include <libport/path.hh>
#include <libport/sys/stat.h>
#include <libport/sys/types.h>
#include <libport/unistd.h>

GD_CATEGORY(Libport.Path);

// Implementation detail: if components() is empty and not absolute
// then the path is '.'.

namespace libport
{
  path::path(const std::string& p)
  {
    init(p);
  }

  path::path(const char* p)
  {
    init(p);
  }

  path::path(const value_type& p)
  {
    init(p.file_string());
  }

  void
  path::init(const std::string& p)
  {
    if (p.empty())
      throw invalid_path("Path can't be empty.");
#ifdef WIN32
    // We want "/" to mean "the root of the current volume" on windows.
    if (p[0] == '/')
      value_ = boost::filesystem::current_path().root_name() + p;
    else
#endif
    value_ = p;
    value_ = clean();
  }

  path&
  path::operator=(const path& rhs)
  {
    value_ = rhs.value_;
    return *this;
  }

  path&
  path::operator/=(const path& rhs)
  {
    if (rhs.absolute_get())
      throw invalid_path(
        "Rhs of concatenation is absolute: " + rhs.to_string());
#ifdef WIN32
    if (!rhs.volume_get().empty())
      throw invalid_path("concatenation of path with volume: " +
			 rhs.to_string());
#endif
    value_ /= rhs.value_;
    value_ = clean();
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
    if (!value_.is_complete() && components().empty())
      return WIN32_IF(volume_get().empty() ? "." : volume_get(), ".");

    return value_.file_string();
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

  std::string
  path::clean() const
  {
    std::string res;
    value_type::iterator i = value_.begin();
    value_type::iterator i_end = value_.end();
    if (i == i_end)
      return ".";

    bool first = true;
    if (absolute_get())
    {
      res = *i;
      ++i;
#ifdef WIN32
      // Add a "\" after the drive letter or network share.
      first = false;
#endif
    }

    for (;i != i_end; ++i)
    {
      // Remove the "." and empty components.
      if (!i->empty() && *i != ".")
      {
        if (!first)
          res += separator_;
        first = false;
        res += *i;
      }
    }
    return res.empty() ? "." : res;
  }

  std::string
  path::volume_get() const
  {
#ifndef WIN32
    return "";
#else
    std::string res = value_.root_name();
    if (res.find("//") == 0)
      res.replace(0, 2, "\\\\");
    return res;
#endif
  }

  path
  path::dirname() const
  {
    // dirname of / is /, dirname of . is .
    if (components().empty())
      return *this;

    std::string res = value_.parent_path().directory_string();
    return path(res.empty() ? "." : res);
  }

  bool path::exists() const
  {
    bool res;
    try
    {
      res = boost::filesystem::exists(value_);
    }
    catch (...)
    {
      res = false;
    }
    GD_FINFO_DUMP("exists: %-5s: %s", res ? "true" : "false", to_string());
    return res;
  }

  path
  path::cwd()
  {
    return path(boost::filesystem::current_path().string());
  }


  bool
  path::is_root() const
  {
    return to_string() == (volume_get() + separator_);
  }


  path
  path::parent() const
  {
    if (is_root())
      return path(volume_get().append(1, separator_));

    const std::string parent_path =
      boost::filesystem::path(to_string()).parent_path().string();

    if (parent_path.empty())
      return cwd();

    return parent_path;
  }

  std::time_t
  path::last_write_time() const
  {
    return boost::filesystem::last_write_time(
      boost::filesystem::path(to_string()));
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

  path
  path::temporary_file()
  {
#ifndef WIN32
    char tmp_name[] = "/tmp/tmp.XXXXXX";
    int fd = mkstemp(tmp_name);
    close (fd);
#else
    char tmp_dir[MAX_PATH];
    char tmp_name[MAX_PATH];

    if(GetTempPathA(sizeof(tmp_dir), tmp_dir) == 0
       || GetTempFileNameA(tmp_dir, "$$$", 0, tmp_name) == 0)
      throw Exception("cannot generate path for temporary file");
#endif
    return path(tmp_name);
  }

  void
  path::remove() const
  {
    if (!boost::filesystem::remove(value_))
      throw Exception(libport::format("cannot unlink file %s", *this));
  }

  void
  path::rename(const std::string& dst)
  {
    try
    {
      boost::filesystem::rename(value_, dst);
    }
    catch (Exception& e)
    {
      throw Exception(libport::format("cannot rename file %s: %s",
                                      *this, format_boost_fs_error(e.what())));
    }

    *this = dst;
  }

  path::path_type
  path::components() const
  {
    path_type path_;
    value_type::iterator
      i = value_.begin(),
      i_end = value_.end();
    if (i == i_end)
      return path_;

    // Skip volume information.
    if (absolute_get())
    {
      ++i;
#ifdef WIN32 // Skip a / after the volume information.
      ++i;
#endif
    }
    for (;i != i_end; ++i)
      if (!i->empty() && *i != ".")
        path_.push_back(*i);

    return path_;
  }

  std::string
  format_boost_fs_error(const char* what)
  {
    static const std::string ns = "boost::filesystem::";
    // Get rid of function namespace.
    std::string error = std::string(what).substr(ns.size());
    // Get rid of function name and ": ".
    error = error.substr(error.find(":") + 2);
    // Urbi errors begin with a lower case letter.
    error[0] = tolower(error[0]);
    return error;
  }
}
