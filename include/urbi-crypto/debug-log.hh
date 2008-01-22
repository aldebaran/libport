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

#ifndef DEBUG_LOG_HH_
# define DEBUG_LOG_HH_

# include "urbi-crypto/exception.hh"

//# define REAL_DEBUG	/// Error messages will be printed to debug log
# define DEBUG_LOG()					\
  if (::crypto::DebugLog::instance ().logEnabled ())	\
    ::crypto::DebugLog::instance ().out ()


namespace crypto
{
  class DebugLog
  {
  public:

    static DebugLog& instance ();

    static DebugLog* ptrInstance ();

    void setLogFile (const std::string& filename);

    void closeLogFile ();

    void append (const std::string& msg);

    std::ofstream& out ();

    bool logEnabled () const;

  private:
    explicit DebugLog ();

  private:

    bool		log_enabled_;
    std::ofstream	out_;
    bool		out_inited_;
  };

  void createDebugLog ();
}

#endif
