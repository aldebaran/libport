/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_DEBUG_HH
# define LIBPORT_DEBUG_HH

# include <libport/detect-win32.h>
# include <libport/compiler.hh>

# ifndef LIBPORT_DEBUG_DISABLE

#  include <libport/csignal>

#  include <list>
#  include <map>
#  include <sstream>

#  include <libport/format.hh>
#  include <boost/function.hpp>
#  include <libport/export.hh>
#  include <libport/finally.hh>
#  include <libport/option-parser.hh>

namespace libport
{
  namespace debug
  {
    typedef std::map<std::string, unsigned long long> categories_type;

    categories_type& get_categories();
    LIBPORT_API int add_category(const std::string& name);
    LIBPORT_API int enable_category(const std::string& name);
    LIBPORT_API int disable_category(const std::string& name);

    /// Reclaim the allocated memory.
    LIBPORT_API void clear();
    bool test_category(const std::string& name);
  }

  class LIBPORT_API Debug
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

    struct levels
    {
      enum Level
      {
	none = 0,
	log = 1,
	trace = 2,
	debug = 3,
	dump = 4,
      };
    };

    Debug();
    virtual ~Debug();
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

    libport::Finally::action_type push_category(const std::string& name);

    libport::Finally::action_type push_level(levels::Level lvl);

    void locations(bool value);
    bool locations();
    void timestamps(bool value);
    bool timestamps();

    ATTRIBUTE_NORETURN static void abort(const std::string& msg);

    void filter(levels::Level lvl);
    void filter(const std::string& lvl);
    levels::Level level() const;
    bool disabled();

  protected:
    std::string category();
    virtual void pop() = 0;

  private:
    void pop_category();
    void pop_level();

    std::list<std::string> categories_stack_;
    std::list<levels::Level> level_stack_;
    bool locations_;
    bool timestamps_;
    levels::Level filter_;
  };

  class LIBPORT_API ConsoleDebug: public Debug
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

#  ifndef WIN32
  class LIBPORT_API SyslogDebug: public Debug
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
#  endif

  LIBPORT_API extern boost::function0<Debug*> make_debugger;
  LIBPORT_API Debug* debugger();

  LIBPORT_API std::string gd_ihexdump(const unsigned char* data, unsigned size);


  class StreamWrapper
  {
  public:
    template <typename T>
    const StreamWrapper& operator <<(const T& e) const
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


  namespace opts
  {

    extern LIBPORT_API libport::OptionValue debug;

  }

}


/*----------.
| Helpers.  |
`----------*/

#  define GD_DEBUGGER libport::debugger()

#  define GD_FORMAT(Msg, ...)                   \
  libport::format(Msg, __VA_ARGS__)

#  define GD_STREAM(Msg)                        \
  (libport::StreamWrapper() << Msg).str()

#  define GD_FUNCTION __FUNCTION__

/*--------.
| Print.  |
`--------*/

// INFO

#  define GD_INFO(Message)                                      \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::info,    \
                    GD_FUNCTION, __FILE__, __LINE__)

#  define GD_FINFO(Msg, ...)                    \
  GD_INFO(GD_FORMAT(Msg, __VA_ARGS__))

#  define GD_SINFO(Msg)                         \
  GD_INFO(GD_STREAM(Msg))

#  define GD_VINFO(Msg, Exp)                    \
  GD_FINFO("%s: %s = %s", Msg, #Exp, Exp)


#  define GD_INFO_LOG(Msg)                      \
  do { GD_LOG(); GD_INFO(Msg); } while (0)

#  define GD_INFO_TRACE(Msg)                    \
  do { GD_TRACE(); GD_INFO(Msg); } while (0)

#  define GD_INFO_DEBUG(Msg)                    \
  do { GD_DEBUG(); GD_INFO(Msg); } while (0)

#  define GD_INFO_DUMP(Msg)                     \
  do { GD_DUMP(); GD_INFO(Msg); } while (0)

#  define GD_FINFO_LOG(Msg, ...)                        \
  do { GD_LOG(); GD_FINFO(Msg, __VA_ARGS__); } while (0)

#  define GD_FINFO_TRACE(Msg, ...)                      \
  do { GD_TRACE(); GD_FINFO(Msg, __VA_ARGS__); } while (0)

#  define GD_FINFO_DEBUG(Msg, ...)                      \
  do { GD_DEBUG(); GD_FINFO(Msg, __VA_ARGS__); } while (0)

#  define GD_FINFO_DUMP(Msg, ...)                       \
  do { GD_DUMP(); GD_FINFO(Msg, __VA_ARGS__); } while (0)

#  define GD_SINFO_LOG(Msg)                        \
  do { GD_LOG(); GD_SINFO(Msg); } while (0)

#  define GD_SINFO_TRACE(Msg)                      \
  do { GD_TRACE(); GD_SINFO(Msg); } while (0)

#  define GD_SINFO_DEBUG(Msg)                      \
  do { GD_DEBUG(); GD_SINFO(Msg); } while (0)

#  define GD_SINFO_DUMP(Msg)                       \
  do { GD_DUMP(); GD_SINFO(Msg); } while (0)

#  define GD_VINFO_LOG(Msg, Val)                        \
  do { GD_LOG(); GD_VINFO(Msg, Val); } while (0)

#  define GD_VINFO_TRACE(Msg, Val)                      \
  do { GD_TRACE(); GD_VINFO(Msg, Val); } while (0)

#  define GD_VINFO_DEBUG(Msg, Val)                      \
  do { GD_DEBUG(); GD_VINFO(Msg, Val); } while (0)

#  define GD_VINFO_DUMP(Msg, Val)                       \
  do { GD_DUMP(); GD_VINFO(Msg, Val); } while (0)

#  define GD_WARN(Message)                                      \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::warn,    \
                    GD_FUNCTION, __FILE__, __LINE__)

