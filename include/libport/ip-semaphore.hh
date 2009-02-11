#ifndef IP_SEMAPHORE_HH
# define IP_SEMAPHORE_HH

# include <sys/types.h>
# include <unistd.h>

# include <libport/config.h>

# ifdef LIBPORT_HAVE_SEMGET
# define LIBPORT_HAVE_IP_SEMAPHORE

namespace libport
{
  // An inter-process semaphore
  // FIXME: works only under Unix
  //        (actually, only when semget/semctl/semop are available)
  class IPSemaphore
  {
  public:
    IPSemaphore(int count = 0);
    ~IPSemaphore();
    void operator++();
    void operator--();

    class Lock
    {
    public:
      Lock(IPSemaphore& sem);
      ~Lock();
    private:
      IPSemaphore& sem_;
    };

  private:
    int id_;
    pid_t owner_pid_;
  };
}

# endif
#endif
