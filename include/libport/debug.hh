/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_DEBUG_HH
# define LIBPORT_DEBUG_HH

# include <list>
# include <sstream>

# include <boost/function.hpp>
# include <boost/unordered_map.hpp>

# include <libport/compiler.hh>
# include <libport/csignal>
# include <libport/detect-win32.h>
# include <libport/export.hh>
# include <libport/finally.hh>
# include <libport/format.hh>
# include <libport/local-data.hh>
# include <libport/option-parser.hh>
# include <libport/symbol.hh>
# include <libport/thread-data.hh>

namespace libport
{
  namespace debug
  {
    typedef Symbol category_type;
    typedef boost::unordered_map<category_type, bool> categories_type;
    typedef std::pair<bool, unsigned> pattern_infos_type;
    typedef boost::unordered_map<category_type, pattern_infos_type>
      patterns_type;

    /// Report \a msg, completely bypassing Libport.Debug.
    LIBPORT_API void uninitialized_msg(const std::string& msg);

    categories_type& categories();
    LIBPORT_API category_type add_category(category_type name);
    LIBPORT_API int enable_category(category_type name, bool enabled = true);
    LIBPORT_API int disable_category(category_type name);
    LIBPORT_API int auto_category(category_type pattern);

    /// Reclaim the allocated memory.
    LIBPORT_API void clear();
    bool test_category(category_type name);
  }

  struct LIBPORT_API local_data
  {
    local_data();
    unsigned indent;
  };
  LIBPORT_API extern boost::function0<local_data&> debugger_data;
  LIBPORT_API local_data& debugger_data_thread_local();

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

    // The current indentation level
    unsigned indentation() const;

    // Report the message if it is enabled.
    // Report the message if its \a lvl suffices.
    void debug(const std::string& msg,
               types::Type type,
               debug::category_type category,
               const std::string& fun = "",
               const std::string& file = "",
               unsigned line = 0);

    Debug* push(debug::category_type category,
                const std::string& msg,
                const std::string& fun = "",
                const std::string& file = "",
                unsigned line = 0);

    // Report the message.
    // This is the backend.
    ATTRIBUTE_COLD
    virtual void message(debug::category_type category,
                         const std::string& msg,
                         types::Type type,
                         const std::string& fun = "",
                         const std::string& file = "",
                         unsigned line = 0) = 0;
    ATTRIBUTE_COLD
    virtual void message_push(debug::category_type category,
                              const std::string& msg,
                              const std::string& fun = "",
                              const std::string& file = "",
                              unsigned line = 0) = 0;

    void locations(bool value);
    bool locations() const;
    void timestamps(bool value);
    bool timestamps() const;

  /*-------------.
  | Filter level |
  `-------------*/

  public:
    /// Set the current filter level.
    static void filter(levels::Level lvl);
    /// Set the current filter level.
    static void filter(const std::string& lvl);
    /// Get the current filter level.
    static levels::Level level();
  private:
    static levels::Level filter_;

  public:
    /// Whether a message with level \a lvl message should be
    /// displayed.
    static bool enabled(levels::Level lvl, debug::category_type category);

  protected:
    std::string category_format(debug::category_type cat) const;
    ATTRIBUTE_COLD
    virtual void pop() = 0;

  private:
    bool locations_;
    bool timestamps_;

  public:
    class Indent
    {
    public:
      ATTRIBUTE_ALWAYS_INLINE
      Indent(Debug* debug, bool active)
        : _debug(debug)
        , _active(active)
      {}

      ATTRIBUTE_ALWAYS_INLINE
      ~Indent()
      {
        if (_active)
          _debug->pop();
      }

    private:
      Debug* _debug;
      bool _active;
    };
  };
} // namespace libport



# if defined LIBPORT_DEBUG_DISABLE


/*=========================\
| Libport.Debug disabled.  |
\=========================*/

