/**
 ** \file libport/path.cc
 ** \brief path: implements file libport/path.hh
 */

#include <iostream>
#include <sys/stat.h>
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
    std::string::size_type pos;

    absolute_ = p[0] == '/';

    while ((pos = p.find ("/")) != std::string::npos)
    {
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
    return *this;
  }

  path&
  path::operator/= (const path& rhs)
  {
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

    for (path_type::const_iterator dir = path_.begin ();
	 dir != path_.end ();
	 ++dir)
    {
      path_str += '/';
      path_str += *dir;
    }

    // Erase the initial / if needed.
    if (!absolute_)
      path_str.erase (0, 1);

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
    precondition(dir.find ('/') == std::string::npos);

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
