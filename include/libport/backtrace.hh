/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_BACKTRACE_HH
# define LIBPORT_BACKTRACE_HH

# include <iosfwd>
# include <string>
# include <vector>

# include <libport/export.hh>

namespace libport
{
  /// Storing the description of the stack trace.
  /// Used to be a vector<char*>, but was wrong, because the
  /// array of char* was malloced as a whole, and freeing it
  /// not only frees the array, but the char* too.  So duplicated
  /// everything.
  typedef std::vector<std::string> backtrace_type;

  /// Store stack trace as a collection of strings in \a bt.
  /// \return \a bt.
  LIBPORT_API backtrace_type& backtrace(backtrace_type& bt);

  /// Return the stack trace as a collection of strings.
  /// There is nothing particular to do about the memory, just
  /// clear the container when done.
  LIBPORT_API backtrace_type backtrace();


  /// Using this object is more efficient that backtrace()
  /// as it avoids useless copies.
  class LIBPORT_API Backtrace
  {
  public:
    /// Build the stack trace now.
    Backtrace();
    const backtrace_type& backtrace_get() const;
    std::ostream& dump(std::ostream& o) const;

    typedef backtrace_type::const_iterator const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

  private:
    backtrace_type backtrace_;
  };

  std::ostream& operator<<(std::ostream& o, const Backtrace& b);
}

# include <libport/backtrace.hxx>

#endif // !LIBPORT_BACKTRACE_HH
