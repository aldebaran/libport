#ifndef NETWORK_BSDNET_NETWORK_HH
# define NETWORK_BSDNET_NETWORK_HH

# ifdef KERNEL_USERVER_HH
#   error network/bsdnet/network.hh must be loaded before kernel/userver.hh.
# endif

# include "libport/network.h"

namespace Network
{
  class Pipe
  {
  public:
    Pipe();
    virtual ~Pipe() {}
    //returns read fd or -1 if none
    virtual int readFD() = 0;
    virtual int writeFD() = 0;

    virtual void notifyRead() = 0;
    virtual void notifyWrite() = 0;
    void trigger(); ///< trigger demuxer fd set reload

    virtual std::ostream& print (std::ostream& o) const = 0;

  public:
    int controlFd;
  };

  std::ostream& operator<< (std::ostream& o, const Pipe& p);

  /// Build the two fd_sets according to registered connections, copy rd to er.
  int buildFD(fd_set& rd, fd_set& wr, fd_set &er);

  /// Notify the Pipe object associed with fd sets in the list.
  void notify(fd_set& rd, fd_set& wr, fd_set &er);

  void registerNetworkPipe(Pipe* p);
  void unregisterNetworkPipe(Pipe* p);

  /// Create a tcp server, binding on specified address/port, return local port.
  int createTCPServer(int port=0, const char *address=0);

  /// Perform the select with a delay of usedDelay microseconds.
  /// \returns  whether at least one action was performed
  bool selectAndProcess(int usDelay);

  /// Create a thread that will loop forever on selectAndProcess.
  void startNetworkProcessingThread();
}

#endif // !NETWORK_BSDNET_NETWORK_HH
