#ifndef LIBPORT_DEBUG_HH
# define LIBPORT_DEBUG_HH

# ifndef NDEBUG

#  include <signal.h>

#  include <list>
#  include <map>
#  include <sstream>

#  include <boost/format.hpp>
#  include <boost/function.hpp>
#  include <boost/preprocessor/seq/for_each.hpp>
#  include <boost/thread.hpp>

#  include <libport/finally.hh>

namespace libport
{
  class Debug
  {
  public:

    struct types
    {
      enum Type
      {
        info,
        warn,
        error,
      };
    };

    struct colors
    {
      enum Color
      {
        white = 0,
        red = 31,
        green = 32,
        blue = 36,
        yellow = 33,
        purple = 34,
      };
    };

    Debug();
    void debug(const std::string& msg,
               types::Type type,
               const std::string& fun = "",
               const std::string& file = "",
               unsigned line = 0);
    libport::Finally::action_type push(const std::string& msg,
                                       const std::string& fun = "",
                                       const std::string& file = "",
                                       unsigned line = 0);

    virtual void message(const std::string& msg,
                         types::Type type,
                         const std::string& fun = "",
                         const std::string& file = "",
                         unsigned line = 0) = 0;
    virtual void message_push(const std::string& msg,
                              const std::string& fun = "",
                              const std::string& file = "",
                              unsigned line = 0) = 0;

    int  add_category(const std::string& name);
    libport::Finally::action_type push_category(const std::string& name);
    void disable_category(const std::string& name);
    void enable_category(const std::string& name);

    libport::Finally::action_type push_level(unsigned lvl);

    void locations(bool value);
    bool locations();
    void timestamps(bool value);
    bool timestamps();

    static void abort(const std::string& msg) __attribute__ ((noreturn));

    void filter(unsigned lvl);
    bool disabled();

  protected:
    std::string category();
    virtual void pop() = 0;

  private:
    void pop_category();
    void pop_level();
    void check_category(const std::string& category);
    std::map<std::string, unsigned long long> categories_;
    std::list<std::string> categories_stack_;
    std::list<unsigned> level_stack_;
    bool locations_;
    bool timestamps_;
    unsigned filter_;
  };

  class ConsoleDebug: public Debug
  {
  public:
    ConsoleDebug();
    virtual void message(const std::string& msg,
                         types::Type type,
                         const std::string& fun = "",
                         const std::string& file = "",
                         unsigned line = 0);
    virtual void message_push(const std::string& msg,
                              const std::string& fun = "",
                              const std::string& file = "",
                              unsigned line = 0);
    virtual void pop();

  private:
    void color(int color, bool bold = true);
    void reset();
    unsigned indent_;
  };

  class SyslogDebug: public Debug
  {
  public:
    SyslogDebug(const std::string& program);
    ~SyslogDebug();
    virtual void message(const std::string& msg,
                         types::Type type,
                         const std::string& fun,
                         const std::string& file,
                         unsigned line);
    virtual void message_push(const std::string& msg,
                              const std::string& fun,
                              const std::string& file,
                              unsigned line);
    virtual void pop();
  private:
    unsigned indent_;
  };

  extern boost::function0<Debug*> make_debugger;
  Debug* debugger();

  std::string gd_ihexdump(const unsigned char* data, unsigned size);


  class StreamWrapper
  {
  public:
    template <typename T>
    const StreamWrapper& operator <<(T& e) const
    {
      stream_ << e;
      return *this;
    }

    std::string str() const
    {
      return stream_.str();
    }
  private:
    mutable std::stringstream stream_;
  };

}



/*--------.
| Helpers |
`--------*/

#  define GD_DEBUGGER libport::debugger()

#  define GD_FORMAT_ELEM(R, Data, Elem) % Elem  \

#  define GD_FORMAT(Msg, Seq)                           \
  str(boost::format(Msg)                                \
      BOOST_PP_SEQ_FOR_EACH(GD_FORMAT_ELEM, , Seq))     \

#  define GD_STREAM(Msg)                                                \
  (libport::StreamWrapper() << Msg).str()                                \

#  define GD_FUNCTION __FUNCTION__

/*------.
| Print |
`------*/

// INFO

#  define GD_INFO(Message)                                            \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::info,           \
                    GD_FUNCTION, __FILE__, __LINE__);        \

#  define GD_FINFO(Msg, Seq)                 \
  GD_INFO(GD_FORMAT(Msg, Seq));              \

#  define GD_SINFO(Msg)                      \
  GD_INFO(GD_STREAM(Msg));                   \

