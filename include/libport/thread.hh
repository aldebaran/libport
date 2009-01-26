#ifndef LIBPORT_THREAD_HH
# define LIBPORT_THREAD_HH

# include <boost/function.hpp>
# include <boost/optional.hpp>

namespace libport
{

  void*
  startThread(boost::function0<void> func);

  template<class T>
  void*
  startThread(T* obj);

  template<class T>
  void*
  startThread(T* obj, void (T::*func)(void));

  void joinThread(void* t);

  template<typename Res>
  class ThreadedCall
  {
  public:
    ThreadedCall();
    ThreadedCall(boost::function0<Res> f);
    void start(boost::function0<Res> f);
    void wrap(boost::function0<Res> f);

    void clear();
    /// Return true if a job finished.
    bool finished() const;
    bool running() const;
    void wait();
    Res get();

  private:
    boost::optional<Res> res_;
    void* handle_;
  };

} // namespace libport

# include <libport/thread.hxx>

#endif // !LIBPORT_THREAD_HH
