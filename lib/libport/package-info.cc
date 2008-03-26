#include <iostream>
#include <boost/foreach.hpp>
#include "libport/package-info.hh"

namespace libport
{
  PackageInfo::PackageInfo ()
    : map_()
  {
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
    return (
      get ("name")
      + " version " + get ("version")
      + " rev. "    + get ("revision")
      + " (C) "     + get ("copyright-years")
      + " "         + get ("copyright-holder")
      );
  }

  PackageInfo::data_type&
  PackageInfo::operator[](const key_type& k)
  {
    return map_[k];
  }

  const PackageInfo::data_type&
  PackageInfo::get(const key_type& k) const
  {
    return map_.find(k)->second;
  }

  std::ostream&
  PackageInfo::dump(std::ostream& o = std::cerr) const
  {
    bool not_first = false;
    BOOST_FOREACH(value_type p, map_)
      o << (not_first++ ? ", " : "") << p.first << " = " << p.second;
    return o;
  }

  std::ostream&
  operator<<(std::ostream& o, const PackageInfo& p)
  {
    return o << p.signature();
  }

}
