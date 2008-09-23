#ifndef NETWORK_BSDNET_NETWORK_HH
# define NETWORK_BSDNET_NETWORK_HH

# ifdef KERNEL_USERVER_HH
#   error network/bsdnet/network.hh must be loaded before kernel/userver.hh.
# endif

# include <libport/export.hh>
# include <libport/network.h>
# include <libport/utime.hh>

namespace Network
{
  class LIBPORT_API Pipe
  {
  public:
    Pipe();
    virtual ~Pipe();
    //returns read fd or -1 if none
    virtual int readFD() = 0;
    virtual int writeFD() = 0;

    virtual void notifyRead() = 0;
    virtual void notifyWrite() = 0;

    /// trigger demuxer fd set reload
    void trigger();

    virtual std::ostream& dump (std::ostream& o) const = 0;

  public:
    int controlFd;
  };

  std::ostream& operator<< (std::ostream& o, const Pipe& p);

  /// Build the two fd_sets according to registered connections, copy rd to er.
  LIBPORT_API int buildFD(fd_set& rd, fd_set& wr, fd_set &er);

  /// Notify the Pipe object associed with fd sets in the list.
  LIBPORT_API void notify(fd_set& rd, fd_set& wr, fd_set &er);

  LIBPORT_API void registerNetworkPipe(Pipe* p);
  LIBPORT_API void unregisterNetworkPipe(Pipe* p);

  /// Create a tcp server, binding on specified address/port, return local port.
  LIBPORT_API int createTCPServer(int port=0, std::string address = "");

  /// Perform the select with a delay of usedDelay microseconds.
  /// \returns  whether at least one action was performed
  LIBPORT_API bool selectAndProcess(libport::utime_t usDelay);

  /// Create a thread that will loop forever on selectAndProcess.
  LIBPORT_API void startNetworkProcessingThread();
}

#endif // !NETWORK_BSDNET_NETWORK_HH
