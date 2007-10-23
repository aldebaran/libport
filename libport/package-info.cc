#include <iostream>
#include "libport/package-info.hh"

namespace libport
{
  std::string
  PackageInfo::name_version_revision() const
  {
    return name + " version " + version + " rev. " + revision;
  }

}

std::ostream&
operator<<(std::ostream& o, const libport::PackageInfo& p)
{
# define DISPLAY1(Field)				\
  << #Field " = " << p.Field
# define DISPLAY2(Field)				\
  DISPLAY1(Field) << ", "
  return o
    DISPLAY1(bug_report)
    DISPLAY2(date)
    DISPLAY2(id)
    DISPLAY2(name)
    DISPLAY2(revision)
    DISPLAY2(string)
    DISPLAY2(tarname)
    DISPLAY2(version)
    DISPLAY2(version_rev)
    ;
# undef DISPLAY2
# undef DISPLAY1
}