#  define GD_CATEGORY(Cat)                                   LIBPORT_EMPTY
#  define GD_CHECK_CATEGORY(Cat)                             LIBPORT_NOP
#  define GD_CURRENT_LEVEL()                                 LIBPORT_NOP
#  define GD_DEBUGGER                                        LIBPORT_NOP
#  define GD_DISABLE_CATEGORY(Cat)                           LIBPORT_NOP
#  define GD_ENABLE(Name)                                    LIBPORT_NOP
#  define GD_ENABLE_CATEGORY(Cat)                            LIBPORT_NOP
#  define GD_FILTER(Lvl)                                     LIBPORT_NOP
#  define GD_FILTER_DEC()                                    LIBPORT_NOP
#  define GD_FILTER_INC()                                    LIBPORT_NOP
#  define GD_FPUSH_(...)                                     LIBPORT_NOP
#  define GD_PUSH_(...)                                      LIBPORT_NOP
#  define GD_IHEXDUMP(Data, Size)                            LIBPORT_NOP
#  define GD_INDENTATION()                                   LIBPORT_NOP
#  define GD_INDENTATION_INC()                               LIBPORT_NOP
#  define GD_INDENTATION_DEC()                               LIBPORT_NOP
#  define GD_INIT_DEBUG_PER_(DebugData, DebugInstantiation)  LIBPORT_EMPTY
#  define GD_LEVEL(Lvl)                                      LIBPORT_NOP
#  define GD_MESSAGE_(...)                                   LIBPORT_NOP
#  define GD_QUIT()                                          LIBPORT_NOP
#  define GD_SHOW_LEVEL(Lvl)                                 LIBPORT_NOP


# else // ! LIBPORT_DEBUG_DISABLE


/*========================\
| Libport.Debug enabled.  |
\========================*/

namespace libport
{
  LIBPORT_API extern Debug* debugger;

