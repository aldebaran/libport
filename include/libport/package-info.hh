/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PACKAGE_INFO_HH
# define LIBPORT_PACKAGE_INFO_HH

# include <iostream>
# include <set>
# include <string>

# include <libport/export.hh>
# include <libport/hash.hh>

namespace libport
{
  class LIBPORT_API PackageInfo
  {
  public:
    /// The keys.
    typedef std::string key_type;
    /// The Data.
    typedef std::string data_type;
    /// The map Key -> Data.
    typedef boost::unordered_map<key_type, data_type> map_type;
    /// Pair Key x Data.
    typedef map_type::value_type value_type;

    PackageInfo();

    /// Register a dependency (the package info of a sub library).
    void dependency_add(const PackageInfo& p);

    data_type& operator[](const key_type& k);
    data_type get(const key_type& k) const;

    /// NAME version VERSION rev. REVISION.
    data_type name_version_revision() const;
    /// name_version_revision + "\n" + "Copyright (C) YEAR OWNER."
    /// Also includes that of the dependencies.  Does not end with eol.
    data_type signature() const;
    /// "Report bugs to <BUGREPORT>.".
    data_type report_bugs() const;

    /// A type to return version numbers as numbers.
    typedef unsigned int integer_type;

    /// "major" as an integer.
    integer_type version_major() const;
    /// "minor" as an integer.
    integer_type version_minor() const;
    /// "sub-minor" as an integer.
    integer_type version_subminor() const;
     /// "patch" as an integer.
    integer_type version_patch() const;
    /// "version-value" as an integer.
    integer_type version_value() const;

    /// Report this on \a o.
    /// Does not end with a std::endl.
    /// For debugging.
    std::ostream& dump(std::ostream& o = std::cerr) const;

    struct LIBPORT_API Version
    {
      integer_type major, minor, subMinor, patchLevel;
      Version(integer_type major=0,
              integer_type minor=0,
              integer_type subMinor=0,
              integer_type patchLevel=0);
      bool operator == (const Version& v) const;
      bool operator != (const Version& v) const;
      bool operator <  (const Version& v) const;
      bool operator >  (const Version& v) const;
      bool operator >= (const Version& v) const;
      bool operator <= (const Version& v) const;
    };
    Version version() const;

  private:
    map_type map_;
    /// Sub packages (Dependencies).
    typedef std::set<const PackageInfo*> dependency_type;
    dependency_type dependencies_;
  };

  /// Report the signature of \a p on \a o.
  LIBPORT_API std::ostream&
  operator<<(std::ostream& o, const PackageInfo& p);
  LIBPORT_API std::ostream&
  operator<<(std::ostream& o, const PackageInfo::Version& p);
}

/// Define \a Var using information from config.h and version.hh.
# define LIBPORT_PACKAGE_INFO_INIT_(Prefix, Var)                \
  do {                                                          \
    (Var)["bug-report"]         = Prefix ## BUGREPORT;          \
    (Var)["copyright-holder"]   = Prefix ## COPYRIGHT_HOLDER;   \
    (Var)["copyright-years"]    = Prefix ## COPYRIGHT_YEARS;    \
    (Var)["date"]               = Prefix ## DATE;               \
    (Var)["id"]                 = Prefix ## ID;                 \
    (Var)["major"]              = Prefix ## MAJOR;              \
    (Var)["minor"]              = Prefix ## MINOR;              \
    (Var)["name"]               = Prefix ## NAME;               \
    (Var)["patch"]              = Prefix ## PATCH;              \
    (Var)["revision"]           = Prefix ## REVISION;           \
    (Var)["string"]             = Prefix ## STRING;             \
    (Var)["subminor"]           = Prefix ## SUB_MINOR;           \
    (Var)["tarname"]            = Prefix ## TARNAME;            \
    (Var)["version"]            = Prefix ## VERSION;            \
    (Var)["version-rev"]        = Prefix ## VERSION_REV;        \
    (Var)["version-value"]      = Prefix ## VERSION_VALUE;      \
  } while (0)

/// Define \a Var using information from config.h and version.hh.
# define LIBPORT_PACKAGE_INFO_INIT(Var)         \
  LIBPORT_PACKAGE_INFO_INIT_(PACKAGE_, Var)

/// Define a static variable name \a Var.
# define LIBPORT_PACKAGE_INFO_STATIC_VAR_(Prefix, Var)	\
  static libport::PackageInfo Var;                      \
  do {                                                  \
    static bool first = true;                           \
    if (first)                                          \
    {                                                   \
      first = false;                                    \
      LIBPORT_PACKAGE_INFO_INIT_(Prefix, Var);          \
    }                                                   \
  } while (0)

/// Define a static variable name \a Var.
# define LIBPORT_PACKAGE_INFO_STATIC_VAR(Var)           \
  LIBPORT_PACKAGE_INFO_STATIC_VAR_(PACKAGE_, Var)

#endif // !LIBPORT_PACKAGE_INFO_HH
