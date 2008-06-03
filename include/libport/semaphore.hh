#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# include <cassert>
# include <libport/windows.hh>

# if defined WIN32
namespace libport
{
#  ifndef SEM_FAILED
#   define SEM_FAILED 0
#  endif
  typedef HANDLE sem_t;
  sem_t* sem_open (const char* name,
		   int oflag,
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

# if defined __linux || defined WIN32
/* Linux defines SEM_FAILED as  (sem_t *) 0 */
#  define IS_SEM_FAILED(sem)   ((sem) == SEM_FAILED)
# else
/* MacOSX (and thus BSD?) defines it as -1 */
#  define IS_SEM_FAILED(sem)   ((long) (sem) == (long) SEM_FAILED)
# endif

namespace libport
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
      sem_t* sem_;
  };

} // namespace libport

# include <libport/semaphore.hxx>

#endif // !LIBPORT_SEMAPHORE_HH
