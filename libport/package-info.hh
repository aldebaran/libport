#if ! defined PACKAGE_INFO_HH
# define PACKAGE_INFO_HH

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

#endif // ! PACKAGE_INFO_HH
