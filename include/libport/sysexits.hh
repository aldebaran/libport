/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/sysexits.hh
/// \brief Provide sysexits.h portably, and in C++ rather than in Cpp.

#ifndef LIBPORT_SYSEXITS_HH
# define LIBPORT_SYSEXITS_HH

# include <iosfwd>

# include <libport/export.hh>
# include <libport/compiler.hh>

// FIXME: Should be moved in libport and lowercased?
enum sysexit
{
  EX_OK		   =  0, ///< successful termination
  EX_FAIL	   =  1, ///< unspecified failure
  EX__BASE	   = 64, ///< base value for error messages
  EX_USAGE	   = 64, ///< command line usage error
  EX_DATAERR	   = 65, ///< data format error
  EX_NOINPUT	   = 66, ///< cannot open input
  EX_NOUSER	   = 67, ///< addressee unknown
  EX_NOHOST	   = 68, ///< host name unknown
  EX_UNAVAILABLE   = 69, ///< service unavailable
  EX_SOFTWARE	   = 70, ///< internal software error
  EX_OSERR	   = 71, ///< system error (e.g., can't fork)
  EX_OSFILE	   = 72, ///< critical OS file missing
  EX_CANTCREAT	   = 73, ///< can't create (user) output file
  EX_IOERR	   = 74, ///< input/output error
  EX_TEMPFAIL	   = 75, ///< temp failure; user is invited to retry
  EX_PROTOCOL	   = 76, ///< remote error in protocol
  EX_NOPERM	   = 77, ///< permission denied
  EX_CONFIG	   = 78, ///< configuration error

  EX_SKIP          = 176, ///< skip this test with unmet dependencies
  EX_HARD          = 177, ///< hard error that cannot be saved (ICE)

  EX__MAX	   = 177, ///< maximum listed value
};

namespace libport
{
  class LIBPORT_API exit
  {
  public:
    explicit exit(unsigned s);
    ATTRIBUTE_NORETURN void operator() () const;
  private:
    unsigned status_;
  };

  LIBPORT_API ATTRIBUTE_NORETURN
  std::ostream& operator<<(std::ostream& o, const exit& e);

}

#endif // !LIBPORT_SYSEXITS_HH
