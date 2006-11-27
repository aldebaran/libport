#ifndef LIBPORT_SEMAPHORE_HH
# define LIBPORT_SEMAPHORE_HH

# if defined WIN32 || defined LIBPORT_WIN32
#  define _WIN32_WINNT 0x0400
#  include <windows.h>

namespace urbi
{
  typedef HANDLE sem_t;
  inline void sem_init(HANDLE *sem, int useless, int cnt)
  {
    *sem = CreateSemaphore(NULL, cnt, 100000, NULL);
  }
  inline void sem_post(HANDLE * sem)
  {
    ReleaseSemaphore(*sem, 1, NULL);
  }
  inline void sem_wait(HANDLE *sem)
  {
    WaitForSingleObject(*sem, INFINITE);
  }
  inline void sem_destroy(HANDLE * sem)
  {
    DeleteObject(*sem);
  }
  inline void sem_getvalue(HANDLE *sem, int *v)
  {
    *v=1; //TODO: implement
  }
}
# else
#  include <semaphore.h>
# endif

namespace urbi
{
  class Semaphore
  {
  public:
    Semaphore(int cnt=0)
    {
      sem_init(&sem,0,cnt);
    }
    ~Semaphore()
    {
      sem_destroy(&sem);
    }
    void operator ++(int) {sem_post(&sem);}
    void operator --(int) {sem_wait(&sem);}
    void operator ++() {sem_post(&sem);}
    void operator --() {sem_wait(&sem);}
    operator int()  {int t;sem_getvalue(&sem,&t); return t;}

  private:
    sem_t sem;
  };

} // namespace urbi

#endif
