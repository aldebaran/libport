/**
 ** \file libport/path.cc
 ** \brief path: implements file libport/path.hh
 */

#include <iostream>
#include <sys/stat.h>
#include <cctype>

#include <libport/detect-win32.h>
#include <libport/contract.hh>
#include <libport/finally.hh>
#include <libport/path.hh>

// Implementation detail: if path_ is empty and absolute_ is false,
// then the path is '.'


namespace libport
{
  path::path (const std::string& p)
  {
    init (p);
  }

  path::path (const char* p)
  {
    init (p);
  }

  void
  path::test_absolute (std::string& p)
  {
#if defined WIN32
    // Under Win32, absolute paths start with a letter followed by
    // ":\". If the trailing slash is missing, this is a relative
    // path.
    if (p.length() >= 2 &&
	isalpha(p[0]) &&
	p[1] == ':')
    {
      volume_ = p.substr(0, 2);
      absolute_ = p.length() >= 3 && p[2] == '\\';
      p = absolute_ ? p.erase(0, 3) : p.erase(0, 2);
    }
    // Network share, such as "\\shared volume\foo\bar"
    else if (p.length() >= 3 &&
	     p[0] == '\\' &&
	     p[1] == '\\')
    {
      absolute_ = true;
      std::string::size_type pos = p.find("\\", 3);
      if (pos == std::string::npos)
      {
	volume_ = p;
	p = "";
      }
      else
      {
	volume_ = p.substr(0, pos);
	p = p.erase(0, pos);
      }
    }
    // Fallback to Unix cases for subsystems such as cygwin
    else
#endif // WIN32
    if ((p.length() > 0) && (p[0] == '/'))
    {
      absolute_ = true;
      p = p.erase(0, 0);
    }
    else
      absolute_ = false;
  }

  void
  path::init (std::string p)
  {
    static std::string sep =
#ifdef WIN32
      "/\\";
#else
      "/";
#endif // WIN32

    if (p.empty())
      throw invalid_path("Path can't be empty.");

    test_absolute(p);

    // Cut directories on / and \.

    for (std::string::size_type pos = p.find_first_of(sep);
	 pos != std::string::npos;
	 pos = p.find_first_of(sep))
    {
      this->append_dir(p.substr(0, pos));
      p.erase (0, pos + 1);
    }
    this->append_dir(p);
  }

  path&
  path::operator= (const path& rhs)
  {
    absolute_ = rhs.absolute_;
    path_ = rhs.path_;
#if defined WIN32
    volume_ = rhs.volume_;
#endif
    return *this;
  }

  path&
  path::operator/= (const path& rhs)
  {
    if (rhs.absolute_get())
      throw invalid_path(
        "Rhs of concatenation is absolute: " + rhs.to_string());
#ifdef WIN32
    if (!rhs.volume_.empty())
      throw invalid_path("concatenation of path with volume: " +
			 rhs.to_string());
#endif
    for (path_type::const_iterator dir = rhs.path_.begin ();
	 dir != rhs.path_.end ();
	 ++dir)
      this->append_dir (*dir);

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
      if (!volume_.empty())
      {
	path_str = volume_ + "\\" + path_str;
	separator = '\\';
      }
      else
#endif
      {
	path_str = "/" + path_str;
	separator = '/';
      }
    }
    else if (path_.empty())
      return ".";

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
  path::append_dir (const std::string& dir)
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
	path_.push_back (dir);
    }
  }


  std::string
  path::basename () const
  {
    // basename of / is /, basename of . is .
    if (path_.empty())
      return *this;

    return path_.back();
  }

  path
  path::dirname () const
  {
    // dirname of / is /, dirname of . is .
    if (path_.empty())
      return *this;

    std::string last = path_.back();
    // The const cast here is justified since the modification is
    // systematically reverted
    const_cast<path*>(this)->path_.pop_back();
    libport::Finally finally(
      boost::bind(&path_type::push_back,
		  boost::ref(const_cast<path*>(this)->path_), last));
    path res = path_.empty() ? "." : *this;
    return res;
  }

  bool path::exists () const
  {
    struct stat buf;
    return 0 == stat (to_string().c_str(), &buf);
  }

  const path::path_type& path::components() const
  {
    return path_;
  }
}
