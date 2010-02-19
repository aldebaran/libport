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
#include <libport/tokenizer.hh>
#include <libport/unistd.h>

GD_ADD_CATEGORY(path);


// Implementation detail: if components() is empty and not absolute
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

  path::path(const fs::path& p)
    : boost_path_(p)
  {
    init();
  }

  void
  path::init()
  {
    if (boost_path_.file_string().empty())
      throw invalid_path("Path can't be empty.");
    boost_path_ = clean();
  }

  path&
  path::operator=(const path& rhs)
  {
    boost_path_ = rhs.boost_path_;
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
    boost_path_ /= rhs.boost_path_;
    boost_path_ = clean();
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
    if (!boost_path_.is_complete() && components().empty())
      return WIN32_IF(volume_get().empty() ? "." : volume_get(), ".");

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

  std::string
  path::clean()
  {
    std::string res;
    fs::path::iterator it = boost_path_.begin();
    fs::path::iterator it_end = boost_path_.end();
    if (it == it_end)
      return ".";

    bool tail = false;
    bool has_path = false;
    if (absolute_get())
    {
      res = *it;
      ++it;
#ifdef WIN32
      // Add an \ after the drive letter / share.
      tail = true;
#endif
    }

    for (;it != it_end; ++it)
    {
      if (!it->empty() && *it != ".")
      {
        if (tail++)
          res += separator_;
        res += *it;
        has_path = true;
      }
    }
    if (has_path && *(--it_end) == ".")
      res += separator_;
    return res.empty() ? "." : res;
  }

  std::string
  path::volume_get() const
  {
#ifndef WIN32
    return "";
#else
    std::string res = boost_path_.root_name();
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

    std::string res = boost_path_.parent_path().directory_string();
    return path(res.empty() ? "." : res);
  }

  bool path::exists() const
  {
    bool res;
    try {
      res = fs::exists(boost_path_);
    }
    catch (...)
    {
      res = false;
    }
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
      throw Exception("unable to generate path for temporary file");
#endif
    return path(tmp_name);
  }

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

  const path::path_type
  path::components() const
  {
    path_type path_;
    fs::path::iterator it_end = boost_path_.end();
    fs::path::iterator it = boost_path_.begin();
    if (it == it_end)
      return path_;

    // Skip volume information
    if (absolute_get())
      ++it;
    for (;it != it_end; ++it)
      if (!it->empty() && *it != ".")
        path_.push_back(*it);

    return path_;
  }
}