#  define GD_FWARN(Msg, ...)                 \
  GD_WARN(GD_FORMAT(Msg, __VA_ARGS__))

#  define GD_SWARN(Msg)                 \
  GD_WARN(GD_STREAM(Msg))

#  define GD_VWARN(Msg, Exp)                    \
  GD_FWARN("%s: %s = %s", Msg, #Exp, Exp)


#  define GD_ERROR(Message)                                     \
  GD_DEBUGGER->debug(Message, ::libport::Debug::types::error,   \
                    GD_FUNCTION, __FILE__, __LINE__)

#  define GD_FERROR(Msg, ...)               \
  GD_ERROR(GD_FORMAT(Msg, __VA_ARGS__))

#  define GD_SERROR(Msg)               \
  GD_ERROR(GD_STREAM(Msg))

#  define GD_VERROR(Msg, Exp)                   \
  GD_FERROR("%s: %s = %s", Msg, #Exp, Exp)


#  define GD_PUSH(Message)                              \
  libport::Finally _gd_pop_                             \
  (GD_DEBUGGER->push(Message,                           \
                     GD_FUNCTION, __FILE__, __LINE__))

#  define GD_FPUSH(Message, ...)                \
  GD_PUSH(GD_FORMAT(Message, __VA_ARGS__))              \
                                                \
/*-------------.
| Categories.  |
`-------------*/

#  define GD_ADD_CATEGORY(Name)                                         \
  static int _gd_category_##Name =                                      \
    ::libport::debug::add_category(#Name)

#  define GD_CATEGORY(Cat)                                              \
  libport::Finally _gd_pop_category_##__LINE__                          \
  (GD_DEBUGGER->push_category(#Cat))

#  define GD_DISABLE_CATEGORY(Cat)                                      \
  static int _gd_category_disable_##Cat =                               \
  ::libport::debug::disable_category(#Cat)

#  define GD_ENABLE_CATEGORY(Cat)                \
  static int _gd_category_enable_##Cat =         \
  ::libport::debug::enable_category(#Cat)

/*--------.
| Level.  |
`--------*/

#  define GD_LEVEL(Lvl)                         \
  libport::Finally _gd_pop_level_##__LINE__     \
  (GD_DEBUGGER->push_level(Lvl))

#  define GD_LOG()						  \
  GD_LEVEL(::libport::Debug::levels::log)

#  define GD_TRACE()						  \
  GD_LEVEL(::libport::Debug::levels::trace)

#  define GD_DEBUG()						  \
  GD_LEVEL(::libport::Debug::levels::debug)

#  define GD_DUMP()						\
  GD_LEVEL(::libport::Debug::levels::dump)

#  define GD_FILTER(Lvl)					\
  GD_DEBUGGER->filter(Lvl)

#  define GD_FILTER_NONE()					   \
  GD_FILTER(::libport::Debug::levels::none)

#  define GD_FILTER_LOG()					   \
  GD_FILTER(::libport::Debug::levels::log)

#  define GD_FILTER_TRACE()					   \
  GD_FILTER(::libport::Debug::levels::trace)

#  define GD_FILTER_DEBUG()					   \
  GD_FILTER(::libport::Debug::levels::debug)

#  define GD_FILTER_DUMP()					\
  GD_FILTER(::libport::Debug::levels::dump)

#  define GD_CURRENT_LEVEL()					\
  GD_DEBUGGER->level()						\

#  define GD_FILTER_INC()                                               \
  do {                                                                  \
    if (GD_CURRENT_LEVEL() != ::libport::Debug::levels::dump)           \
      GD_DEBUGGER->filter(                                              \
        (::libport::Debug::levels::Level)(GD_CURRENT_LEVEL() + 1));     \
  } while (0)

#  define GD_FILTER_DEC()                                               \
  do {                                                                  \
  if (GD_CURRENT_LEVEL() != ::libport::Debug::levels::none)             \
    GD_DEBUGGER->filter(                                                \
      (::libport::Debug::levels::Level)(GD_CURRENT_LEVEL() - 1));       \
  } while (0)

#  define GD_SHOW_LEVEL(Lvl)				\
  (GD_CURRENT_LEVEL() >= Lvl)

#  define GD_SHOW_LOG()                         \
  GD_SHOW_LEVEL(::libport::Debug::levels::log)

#  define GD_SHOW_TRACE()                               \
  GD_SHOW_LEVEL(::libport::Debug::levels::trace)

#  define GD_SHOW_DEBUG()                               \
  GD_SHOW_LEVEL(::libport::Debug::levels::debug)

#  define GD_SHOW_DUMP()                        \
  GD_SHOW_LEVEL(::libport::Debug::levels::dump)

/*-------------.
| Assertions.  |
`-------------*/

#  define GD_ABORT(Msg)                         \
  libport::Debug::abort(Msg)

#  define GD_FABORT(Msg, ...)                   \
  GD_ABORT(GD_FORMAT(Msg, __VA_ARGS__))

#  define GD_UNREACHABLE()                      \
  GD_ABORT("Unreachable code reached")

#  define GD_IHEXDUMP(Data, Size)                       \
  libport::gd_ihexdump                                  \
  (reinterpret_cast<const unsigned char*>(Data), Size)


/*------------.
| Configure.  |
`------------*/

#  define GD__INIT_                                                     \
  static int                                                            \
  _libport_initdebug_()                                                 \
  {                                                                     \
    make_debugger = _libport_mkdebug_;                                  \
    return 42;                                                          \
  }                                                                     \
                                                                        \
  static int _libport_debug_initialized_ = _libport_initdebug_();

// Must be called before any use.
#  define GD_INIT()                             \
  GD_INIT_CONSOLE()

// Should be called before quitting to reclaim memory.
#  define GD_QUIT()                             \
  ::libport::debug::clear()

#  define GD_INIT_CONSOLE()                                             \
  namespace libport                                                     \
  {                                                                     \
    static Debug* _libport_mkdebug_()                                   \
    {                                                                   \
      return new ConsoleDebug();                                        \
    }                                                                   \
    GD__INIT_;                                                          \
  }

#  define GD_INIT_SYSLOG(Program)                                       \
  namespace libport                                                     \
  {                                                                     \
    static Debug* _libport_mkdebug_()                                   \
    {                                                                   \
      return new SyslogDebug(#Program);                                 \
    }                                                                   \
    GD__INIT_;                                                          \
  }

#  define GD_ENABLE(Name)                       \
  GD_DEBUGGER->Name(true)

#  define GD_ENABLE_LOCATIONS()                 \
  GD_ENABLE(locations)

#  define GD_ENABLE_TIMESTAMPS()                \
  GD_ENABLE(timestamps)

# else

#  define GD_DEBUGGER
#  define GD_FORMAT_ELEM(R, Data, Elem)
#  define GD_FORMAT(Msg, ...)
#  define GD_INFO(Message)
#  define GD_FINFO(Msg, ...)
#  define GD_VINFO(Msg, Exp)
#  define GD_INFO_LOG(Msg)
#  define GD_INFO_TRACE(Msg)
#  define GD_INFO_DEBUG(Msg)
#  define GD_INFO_DUMP(Msg)
#  define GD_FINFO_LOG(Msg, ...)
#  define GD_FINFO_TRACE(Msg, ...)
#  define GD_FINFO_DEBUG(Msg, ...)
#  define GD_FINFO_DUMP(Msg, ...)
#  define GD_SINFO_LOG(Msg, ...)
#  define GD_SINFO_TRACE(Msg, ...)
#  define GD_SINFO_DEBUG(Msg, ...)
#  define GD_SINFO_DUMP(Msg, ...)
#  define GD_VINFO_LOG(Msg, Val)
#  define GD_VINFO_TRACE(Msg, Val)
#  define GD_VINFO_DEBUG(Msg, Val)
#  define GD_VINFO_DUMP(Msg, Val)
#  define GD_WARN(Message)
#  define GD_FWARN(Msg, ...)
#  define GD_VWARN(Msg, Exp)
#  define GD_ERROR(Message)
#  define GD_FERROR(Msg, ...)
#  define GD_VERROR(Msg, Exp)
#  define GD_PUSH(Message)
#  define GD_FPUSH(Message, ...)
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
#  define GD_CURRENT_LEVEL()
#  define GD_FILTER_INC()
#  define GD_FILTER_DEC()
#  define GD_SHOW_LEVEL(Lvl)
#  define GD_SHOW_LOG()
#  define GD_SHOW_TRACE()
#  define GD_SHOW_DEBUG()
#  define GD_SHOW_DUMP()
#  define GD_ABORT(Msg)
#  define GD_FABORT(Msg, ...)
#  define GD_UNREACHABLE()
#  define GD_IHEXDUMP(Data, Size)
#  define GD_INIT()
#  define GD_QUIT()
#  define GD_INIT_CONSOLE()
#  define GD_ADD_CATEGORY(Name)
#  define GD_ENABLE(Name)
#  define GD_ENABLE_LOCATIONS()
#  define GD_ENABLE_TIMESTAMPS()

# endif

#endif