#  define GD_VINFO(Msg, Exp)                     \
  GD_FINFO("%s: %s = %s", (Msg) (#Exp) (Exp))    \


#  define GD_INFO_LOG(Msg)                        \
  { GD_LOG(); GD_INFO(Msg)}                       \

#  define GD_INFO_TRACE(Msg)                    \
  { GD_TRACE(); GD_INFO(Msg)}                   \

#  define GD_INFO_DEBUG(Msg)                    \
  { GD_DEBUG(); GD_INFO(Msg)}                   \

#  define GD_INFO_DUMP(Msg)                       \
  { GD_DUMP(); GD_INFO(Msg)}                      \

#  define GD_FINFO_LOG(Msg, Seq)                 \
  { GD_LOG(); GD_FINFO(Msg, Seq)}                \

#  define GD_FINFO_TRACE(Msg, Seq)               \
  { GD_TRACE(); GD_FINFO(Msg, Seq)}              \

#  define GD_FINFO_DEBUG(Msg, Seq)               \
  { GD_DEBUG(); GD_FINFO(Msg, Seq)}              \

#  define GD_FINFO_DUMP(Msg, Seq)                \
  { GD_DUMP(); GD_FINFO(Msg, Seq)}               \

#  define GD_SINFO_LOG(Msg, Seq)                 \
  { GD_LOG(); GD_SINFO(Msg, Seq)}                \

#  define GD_SINFO_TRACE(Msg, Seq)               \
  { GD_TRACE(); GD_SINFO(Msg, Seq)}              \

#  define GD_SINFO_DEBUG(Msg, Seq)               \
  { GD_DEBUG(); GD_SINFO(Msg, Seq)}              \

#  define GD_SINFO_DUMP(Msg, Seq)                \
  { GD_DUMP(); GD_SINFO(Msg, Seq)}               \

#  define GD_VINFO_LOG(Msg, Val)                 \
  { GD_LOG(); GD_VINFO(Msg, Val)}                \

#  define GD_VINFO_TRACE(Msg, Val)               \
  { GD_TRACE(); GD_VINFO(Msg, Val)}              \

#  define GD_VINFO_DEBUG(Msg, Val)               \
  { GD_DEBUG(); GD_VINFO(Msg, Val)}              \

#  define GD_VINFO_DUMP(Msg, Val)                \
  { GD_DUMP(); GD_VINFO(Msg, Val)}               \


#  define GD_WARN(Message)                                      \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::warn,      \
                    GD_FUNCTION, __FILE__, __LINE__);           \

#  define GD_FWARN(Msg, Seq)                 \
  GD_WARN(GD_FORMAT(Msg, Seq));              \

#  define GD_SWARN(Msg)                 \
  GD_WARN(GD_STREAM(Msg));              \

#  define GD_VWARN(Msg, Exp)                    \
  GD_FWARN("%s: %s = %s", (Msg) (#Exp) (Exp))   \


#  define GD_ERROR(Message)                                             \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::error,             \
                    GD_FUNCTION, __FILE__, __LINE__);                   \

#  define GD_FERROR(Msg, Seq)               \
  GD_ERROR(GD_FORMAT(Msg, Seq));            \

#  define GD_SERROR(Msg)               \
  GD_ERROR(GD_STREAM(Msg));            \

#  define GD_VERROR(Msg, Exp)                    \
  GD_FERROR("%s: %s = %s", (Msg) (#Exp) (Exp))   \


#  define GD_PUSH(Message)                                             \
  libport::Finally _gd_pop_                                            \
  (GD_DEBUGGER->push(Message,                                           \
                    GD_FUNCTION, __FILE__, __LINE__));                 \

#  define GD_FPUSH(Message, Seq)                \
  GD_PUSH(GD_FORMAT(Message, Seq))              \
                                                \
/*-----------.
| Categories |
`-----------*/

#  define GD_CATEGORY(Cat)                                              \
  libport::Finally _gd_pop_category_##Cat                               \
  (GD_DEBUGGER->push_category(#Cat));                                    \

#  define GD_DISABLE_CATEGORY(Cat)                                      \
  GD_DEBUGGER->disable_category(#Cat);                                   \

#  define GD_ENABLE_CATEGORY(Cat)               \
  GD_DEBUGGER->enable_category(#Cat);            \
                                                \
/*------.
| Level |
`------*/

#  define GD_LEVEL(Lvl)                                                 \
  libport::Finally _gd_pop_level_##Lvl                                  \
  (GD_DEBUGGER->push_level(Lvl));                                        \

#  define GD_LOG()                              \
  GD_LEVEL(1);                                  \

#  define GD_TRACE()                            \
  GD_LEVEL(2);                                  \

#  define GD_DEBUG()                            \
  GD_LEVEL(3);                                  \

#  define GD_DUMP()                             \
  GD_LEVEL(4);                                  \

#  define GD_FILTER(Lvl)                                                \
  GD_DEBUGGER->filter(Lvl);                                              \

#  define GD_FILTER_NONE()                       \
  GD_FILTER(0);                                  \

#  define GD_FILTER_LOG()                        \
  GD_FILTER(1);                                  \

#  define GD_FILTER_TRACE()                      \
  GD_FILTER(2);                                  \

#  define GD_FILTER_DEBUG()                      \
  GD_FILTER(3);                                  \

#  define GD_FILTER_DUMP()                      \
  GD_FILTER(4);                                 \
                                                \
/*-----------.
| Assertions |
`-----------*/

#  define GD_ABORT(Msg)                            \
  libport::Debug::abort(Msg);                       \

#  define GD_FABORT(Msg, Seq)                   \
  GD_ABORT(GD_FORMAT(Msg, Seq))                 \

#  define GD_UNREACHABLE()                      \
  GD_ABORT("Unreachable code reached")          \

#  define GD_IHEXDUMP(Data, Size)                                       \
  libport::gd_ihexdump                                                   \
  (reinterpret_cast<const unsigned char*>(Data), Size)                  \

/*----------.
| Configure |
`----------*/

#  define GD_INIT()                               \
  GD_INIT_CONSOLE()                               \

#  define GD_INIT_CONSOLE()                                     \
  namespace libport                                             \
  {                                                             \
    static Debug* _libport_mkdebug_()                           \
    { return new ConsoleDebug(); }                              \
    boost::function0<Debug*> make_debugger(_libport_mkdebug_);  \
  }                                                             \

#  define GD_INIT_SYSLOG(Program)                                       \
  namespace libport                                                     \
  {                                                                     \
    static Debug* _libport_mkdebug_()                                   \
    { return new SyslogDebug(#Program); }                               \
    boost::function0<Debug*> make_debugger(_libport_mkdebug_);          \
  }                                                                     \

#  define GD_ADD_CATEGORY(Name)                                         \
  static int _gd_category_##Name = GD_DEBUGGER->add_category(#Name);    \

#  define GD_ENABLE(Name)                         \
  GD_DEBUGGER->Name(true);                         \

#  define GD_ENABLE_LOCATIONS()                   \
  GD_ENABLE(locations)                            \

#  define GD_ENABLE_TIMESTAMPS()                  \
  GD_ENABLE(timestamps)                           \

# else

#  define GD_DEBUGGER
#  define GD_FORMAT_ELEM(R, Data, Elem)
#  define GD_FORMAT(Msg, Seq)
#  define GD_INFO(Message)
#  define GD_FINFO(Msg, Seq)
#  define GD_VINFO(Msg, Exp)
#  define GD_INFO_LOG(Msg)
#  define GD_INFO_TRACE(Msg)
#  define GD_INFO_DEBUG(Msg)
#  define GD_INFO_DUMP(Msg)
#  define GD_FINFO_LOG(Msg, Seq)
#  define GD_FINFO_TRACE(Msg, Seq)
#  define GD_FINFO_DEBUG(Msg, Seq)
#  define GD_FINFO_DUMP(Msg, Seq)
#  define GD_SINFO_LOG(Msg, Seq)
#  define GD_SINFO_TRACE(Msg, Seq)
#  define GD_SINFO_DEBUG(Msg, Seq)
#  define GD_SINFO_DUMP(Msg, Seq)
#  define GD_VINFO_LOG(Msg, Val)
#  define GD_VINFO_TRACE(Msg, Val)
#  define GD_VINFO_DEBUG(Msg, Val)
#  define GD_VINFO_DUMP(Msg, Val)
#  define GD_WARN(Message)
#  define GD_FWARN(Msg, Seq)
#  define GD_VWARN(Msg, Exp)
#  define GD_ERROR(Message)
#  define GD_FERROR(Msg, Seq)
#  define GD_VERROR(Msg, Exp)
#  define GD_PUSH(Message)
#  define GD_FPUSH(Message, Seq)
#  define GD_CATEGORY(Cat)
#  define GD_DISABLE_CATEGORY(Cat)
#  define GD_ENABLE_CATEGORY(Cat)
#  define GD_LEVEL(Lvl)
#  define GD_LOG()
#  define GD_TRACE()
#  define GD_DEBUG()
#  define GD_DUMP()
#  define GD_FILTER(Lvl)
#  define GD_FILTER_NONE()
#  define GD_FILTER_LOG()
#  define GD_FILTER_TRACE()
#  define GD_FILTER_DEBUG()
#  define GD_FILTER_DUMP()
#  define GD_ABORT(Msg)
#  define GD_FABORT(Msg, Seq)
#  define GD_UNREACHABLE()
#  define GD_IHEXDUMP(Data, Size)
#  define GD_INIT()
#  define GD_INIT_CONSOLE()
#  define GD_ADD_CATEGORY(Name)
#  define GD_ENABLE(Name)
#  define GD_ENABLE_LOCATIONS()
#  define GD_ENABLE_TIMESTAMPS()

# endif

#endif