  class LIBPORT_API ConsoleDebug: public Debug
  {
  public:
    ConsoleDebug();
    ATTRIBUTE_COLD
    virtual void message(debug::category_type category,
                         const std::string& msg,
                         types::Type type,
                         const std::string& fun = "",
                         const std::string& file = "",
                         unsigned line = 0);
    ATTRIBUTE_COLD
    virtual void message_push(debug::category_type category,
                              const std::string& msg,
                              const std::string& fun = "",
                              const std::string& file = "",
                              unsigned line = 0);
    ATTRIBUTE_COLD
    virtual void pop();
  };

#  ifndef WIN32
  class LIBPORT_API SyslogDebug: public Debug
  {
  public:
    SyslogDebug(const std::string& program);
    ~SyslogDebug();
    ATTRIBUTE_COLD
    virtual void message(debug::category_type category,
                         const std::string& msg,
                         types::Type type,
                         const std::string& fun,
                         const std::string& file,
                         unsigned line);
    ATTRIBUTE_COLD
    virtual void message_push(debug::category_type category,
                              const std::string& msg,
                              const std::string& fun,
                              const std::string& file,
                              unsigned line);
    ATTRIBUTE_COLD
    virtual void pop();
  };
#  endif

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

#  define GD_DEBUGGER ::libport::debugger

#  define GD_FORMAT(Msg, ...)                   \
  libport::format(Msg, __VA_ARGS__)

#  define GD_STREAM(Msg)                        \
  (libport::StreamWrapper() << Msg).str()

#  define GD_FUNCTION __FUNCTION__

#  define GD_ENABLED(Level)                                             \
  (GD_DEBUGGER->enabled(::libport::Debug::levels::Level,                \
                        GD_CATEGORY_GET()))                             \


/*---------.
| Indent.  |
`---------*/

#  define GD_INDENTATION()                      \
  GD_DEBUGGER->indentation()

#  define GD_INDENTATION_INC()                  \
  ++libport::debugger_data().indent

#  define GD_INDENTATION_DEC()                  \
  --libport::debugger_data().indent


/*--------.
| Print.  |
`--------*/

#  define GD_MESSAGE_(Type, Level, Message)                             \
  do                                                                    \
  {                                                                     \
    if (GD_DEBUGGER)                                                    \
    {                                                                   \
      if (GD_ENABLED(Level))                                            \
        GD_DEBUGGER->debug(Message,                                     \
                           ::libport::Debug::types::Type,               \
                           GD_CATEGORY_GET(),                           \
                           GD_FUNCTION, __FILE__, __LINE__);            \
    }                                                                   \
    else                                                                \
      ::libport::debug::uninitialized_msg(Message);                     \
  }                                                                     \
  while (0)                                                             \


// PUSH

#  define GD_PUSH_(Message, Level)                                      \
  libport::Debug::Indent BOOST_PP_CAT(_gd_indent_, __LINE__)            \
    (GD_DEBUGGER, GD_ENABLED(Level));                                   \
  if (GD_DEBUGGER)                                                      \
  {                                                                     \
  if (GD_ENABLED(Level))                                                \
    GD_DEBUGGER->push(GD_CATEGORY_GET(), Message,                       \
                      GD_FUNCTION, __FILE__, __LINE__);                 \
  }                                                                     \
  else                                                                  \
    ::libport::debug::uninitialized_msg(Message);                       \


/*-------------.
| Categories.  |
`-------------*/

#  define GD_CATEGORY_GET() _libport_gd_category

#  define GD_CATEGORY(Name)                                             \
  static ::libport::debug::category_type GD_CATEGORY_GET() =            \
    ::libport::debug::add_category(::libport::debug::category_type(#Name))

// #  define GD_GET_CATEGORY(Name)
//   _gd_category_##Name

#  define GD_DISABLE_CATEGORY(Cat)                                      \
  static int _gd_category_disable_##Cat =                               \
    ::libport::debug::disable_category(GD_GET_CATEGORY(Cat))

#  define GD_ENABLE_CATEGORY(Cat)                \
  static int _gd_category_enable_##Cat =         \
    ::libport::debug::enable_category(GD_GET_CATEGORY(Cat))

// #  define GD_CHECK_CATEGORY(Cat)
//   GD_DEBUGGER->test_category(GD_GET_CATEGORY(Cat))

/*--------.
| Level.  |
`--------*/

#  define GD_FILTER(Lvl)                        \
  GD_DEBUGGER->filter(Lvl)

#  define GD_CURRENT_LEVEL()                    \
  GD_DEBUGGER->level()

#  define GD_FILTER_INC()                                               \
  do {                                                                  \
    if (GD_CURRENT_LEVEL() != ::libport::Debug::levels::dump)           \
      GD_DEBUGGER->filter(                                              \
        (::libport::Debug::levels::Level)(GD_CURRENT_LEVEL() + 1));     \
  } while (0)                                                           \

#  define GD_FILTER_DEC()                                               \
  do {                                                                  \
    if (GD_CURRENT_LEVEL() != ::libport::Debug::levels::none)           \
      GD_DEBUGGER->filter(                                              \
        (::libport::Debug::levels::Level)(GD_CURRENT_LEVEL() - 1));     \
  } while (0)                                                           \

#  define GD_SHOW_LEVEL(Lvl)                    \
  (GD_CURRENT_LEVEL() >= Lvl)

#  define GD_IHEXDUMP(Data, Size)                                       \
  libport::gd_ihexdump(reinterpret_cast<const unsigned char*>(Data), Size)


/*------------.
| Configure.  |
`------------*/

#  define GD_DEFAULT_DEBUG_DATA &::libport::debugger_data_thread_local

// Should be called before quitting to reclaim memory.
#  define GD_QUIT()                             \
  ::libport::debug::clear()

#  define GD_INIT_DEBUG_PER_(DebugData, DebugInstantiation)             \
  struct DebugInitialize                                                \
  {                                                                     \
    DebugInitialize()                                                   \
    {                                                                   \
      GD_CATEGORY(Libport.Debug);                                       \
      if (::libport::debugger)                                          \
        GD_ERROR("GD was already initialized");                         \
      else                                                              \
      {                                                                 \
        ::libport::debugger = new DebugInstantiation;                   \
        ::libport::debugger_data = DebugData;                           \
        GD_INFO_TRACE("Initialize GD with backend "                     \
                      #DebugInstantiation ", storage " #DebugData       \
                      ".");                                             \
      }                                                                 \
    }                                                                   \
  };                                                                    \
  static DebugInitialize _libport_debug_initialized_;

#  define GD_ENABLE(Name)                       \
  GD_DEBUGGER->Name(true)

#  include <libport/debug.hxx>

# endif //!LIBPORT_DEBUG_DISABLE



/*=============================================\
| Bouncing macros, do not need to be undefined |
\=============================================*/

/*-------------.
| Assertions.  |
`-------------*/

# define GD_ABORT(Msg)                          \
  do                                            \
  {                                             \
    GD_ERROR(Msg);                              \
    libport::abort();                           \
  }                                             \
  while (0)                                     \


/*-------------------------.
| Backward compatibility.  |
`-------------------------*/

# define GD_ADD_CATEGORY(Category) /* Ignore. */

/*-------.
| Info.  |
`-------*/

# define GD_INFO_(Level, Message)               \
  GD_MESSAGE_(info, Level, Message)

# define GD_INFO(Message)                       \
  GD_INFO_(log, Message)

# define GD_FINFO(Msg, ...)                     \
  GD_INFO(GD_FORMAT(Msg, __VA_ARGS__))

# define GD_SINFO(Msg)                          \
  GD_INFO(GD_STREAM(Msg))

# define GD_VINFO(Msg, Exp)                     \
  GD_FINFO("%s: %s = %s", Msg, #Exp, Exp)


# define GD_INFO_LOG(Msg)   GD_INFO_(log, Msg)
# define GD_INFO_TRACE(Msg) GD_INFO_(trace, Msg)
# define GD_INFO_DEBUG(Msg) GD_INFO_(debug, Msg)
# define GD_INFO_DUMP(Msg)  GD_INFO_(dump, Msg)

# define GD_FINFO_LOG(Msg, ...)   GD_INFO_(log,   GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FINFO_TRACE(Msg, ...) GD_INFO_(trace, GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FINFO_DEBUG(Msg, ...) GD_INFO_(debug, GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FINFO_DUMP(Msg, ...)  GD_INFO_(dump,  GD_FORMAT(Msg, __VA_ARGS__))

# define GD_SINFO_LOG(Msg)   GD_INFO_(log,   GD_STREAM(Msg))
# define GD_SINFO_TRACE(Msg) GD_INFO_(trace, GD_STREAM(Msg))
# define GD_SINFO_DEBUG(Msg) GD_INFO_(debug, GD_STREAM(Msg))
# define GD_SINFO_DUMP(Msg)  GD_INFO_(dump,  GD_STREAM(Msg))

# define GD_VINFO_LOG(Msg, Exp)   GD_INFO_LOG  ("%s: %s = %s", Msg, #Exp, Exp)
# define GD_VINFO_TRACE(Msg, Exp) GD_INFO_TRACE("%s: %s = %s", Msg, #Exp, Exp)
# define GD_VINFO_DEBUG(Msg, Exp) GD_INFO_DEBUG("%s: %s = %s", Msg, #Exp, Exp)
# define GD_VINFO_DUMP(Msg, Exp)  GD_INFO_DUMP ("%s: %s = %s", Msg, #Exp, Exp)


/*-------.
| WARN.  |
`-------*/

# define GD_WARN(Message)   GD_MESSAGE_(warn, log, Message)
# define GD_FWARN(Msg, ...) GD_WARN(GD_FORMAT(Msg, __VA_ARGS__))
# define GD_SWARN(Msg)      GD_WARN(GD_STREAM(Msg))
# define GD_VWARN(Msg, Exp) GD_FWARN("%s: %s = %s", Msg, #Exp, Exp)

/*--------.
| ERROR.  |
`--------*/

# define GD_ERROR(Message)   GD_MESSAGE_(error, log, Message)
# define GD_FERROR(Msg, ...) GD_ERROR(GD_FORMAT(Msg, __VA_ARGS__))
# define GD_SERROR(Msg)      GD_ERROR(GD_STREAM(Msg))
# define GD_VERROR(Msg, Exp) GD_FERROR("%s: %s = %s", Msg, #Exp, Exp)

/*-------.
| PUSH.  |
`-------*/

# define GD_PUSH(Msg)       GD_PUSH_(Msg, log)
# define GD_PUSH_LOG(Msg)   GD_PUSH_(Msg, log)
# define GD_PUSH_TRACE(Msg) GD_PUSH_(Msg, trace)
# define GD_PUSH_DEBUG(Msg) GD_PUSH_(Msg, debug)
# define GD_PUSH_DUMP(Msg)  GD_PUSH_(Msg, dump)

# define GD_FPUSH(Msg, ...)       GD_PUSH      (GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FPUSH_LOG(Msg, ...)   GD_PUSH_LOG  (GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FPUSH_TRACE(Msg, ...) GD_PUSH_TRACE(GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FPUSH_DEBUG(Msg, ...) GD_PUSH_DEBUG(GD_FORMAT(Msg, __VA_ARGS__))
# define GD_FPUSH_DUMP(Msg, ...)  GD_PUSH_DUMP (GD_FORMAT(Msg, __VA_ARGS__))

/*--------.
| Level.  |
`--------*/

# define GD_FILTER_NONE()                       \
  GD_FILTER(::libport::Debug::levels::none)

# define GD_FILTER_LOG()                        \
  GD_FILTER(::libport::Debug::levels::log)

# define GD_FILTER_TRACE()                      \
  GD_FILTER(::libport::Debug::levels::trace)

# define GD_FILTER_DEBUG()                      \
  GD_FILTER(::libport::Debug::levels::debug)

# define GD_FILTER_DUMP()                       \
  GD_FILTER(::libport::Debug::levels::dump)

# define GD_SHOW_LOG()                          \
  GD_SHOW_LEVEL(::libport::Debug::levels::log)

# define GD_SHOW_TRACE()                                \
  GD_SHOW_LEVEL(::libport::Debug::levels::trace)

# define GD_SHOW_DEBUG()                                \
  GD_SHOW_LEVEL(::libport::Debug::levels::debug)

# define GD_SHOW_DUMP()                         \
  GD_SHOW_LEVEL(::libport::Debug::levels::dump)

/*-------------.
| Assertions.  |
`-------------*/

# define GD_FABORT(Msg, ...)                    \
  GD_ABORT(GD_FORMAT(Msg, __VA_ARGS__))

# define GD_UNREACHABLE()                       \
  GD_ABORT("Unreachable code reached")

/*------------.
| Configure.  |
`------------*/

# define GD_INIT_CONSOLE_DEBUG_PER(DebugData)   \
  GD_INIT_DEBUG_PER_(DebugData, ::libport::ConsoleDebug)

# define GD_INIT_SYSLOG_DEBUG_PER(Program, DebugData)   \
  GD_INIT_DEBUG_PER_(DebugData, ::libport::SyslogDebug(#Program))

// Must be called before any use.
# define GD_INIT()                             \
  GD_INIT_CONSOLE()

# define GD_INIT_DEBUG_PER(DebugData)   \
  GD_INIT_CONSOLE_DEBUG_PER(DebugData)

# define GD_INIT_CONSOLE()                              \
  GD_INIT_CONSOLE_DEBUG_PER(GD_DEFAULT_DEBUG_DATA)

# define GD_INIT_SYSLOG(Program)                                \
  GD_INIT_SYSLOG_DEBUG_PER(Program, GD_DEFAULT_DEBUG_DATA)

# define GD_ENABLE_LOCATIONS()                  \
  GD_ENABLE(locations)

# define GD_ENABLE_TIMESTAMPS()                 \
  GD_ENABLE(timestamps)

#endif
