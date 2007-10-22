#if ! defined PACKAGE_INFO_HH
# define PACKAGE_INFO_HH

# include <iosfwd>
# include <string>

namespace libport
{
  struct PackageInfo
  {
    PackageInfo();
    const std::string
      bug_report,
      date,
      id,
      name,
      revision,
      string,
      tarname,
      version,
      version_rev;
  };
}

std::ostream& operator<<(std::ostream& o, const libport::PackageInfo& p);

#endif // ! PACKAGE_INFO_HH
