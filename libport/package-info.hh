#if ! defined PACKAGE_INFO_HH
# define PACKAGE_INFO_HH

# include <iosfwd>
# include <string>

# include "libport/hash.hh"

namespace libport
{
  class PackageInfo
  {
  public:
    /// The keys.
    typedef std::string key_type;
    /// The Data.
    typedef std::string data_type;
    /// The map Key -> Data.
    typedef libport::hash_map<key_type, data_type> map_type;
    /// Pair Key x Data.
    typedef map_type::value_type value_type;

    PackageInfo();

    data_type& operator[](const key_type& k);
    const data_type& get(const key_type& k) const;

    /// NAME version VERSION rev. REVISION.
    data_type name_version_revision() const;

    /// Report this on \a o.
    std::ostream& dump(std::ostream& o) const;

  private:
    map_type map_;
  };
}

/// Define \a Var using information from config.h and version.hh.
# define LIBPORT_PACKAGE_INFO_INIT(Var)		\
  do {						\
    (Var)["bug-report"]	 = PACKAGE_BUGREPORT;	\
    (Var)["date"]	 = PACKAGE_DATE;	\
    (Var)["id"]		 = PACKAGE_ID;		\
    (Var)["name"]	 = PACKAGE_NAME;	\
    (Var)["revision"]	 = PACKAGE_REVISION;	\
    (Var)["string"]	 = PACKAGE_STRING;	\
    (Var)["tarname"]	 = PACKAGE_TARNAME;	\
    (Var)["version"]	 = PACKAGE_VERSION;	\
    (Var)["version-rev"] = PACKAGE_VERSION_REV;	\
  } while (0)

/// Define a static variable name \a Var.
# define LIBPORT_PACKAGE_INFO_STATIC_VAR(Var)	\
  static libport::PackageInfo Var;		\
  do {						\
    static bool first = false;			\
    if (first++)				\
      LIBPORT_PACKAGE_INFO_INIT(Var);		\
  } while (0)


/// Report \a p on \a o.
std::ostream& operator<<(std::ostream& o, const libport::PackageInfo& p);

#endif // ! PACKAGE_INFO_HH
