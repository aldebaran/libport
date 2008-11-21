#ifndef NETWORK_BSDNET_NETWORK_HH
# define NETWORK_BSDNET_NETWORK_HH

# ifdef KERNEL_USERVER_HH
#   error network/bsdnet/network.hh must be loaded before kernel/userver.hh.
# endif

# include <libport/config.h>
# include <libport/network.h>
# include <libport/utime.hh>

# include <urbi/export.hh>

namespace Network
{
  class URBI_SDK_API Pipe
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
  URBI_SDK_API int buildFD(fd_set& rd, fd_set& wr, fd_set &er);

  /// Notify the Pipe object associed with fd sets in the list.
  URBI_SDK_API void notify(fd_set& rd, fd_set& wr, fd_set &er);

  URBI_SDK_API void registerNetworkPipe(Pipe* p);
  URBI_SDK_API void unregisterNetworkPipe(Pipe* p);

  /// Create a tcp server, binding on specified address/port, return local port.
  URBI_SDK_API int createTCPServer(int port=0, std::string address = "");

  /// Perform the select with a delay of usedDelay microseconds.
  /// \returns  whether at least one action was performed
  URBI_SDK_API bool selectAndProcess(libport::utime_t usDelay);

# ifdef WITH_BOOST_THREADS
  /// Create a thread that will loop forever on selectAndProcess.
  URBI_SDK_API void startNetworkProcessingThread();
# endif // WITH_BOOST_THREADS
}

#endif // !NETWORK_BSDNET_NETWORK_HH
