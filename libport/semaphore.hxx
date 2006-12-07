#ifndef SEMAPHORE_HXX_
# define SEMAPHORE_HXX_

# include "semaphore.hh"

namespace urbi
{

# if defined WIN32 || defined LIBPORT_WIN32
  inline sem_t*
  sem_open (const char* name,
            int oflag,
            unsigned int /*mode_t*/ mode,
            unsigned int value)
  {
    return SEM_FAILED; // Use sem_init instead.
  }

  inline void sem_init(sem_t* sem, int useless, int cnt)
  {
    *sem = CreateSemaphore(0, cnt, 100000, 0);
  }

  inline void sem_post(sem_t* sem)
  {
    ReleaseSemaphore(*sem, 1, 0);
  }

  inline void sem_wait(sem_t* sem)
  {
    WaitForSingleObject(*sem, INFINITE);
  }

  inline void sem_destroy(sem_t* sem)
  {
    DeleteObject(*sem);
  }

  inline void sem_getvalue(sem_t* sem, int* v)
  {
    *v = 1; //TODO: implement
  }
#endif /* !WIN32 */

  inline Semaphore::Semaphore (int cnt)
  {
    std::stringstream s;

    s << "sema_";
    s << counter_++;
    sem = sem_open (s.str ().c_str (), O_CREAT, 0777, cnt);
    if (IS_SEM_FAILED (sem))
      sem_init(sem, 0, cnt);
    assert (!IS_SEM_FAILED (sem));
  }

  inline Semaphore::~Semaphore ()
  {
    sem_destroy(sem);
  }


  inline void
  Semaphore::operator++ (int)
  {
    sem_post(sem);
  }

  inline void
  Semaphore::operator-- (int)
  {
    sem_wait(sem);
  }

  inline void
  Semaphore::operator++ ()
  {
    sem_post(sem);
  }

  inline void
  Semaphore::operator-- ()
  {
    sem_wait(sem);
  }

  inline
  Semaphore::operator int ()
  {
    int t;
    sem_getvalue(sem, &t);
    return t;
  }

} // namespace urbi

#endif /* !SEMAPHORE_HXX_ */
