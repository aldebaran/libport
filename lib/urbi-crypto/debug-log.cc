/*
 * Copyright (C) Gostai S.A.S., 2006-2008.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#include <iostream>
#include <string>
#include "urbi-crypto/utility.hh"
#include "urbi-crypto/debug-log.hh"
#include "libport/config.h"

#ifdef WIN32
# include <Windows.h>
#endif

namespace crypto
{

  DebugLog::DebugLog ()
  {
    char *debugEnabled = getenv ("ENABLE_URBI_DEBUG_LOG");
    if (debugEnabled)
    {
      std::cout << debugEnabled << std::endl;
      std::string tmp = debugEnabled;
      // check code: URBI_DEBUG
      if (tmp.size () == 10
	  && tmp[0] == 'U'
	  && tmp[1] == 'R'
	  && tmp[2] == 'B'
	  && tmp[3] == 'I'
	  && tmp[4] == '_'
	  && tmp[5] == 'D'
	  && tmp[6] == 'E'
	  && tmp[7] == 'B'
	  && tmp[8] == 'U'
	  && tmp[9] == 'G')
	log_enabled_ = true;
    }
    else
      log_enabled_ = false;
    out_inited_ = false;
  }

  DebugLog&
  DebugLog::instance ()
  {
    return *(ptrInstance ());
  }

  DebugLog*
  DebugLog::ptrInstance ()
  {
    static DebugLog newInstance;
    return &newInstance;
  }

  void
  DebugLog::setLogFile (const std::string& filename)
  {
    std::cerr << "Creating log: " << filename << std::endl;
    out_.open (filename.c_str (), std::ofstream::trunc);
    if (out_.fail ())
    {
      std::string msg = "Unable to open log file: \"";
      msg += filename + "\"";
      THROW_CRYPTO (msg.c_str ());
    }
    out_inited_ = true;
  }

  void
  DebugLog::closeLogFile ()
  {
    out_.close ();
    //std::cout << "log init = 0" << std::endl;
    out_inited_ = false;
  }

  void
  DebugLog::append (const std::string& msg)
  {
    if (!out_inited_)
      THROW_CRYPTO ("Debug log file is not set");
    out_ << msg;
  }

  std::ofstream&
  DebugLog::out ()
  {
    if (!out_inited_)
      THROW_CRYPTO ("Debug log file is not set");
    return out_;
  }

  bool
  DebugLog::logEnabled () const
  {
    return log_enabled_ && out_inited_;
  }

#ifndef WIN32

  void createDebugLog ()
  {
    /// Construct the log
    std::string tmp_file;
    if (!crypto::Utility::temporary_directory)
      tmp_file = "/tmp";
    else
      tmp_file = *crypto::Utility::temporary_directory;
    tmp_file += "/urbi-license.log";
    crypto::DebugLog::instance ().setLogFile (tmp_file);
  }

#else

  void createDebugLog ()
  {
    /// Construct the log
    char tmp_path[MAX_PATH];
    memset (tmp_path, 0, MAX_PATH);
    GetTempPath(MAX_PATH, tmp_path);
    std::string tmp_file;
    if (!crypto::Utility::temporary_directory)
      tmp_file = tmp_path;
    else
      tmp_file = *crypto::Utility::temporary_directory;
    tmp_file += "/urbi-license.log";
    crypto::DebugLog::instance ().setLogFile (tmp_file);
  }

#endif /* !WIN32 */

}
