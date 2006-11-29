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
#  include <sstream>
# endif

namespace urbi
{
  class Semaphore
  {
    public:
      Semaphore(int cnt=0)
      {
	std::stringstream s;

	s << "sema_";
	s << counter_++;
	sem = sem_open (s.str ().c_str (), O_CREAT, 0777, cnt);
	if ((int) sem == SEM_FAILED)
	  sem_init(sem, 0, cnt);
	assert ((int) sem != SEM_FAILED);
      }
      ~Semaphore()
      {
	sem_destroy(sem);
      }
      void operator ++(int) {sem_post(sem);}
      void operator --(int) {sem_wait(sem);}
      void operator ++() {sem_post(sem);}
      void operator --() {sem_wait(sem);}
      operator int()  {int t;sem_getvalue(sem,&t); return t;}

    private:
      sem_t* sem;
      static unsigned int counter_;
  };

  unsigned int Semaphore::counter_ = 0;

} // namespace urbi

#endif
