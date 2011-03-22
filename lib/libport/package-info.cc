/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>

#include <libport/foreach.hh>
#include <libport/package-info.hh>
#include <libport/lexical-cast.hh>

#include <boost/algorithm/string.hpp>

namespace libport
{
  PackageInfo::PackageInfo ()
    : map_()
  {
  }

  void
  PackageInfo::dependency_add(const PackageInfo& p)
  {
    dependencies_.insert(&p);
  }

  PackageInfo::data_type
  PackageInfo::name_version_revision() const
  {
    return (get ("name")
	    + " version " + get ("version")
	    + " rev. "    + get ("revision"));
  }

  PackageInfo::data_type
  PackageInfo::signature() const
  {
    data_type res;
    res =
      name_version_revision() + "\n"
      + "Copyright (C) " + get ("copyright-years")
      + " " + get ("copyright-holder")
      + "."
      ;

    foreach (const PackageInfo *p, dependencies_)
      res += "\n\n"
      + p->signature();

    return res;
  }

  PackageInfo::data_type
  PackageInfo::report_bugs() const
  {
    return "Report bugs to <" + get("bug-report") + ">.";
  }

  PackageInfo::data_type&
  PackageInfo::operator[](const key_type& k)
  {
    return map_[k];
  }

  PackageInfo::data_type
  PackageInfo::get(const key_type& k) const
  {
    data_type res = map_.find(k)->second;
    if (boost::starts_with(res, "VERSIONIFY("))
      // Skip up to the first closing paren.
      res = res.substr(res.find(')') + 1);
    if (res.empty())
      res = "<EMPTY>";
    return res;
  }

  std::ostream&
  PackageInfo::dump(std::ostream& o) const
  {
    bool first = true;
    foreach (value_type p, map_)
    {
      o << (first ? "" : ", ") << p.first << " = " << p.second;
      first = false;
    }
    return o;
  }

#define DEFINE(Fun, Field)                              \
  PackageInfo::integer_type                             \
  PackageInfo::Fun() const                              \
  {                                                     \
    return lexical_cast<integer_type>(get(#Field));     \
  }

  DEFINE(version_major, "major")
  DEFINE(version_minor, "minor")
  DEFINE(version_subminor, "subminor")
  DEFINE(version_patch, "patch")
  DEFINE(version_value, "version-value")
#undef DEFINE

  std::ostream&
  operator<<(std::ostream& o, const PackageInfo& p)
  {
    return o << p.signature();
  }

  PackageInfo::Version
  PackageInfo::version() const
  {
    return Version(version_major(), version_minor(), version_subminor(),
                   version_patch());
  }

  PackageInfo::Version::Version(integer_type major,
                                integer_type minor,
                                integer_type subMinor,
                                integer_type patchLevel)
  : subMinor(subMinor)
  , patchLevel(patchLevel)
  {
    // Workaround glibc macros major(x) and minor(x)
    this->major = major;
    this->minor = minor;
  }

  bool PackageInfo::Version::operator < (const PackageInfo::Version& v) const
  {
    if (major < v.major) return true;
    if (major > v.major) return false;
    if (minor < v.minor) return true;
    if (minor > v.minor) return false;
    if (subMinor < v.subMinor) return true;
    if (subMinor > v.subMinor) return false;
    return patchLevel < v.patchLevel;
  }

  bool PackageInfo::Version::operator <= (const PackageInfo::Version& v) const
  {
    return *this == v || *this < v;
  }

  bool PackageInfo::Version::operator > (const PackageInfo::Version& v) const
  {
    return ! (*this <= v);
  }

  bool PackageInfo::Version::operator >= (const PackageInfo::Version& v) const
  {
    return ! (*this < v);
  }

  bool PackageInfo::Version::operator == (const PackageInfo::Version& v) const
  {
    return major == v.major
      && minor == v.minor
      && subMinor == v.subMinor
      && patchLevel == v.patchLevel;
  }

  bool PackageInfo::Version::operator != (const PackageInfo::Version& v) const
  {
    return ! (*this == v);
  }

  std::ostream&
  operator<<(std::ostream& o, const PackageInfo::Version& v)
  {
    return o << '('
      << v.major << ", "
      << v.minor << ", "
      << v.subMinor << ", "
      << v.patchLevel << ')';
  }

}
