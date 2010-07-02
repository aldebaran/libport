/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_EXCEPTION_HH
# define SCHED_EXCEPTION_HH

# include <string>

# include <boost/optional.hpp>

# include <libport/compiler.hh>
# include <sched/export.hh>

namespace sched
{
  class exception;
  typedef std::auto_ptr<exception> exception_ptr;

  class SCHED_API exception: public std::exception
  {
  public:
    virtual ~exception() throw();
    virtual const char* what() const throw();
    virtual exception_ptr clone() const = 0;
    ATTRIBUTE_NORETURN void rethrow() const;
  protected:
    ATTRIBUTE_NORETURN virtual void rethrow_() const = 0;
  };

/// \def ADD_FIELD(Type, Name)
/// Define an optional field Name, and accessors.
#define ADD_FIELD(Type, Name)						\
 public:                                                                \
   bool Name ## _is_set() const { return Name ## _; };			\
   const Type& Name ## _get() const { return Name ## _ .get(); };	\
   void Name ## _set(const Type& data) { Name ## _ = data; };		\
 private:								\
   boost::optional<Type> Name ## _;

#define PARTIAL_COMPLETE_EXCEPTION(Name)                  \
public:                                                   \
  virtual ::sched::exception_ptr clone() const            \
  {                                                       \
    return ::sched::exception_ptr(new Name(*this));       \
  }                                                       \
private:                                                  \
  ATTRIBUTE_NORETURN virtual void rethrow_() const        \
  {                                                       \
    throw *this;                                          \
  };

#define COMPLETE_EXCEPTION(Name)			\
  PARTIAL_COMPLETE_EXCEPTION(Name)			\
public:							\
 virtual const char* what() const throw()               \
 {                                                      \
   return #Name;                                        \
 }                                                      \
private:

} // namespace sched

# include <sched/exception.hxx>

#endif // SCHED_EXCEPTION_HH
