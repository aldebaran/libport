/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/sys/types.h>

#include <fstream>

#include <libport/format.hh>
#include <boost/optional.hpp>

#include <libport/cassert>
#include <libport/path.hh>
#include <libport/pid-file.hh>
#include <libport/program-name.hh>
#include <libport/unistd.h>

namespace libport
{
  PidFile::PidFile(const std::string& path)
    : path_(path)
  {}

  PidFile::PidFile(int& argc,
                   const char* argv[],
                   const std::string& path,
                   const std::string& opt)
    : path_(path)
  {
    aver(argc > 0);

    for (int i = 1; i < argc; ++i)
      if (opt == argv[i] && i + 1 < argc)
      {
        path_ = argv[i + 1];
        for (int j = i; j < argc - 1; ++j)
          argv[j] = argv[j + 1];
        argc -= 2;
      }
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
    if (!out.good())
    {
      static boost::format fmt("unable to create PID file: %s");
      throw std::runtime_error(str(fmt % path_));
    }
    out << getpid() << std::endl;
  }

  bool check_program_name()
  {
    if (libport::program_name() == "")
      pabort("You forgot to set libport::program_name");
    return true;
  }

  std::string PidFile::default_filename()
  {
    static boost::format fmt("/var/run/%s.pid");
    static std::string res =
      (check_program_name(),
       str(fmt % libport::path(program_name()).basename()));
    return res;
  }
}
