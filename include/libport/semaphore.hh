#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# include <cassert>
# include <cstddef> // size_t

# include <boost/noncopyable.hpp>

# ifdef __APPLE__
#  include <string>
# endif

# include <libport/export.hh>
# include <libport/semaphore.h>
# include <libport/utime.hh>

namespace libport
{
  class Condition;

  class LIBPORT_API Semaphore: public boost::noncopyable
  {
  public:
    Semaphore(unsigned value = 0);
    ~Semaphore();

    /// Release.
    void operator++();
    /// Acquire.
    void operator--();

    void operator++(int);
    void operator--(int);

    /// Acquire \a c times.
    Semaphore& operator -= (unsigned c);

    /// Releases \a c times.
    Semaphore& operator += (unsigned c);

    /** Get semaphore with a limit of \b timeout in second.
     * Fixme: Use seconds != 0 work only on linux.
     */
    bool get(unsigned seconds = 0);
    bool uget(utime_t useconds = 0);

    /// The number of alive Semaphores (#constructed - #destroyed).
    /// Used in the test suite.
    static size_t instances();

  private:
    void destroy();
    sem_t* sem_;
# ifdef __APPLE__
    std::string name_;
# endif
    static size_t instances_;
    friend class Condition;
  };

} // namespace libport

# include <libport/semaphore.hxx>

#endif // !LIBPORT_SEMAPHORE_HH
