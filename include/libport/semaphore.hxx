#ifndef LIBPORT_SEMAPHORE_HXX
# define LIBPORT_SEMAPHORE_HXX

# include "libport/cstring"
# include "libport/exception.hh"
# include "libport/semaphore.hh"
# include <sstream>
# include <string>

namespace libport
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

  inline int sem_init(sem_t* sem, int, int cnt)
  {
    *sem = CreateSemaphore(0, cnt, 100000, 0);
    if(sem == 0)
    {
      std::stringstream s;
      s << GetLastError();
      std::string msg = s.str();
      throw libport::exception::Semaphore("sem_init",
					  "Windows system Error #" + msg
					  + " in CreateSemaphore.");
    }
    return sem?0:-1;
  }

  inline int sem_post(sem_t* sem)
  {
    unsigned int result = ReleaseSemaphore(*sem, 1, 0);
    if(result==0)
    {
      std::stringstream s;
      s << GetLastError();
      std::string msg = s.str();
      throw libport::exception::Semaphore("sem_post",
					  "Windows system Error #" + msg
					  + " in ReleaseSemaphore.");
    }
    return result?0:-1;
  }

  inline int sem_wait(sem_t* sem)
  {
    unsigned int result = WaitForSingleObject(*sem, INFINITE);
    if(result == WAIT_FAILED)
    {
      std::stringstream s;
      s << GetLastError();
      std::string msg = s.str();
      throw libport::exception::Semaphore("sem_wait",
					  "Windows system Error # " + msg
					  + " in WaitForSingleObject.");
    }
    return (result == WAIT_FAILED)?-1:0;
  }

  inline int sem_destroy(sem_t* sem)
  {
    return CloseHandle(*sem)?0:-1;
  }

  inline int sem_getvalue(sem_t* sem, int* v)
  {
    *v = 1; //TODO: implement
    // Maybe look at: http://www.codeguru.com/Cpp/W-P/win32/article.php/c1423
    return 0;
  }
#endif /* !WIN32 */

} // namespace libport

/* Darwin doesn't implement sem_init/sem_close (although the functions exists
 * and is defined, in fact it just returns ENOSYS). That's why need to use
 * named semaphores (with sem_open) instead.
 * See:
 * http://lists.apple.com/archives/darwin-development/2004/May/msg00077.html
 * http://www.google.com/search?as_q=sem_init&as_sitesearch=apple.com
 *
 * Semaphores created with sem_open must be closed with sem_close.
 * Semaphores created with sem_init must be closed with sem_destroy.
 */
# ifdef __APPLE__
#  include <sstream>
# endif

# include <cerrno>

namespace libport
{

  inline Semaphore::Semaphore (int cnt)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    std::stringstream s;
    s << "sema/" << getpid () << "/" << counter++;
    sem_ = sem_open (s.str ().c_str (), O_CREAT | O_EXCL, 0777, cnt);
    assert (!IS_SEM_FAILED (sem_));
# else
    sem_ = new sem_t;
    int err = sem_init(sem_, 0, cnt);
#  if not defined WIN32 && not defined LIBPORT_WIN32
    if (err)
    {
      std::string msg = strerror(errno);
      throw libport::exception::Semaphore("Semaphore::Semaphore (int)",
					  msg + " in sem_init.");
    }
#  else
    assert (!err);
#  endif
# endif
  }

  inline Semaphore::~Semaphore ()
  {
# ifdef __APPLE__
    int err = sem_close(sem_);
# else
    int err = sem_destroy(sem_);
    delete sem_;
# endif
    assert (!err);
  }


  inline void
  Semaphore::operator++ (int)
  {
    Semaphore::operator++ ();
  }

  inline void
  Semaphore::operator-- (int)
  {
    Semaphore::operator-- ();
  }

  inline void
  Semaphore::operator++ ()
  {
    int err = sem_post(sem_);
# if not defined WIN32 && not defined LIBPORT_WIN32
    if (err)
    {
      std::string msg = strerror(errno);
      throw libport::exception::Semaphore("Semaphore::operator++ ()",
					  msg + " in sem_post.");
    }
# else
    assert (!err);
# endif
  }

  inline void
  Semaphore::operator-- ()
  {
    int err;
    do
      {
	err = sem_wait(sem_);
      }
    while (err == -1 && errno == EINTR);
# if not defined WIN32 && not defined LIBPORT_WIN32
    if (err)
    {
      std::string msg = strerror(errno);
      throw libport::exception::Semaphore("Semaphore::operator-- ()",
					  msg + " in sem_wait.");
    }
# else
    assert (!err);
# endif
  }

  inline
  Semaphore::operator int ()
  {
    int t;
    int err = sem_getvalue(sem_, &t);
    assert (!err);
    return t;
  }

} // namespace libport

#endif // !LIBPORT_SEMAPHORE_HXX
