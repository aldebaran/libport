#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# include <cassert>
# include <cstddef> // size_t

# ifdef __APPLE__
#  include <string>
# endif

# include <libport/export.hh>
# include <libport/detect-win32.h>

// Get sem_t.
# if defined WIN32
#  include <libport/windows.hh>
namespace libport
{
  typedef HANDLE sem_t;
}

# else /* !WIN32 */
#  include <semaphore.h>
# endif

namespace libport
{
  class Condition;

  class LIBPORT_API Semaphore
  {
  public:
    Semaphore(unsigned cnt = 0);
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

    operator int ();

    /** Get semaphore with a limit of \b timeout in second.
     * Fixme: Use timeout != 0 work only on linux.
     */
    bool get(const int timeout = 0);

    /// The number of alive Semaphores (#constructed - #destroyed).
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
