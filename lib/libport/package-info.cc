/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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
      + " "         + get ("copyright-holder")
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
    if (boost::starts_with(res, "GIT-VERSION-GEN("))
      // Skip up to the first closing paren.
      res = res.substr(res.find(')') + 1);
    if (res.empty())
      res = "<EMPTY>";
    return res;
  }

  std::ostream&
  PackageInfo::dump(std::ostream& o) const
  {
    bool tail = false;
    foreach (value_type p, map_)
      o << (tail++ ? ", " : "") << p.first << " = " << p.second;
    return o;
  }

  std::ostream&
  operator<<(std::ostream& o, const PackageInfo& p)
  {
    return o << p.signature();
  }

}
