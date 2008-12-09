#include <sys/types.h>

#include <fstream>

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <libport/path.hh>
#include <libport/pid-file.hh>
#include <libport/program-name.hh>
#include <libport/unistd.h>

namespace libport
{
  PidFile::PidFile(boost::optional<std::string> path )
    : path_(".")
  {
    static boost::format fmt("/var/run/%s.pid");
    path_= path ? path.get()
      : str(fmt % libport::path(program_name).basename());
  }

  PidFile::~PidFile()
  {
    if (path_.exists())
      unlink(path_.to_string().c_str());
  }

  void
  PidFile::create()
  {
    if (path_.exists())
    {
      static boost::format fmt("pid file already exists: %s");
      throw std::runtime_error(str(fmt % path_));
    }
    std::ofstream out(path_.to_string().c_str(), std::ios::out);
    out << getpid() << std::endl;
  }
}
