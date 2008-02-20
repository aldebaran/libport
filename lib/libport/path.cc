/**
 ** \file libport/path.cc
 ** \brief path: implements file libport/path.hh
 */

#include <iostream>
#include <sys/stat.h>
#include <cctype>
#include "libport/contract.hh"
#include "libport/path.hh"

namespace libport
{
  path::path () :
    absolute_ (false)
  {
  }

  path::path (std::string p)
  {
    init (p);
  }

  path::path (const char* p)
  {
    init (p);
  }

  void
  path::init (std::string p)
  {
    absolute_ = false;

#if defined WIN32
    // Under Win32, absolute paths start with a letter followed by
    // ':\'
    absolute_ = isalpha(p[0]);
    absolute_ = absolute_ && (p[1] == ':');
    absolute_ = absolute_ && (p[2] == '\\');

    if (absolute_)
    {
      volume_ = p[0];
      p = p.erase(0, 2);
    }
#endif

    // Under unix, absolute paths start with a slash
    if (!absolute_)
    {
      absolute_ = (p[0] == '/');

      if (absolute_)
	p = p.erase(0, 0);
    }

    // Cut directories on / and \.
    std::string::size_type pos_s;
    std::string::size_type pos_b;

    while ((pos_s = p.find ('/')) != std::string::npos ||
	   (pos_b = p.find ('\\')) != std::string::npos)
    {
      std::string::size_type pos =
	pos_s == std::string::npos ? pos_b :
	pos_b == std::string::npos ? pos_s :
	std::min(pos_s, pos_b);

      std::string dir;

      dir = p.substr (0, pos);
      p.erase (0, pos + 1);

      this->append_dir (dir);
    }

    this->append_dir (p);
  }

  path&
  path::operator= (const path& rhs)
  {
    absolute_ = rhs.absolute_;
    path_ = rhs.path_;
#if WIN32
    volume_ = rhs.volume_;
#endif
    return *this;
  }

  path&
  path::operator/= (const path& rhs)
  {
    precondition(!rhs.absolute_);
    for (path_type::const_iterator dir = rhs.path_.begin ();
	 dir != rhs.path_.end ();
	 ++dir)
    {
      this->append_dir (*dir);
    }

    return *this;
  }

  path
  path::operator/ (const std::string& rhs) const
  {
    path ret = *this;

    return ret /= rhs;
  }

  path::operator std::string () const
  {
    return to_string();
  }

  std::string
  path::to_string () const
  {
    std::string path_str;
    char separator = separator_;

    if (absolute_)
    {
#ifdef WIN32
      if (volume_ != "")
      {
	path_str = volume_ + ":\\" + path_str;
	separator = '\\';
      }
      else
#endif
      {
 	path_str = "/" + path_str;
	separator = '/';
      }
    }

    bool first = true;
    for (path_type::const_iterator dir = path_.begin ();
	 dir != path_.end ();
	 ++dir)
    {
      if (first)
	first = false;
      else
	path_str += separator;
      path_str += *dir;
    }

    return path_str;
  }

  bool
  path::operator== (const path& rhs) const
  {
    return path_ == rhs.path_;
  }

  std::ostream&
  path::dump (std::ostream& ostr) const
  {
    ostr << this->operator std::string ();
    return ostr;
  }

  void
  path::append_dir (std::string dir)
  {
    precondition(dir.find (separator_) == std::string::npos);

    if (dir != "" && dir != ".")
    {
      if (dir == "..")
      {
	if (path_.empty () || *path_.rbegin () == "..")
	  path_.push_back ("..");
	else
	  path_.pop_back ();
      }
      else
      {
	path_.push_back (dir);
      }
    }
  }


  std::string
  path::basename () const
  {
    return path_.back();
  }

  path
  path::dirname () const
  {
    if (!path_.empty())
    {
      std::string last = path_.back();
      // The const cast here is justified since the modification is
      // reverted two lines below.
      const_cast<path*>(this)->path_.pop_back();
      path res = *this;
      const_cast<path*>(this)->path_.push_back(last);
      return res;
    }
    else
      return *this;
  }

  bool path::exists () const
  {
    struct stat buf;
    return 0 == stat (to_string().c_str(), &buf);
  }

}
