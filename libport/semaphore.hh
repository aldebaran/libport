#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# include "libport/windows.h"
# include <cassert>

# if defined WIN32 || defined LIBPORT_WIN32

namespace urbi
{
  typedef HANDLE sem_t;
  void sem_init(HANDLE* sem, int useless, int cnt);
  void sem_post(HANDLE* sem);
  void sem_wait(HANDLE* sem);
  void sem_destroy(HANDLE* sem);
  void sem_getvalue(HANDLE* sem, int* v);
}
# else /* !WIN32 */
#  include <semaphore.h>
#  include <sstream>
# endif

# if defined __linux
/* Linux defines SEM_FAILED as  (sem_t *) 0 */
#  define IS_SEM_FAILED(sem)   ((sem) == SEM_FAILED)
# else
/* MacOSX (and thus BSD?) defines it as -1 */
#  define IS_SEM_FAILED(sem)   ((long) (sem) == (long) SEM_FAILED)
# endif

namespace urbi
{
  class Semaphore
  {
    public:
      Semaphore (int cnt = 0);

      ~Semaphore ();

      void operator++ (int);
      void operator-- (int);
      void operator++ ();
      void operator-- ();
      operator int ();

    private:
      sem_t* sem;
      static unsigned int counter_;
  };

  unsigned int Semaphore::counter_ = 0;

} // namespace urbi

# include "semaphore.hxx"

#endif /* !LIBPORT_SEMAPHORE_HH */
