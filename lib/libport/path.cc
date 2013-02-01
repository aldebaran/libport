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

namespace boostfs = boost::filesystem;

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
    init(p.string());
  }

  void
  path::init(const std::string& p)
  {
    if (p.empty())
      throw invalid_path("Path can't be empty.");
#ifdef WIN32
    // We want "/" to mean "the root of the current volume" on windows.
    if (p == "/")
      value_ = boostfs::current_path().root_name().string();
    else if (p[0] == '/')
      value_ = boostfs::current_path().root_name().string() + p.substr(1);
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

    return value_.string();
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

  bool
  path::is_dir() const
  {
    return boostfs::is_directory(to_string());
  }

  bool
  path::is_reg() const
  {
    return boostfs::is_regular_file(to_string());
  }

  path::value_type
  path::clean() const
  {

    value_type::iterator i = value_.begin();
    value_type::iterator i_end = value_.end();
    if (i == i_end)
      return value_type(".");
    value_type res = *i;
    ++i;

    for (;i != i_end; ++i)
    {
      // Remove the "." and empty components.
      if (!i->empty() && *i != ".")
      {
        res /= *i;
      }
    }
    return res;
  }

  std::string
  path::volume_get() const
  {
#ifndef WIN32
    return "";
#else
    std::string res = value_.root_name().string();
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

    std::string res = value_.parent_path().string();
    return path(res.empty() ? "." : res);
  }

  bool path::exists() const
  {
    bool res;
    try
    {
      res = boostfs::exists(value_);
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
    return path(boostfs::current_path().string());
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
      boostfs::path(to_string()).parent_path().string();

    if (parent_path.empty())
      return cwd();

    return parent_path;
  }

  std::time_t
  path::last_write_time() const
  {
    return boostfs::last_write_time(boostfs::path(to_string()));
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
    if (!boostfs::remove(value_))
      throw Exception(libport::format("cannot unlink file %s", *this));
  }

  void
  path::rename(const std::string& dst)
  {
    try
    {
      boostfs::rename(value_, dst);
    }
    catch (const boostfs::filesystem_error& e)
    {
      throw Exception(libport::format("cannot rename directory %s: %s",
                                      *this, format_error(e)));
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
        path_.push_back(i->string());

    return path_;
  }

  std::string
  format_error(const boostfs::filesystem_error& e)
  {
    std::string res = e.code().message();
    res[0] = tolower(res[0]);
    // Boost includes quotes in the error messages, which does not
    // match our style.
    if (!e.path1().empty())
      res += libport::format(": %s", e.path1());
    if (!e.path2().empty())
      res += libport::format(": %s", e.path2());
    return res;
  }
}
