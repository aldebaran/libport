#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# include <cassert>

# ifdef __APPLE__
#  include <string>
# endif

# include <libport/export.hh>
# include <libport/detect-win32.h>

# if defined WIN32
namespace libport
{
  typedef HANDLE sem_t;
  sem_t* sem_open(const char* name, int oflag,
                  unsigned int /*mode_t*/ mode,
                  unsigned int value);
  int sem_init(sem_t* sem, int useless, int cnt);
  int sem_post(sem_t* sem);
  int sem_wait(sem_t* sem);
  int sem_destroy(sem_t* sem);
  int sem_getvalue(sem_t* sem, int* v);
}
# else /* !WIN32 */
#  include <semaphore.h>
# endif

namespace libport
{
  class LIBPORT_API Semaphore
  {
  public:
    Semaphore(int cnt = 0);
    ~Semaphore();
    void operator++ (int);
    void operator-- (int);
    void operator++ ();
    void operator-- ();
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
  };

} // namespace libport

# include <libport/semaphore.hxx>

#endif // !LIBPORT_SEMAPHORE_HH
