#include <cassert>
#include <iostream>

#include <libport/containers.hh>
#include <libport/debug.hh>
#include <libport/ip-semaphore.hh>
#include <libport/windows.hh>
#include <libport/unistd.h>

#ifndef WIN32
# include <syslog.h>
# include <pthread.h>
#endif

#ifndef LIBPORT_DEBUG_DISABLE

namespace libport
{

  namespace debug
  {
    categories_type& get_categories()
    {
      static categories_type categories;
      return categories;
    }

    int add_category(const std::string& name)
    {
      get_categories()[name] = true;
      return 42;
    }

    int enable_category(const std::string& name)
    {
      get_categories()[name] = true;
      return 42;
    }

    int disable_category(const std::string& name)
    {
      get_categories()[name] = false;
      return 42;
    }

    bool test_category(const std::string& name)
    {
      return get_categories()[name];
    }
  }

  Debug::Debug()
    : categories_stack_()
    , level_stack_()
    , locations_(getenv("GD_LOC"))
    , timestamps_(getenv("GD_TIME"))
    , filter_(1)
  {
    push_level(1);
    if (const char* lvl_c = getenv("GD_LEVEL"))
    {
      std::string lvl = lvl_c;
      if (lvl == "NONE")
        filter_ = 0;
      else if (lvl == "LOG")
        filter_ = 1;
      else if (lvl == "TRACE")
        filter_ = 2;
      else if (lvl == "DEBUG")
        filter_ = 3;
      else if (lvl == "DUMP")
        filter_ = 4;
      else
        // Don't use GD_ABORT here, we're in the debugger constructor!
        assert(!"invalid debug level (NONE, LOG, TRACE, DEBUG, DUMP)");
    }
  }

  Debug::~Debug()
  {}

  void
  Debug::filter(unsigned lvl)
  {
    filter_ = lvl;
  }

# ifdef LIBPORT_HAVE_IP_SEMAPHORE
  static IPSemaphore& sem()
  {
    static IPSemaphore res(1);
    return res;
  }
# endif

  void
  Debug::debug(const std::string& msg,
               types::Type type,
               const std::string& fun,
               const std::string& file,
               unsigned line)
  {
    if (disabled())
      return;

    {
# ifdef LIBPORT_HAVE_IP_SEMAPHORE
      IPSemaphore::Lock lock(sem());
# endif
      message(msg, type, fun, file, line);
    }
  }

  libport::Finally::action_type
  Debug::push_category(const std::string& category)
  {
    categories_stack_.push_back(category);
    return boost::bind(&Debug::pop_category, this);
  }

  libport::Finally::action_type
  Debug::push_level(unsigned lvl)
  {
    level_stack_.push_back(lvl);
    return boost::bind(&Debug::pop_level, this);
  }

  bool
  Debug::disabled()
  {
    return !debug::test_category(category()) || level_stack_.back() > filter_;
  }

  static void noop()
  {}

  libport::Finally::action_type
  Debug::push(const std::string& msg,
              const std::string& fun,
              const std::string& file,
              unsigned line)
  {
    if (disabled())
      return noop;
    message_push(msg, fun, file, line);
    return boost::bind(&Debug::pop, this);
  }

  void
  Debug::pop_category()
  {
    categories_stack_.pop_back();
  }

  void
  Debug::pop_level()
  {
    level_stack_.pop_back();
  }

  std::string
  Debug::category()
  {
    assert(!categories_stack_.empty());
    return categories_stack_.back();
  }

#define ATTRIBUTE(Name)                         \
                                                \
  void Debug::Name(bool v)                      \
  {                                             \
    Name##_ = v;                                \
  }                                             \
                                                \
  bool Debug::Name()                            \
  {                                             \
    return Name##_;                             \
  }                                             \

  ATTRIBUTE(locations);
  ATTRIBUTE(timestamps);

#undef ATTRIBUTE

  void Debug::abort(const std::string& msg)
  {
    GD_ERROR(msg);
    std::abort();
    throw 0;
  }

  ConsoleDebug::ConsoleDebug()
    : indent_(0)
  {}

  static void show_time()
  {
    time_t     now;
    struct tm  *ts;
    char       buf[80];

    now = time(NULL);
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
    std::cerr << buf;
  }

  void
  ConsoleDebug::color(int color, bool bold)
  {
    static bool tty = isatty(STDOUT_FILENO);
    boost::format format("[33;0%s;%sm");
    if (tty)
      std::cerr << str(format % (bold ? 1 : 0) % color);
  }

  void
  ConsoleDebug::reset()
  {
    color(0);
  }

  static Debug::colors::Color
  msg_color(Debug::types::Type type)
  {
    switch (type)
    {
      case Debug::types::info:
        return Debug::colors::white;
      case Debug::types::warn:
        return Debug::colors::yellow;
      case Debug::types::error:
        return Debug::colors::red;
    };
    GD_UNREACHABLE();
  }

  void
  ConsoleDebug::message(const std::string& msg,
                        types::Type type,
                        const std::string& fun,
                        const std::string& file,
                        unsigned line)
  {
    Debug::colors::Color c = msg_color(type);
    if (timestamps())
    {
      color(c);
      show_time();
      std::cerr << "    ";
    }
    color(colors::purple);
    std::cerr << "[" << category() << "] ";
    {
      static bool pid = getenv("GD_PID");
      if (pid)
        std::cerr << "[" << getpid() << "] ";
    }
#ifndef WIN32
    {
      static bool thread = getenv("GD_THREAD");
      if (thread)
        std::cerr << "[" << pthread_self() << "] ";
    }
#endif
    color(c);
    for (unsigned i = 0; i < indent_; ++i)
      std::cerr << " ";
    std::cerr << msg;
    if (locations())
    {
      color(colors::blue);
      std::cerr << "    (" << fun << ", " << file << ":" << line << ")";
    }
    std::cerr << std::endl;
  }

  void
  ConsoleDebug::message_push(const std::string& msg,
                             const std::string& fun,
                             const std::string& file,
                             unsigned line)
  {
    debug(msg, types::info, fun, file, line);
    indent_ += 2;
  }

  void
  ConsoleDebug::pop()
  {
    indent_ -= 2;
  }

  std::string gd_ihexdump(const unsigned char* data, unsigned size)
  {
    std::string res;
    bool first = true;
    for (unsigned i = 0; i < size; ++i)
    {
      if (first)
        first = false;
      else
        res += " ";
      boost::format format("0x%x");
      // This is sick, but we have to cast to int, or boost::format
      // will print the character.
      res += str(format % static_cast<unsigned int>(data[i]));
    }
    return res;
  }

#ifndef WIN32
  /*-------------.
  | Syslog debug |
  `-------------*/

  SyslogDebug::SyslogDebug(const std::string& program)
  {
    boost::format format("Opening syslog session for '%s'");

    openlog(strdup(program.c_str()), LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO | LOG_DAEMON, "%s", (format % program).str().c_str());
  }

  SyslogDebug::~SyslogDebug()
  {
    syslog(LOG_INFO | LOG_DAEMON, "Closing syslog session.");
    closelog();
  }

  static
  int type_to_prio(Debug::types::Type t)
  {
    switch (t)
    {
#define CASE(In, Out)                           \
      case Debug::types::In: return Out; break
      CASE(info,  LOG_INFO);
      CASE(warn,  LOG_WARNING);
      CASE(error, LOG_ERR);
      // Pacify Gcc.
    }
    abort();
#undef CASE
  }

  void
  SyslogDebug::message(const std::string& msg,
                        types::Type type,
                        const std::string& fun,
                        const std::string& file,
                        unsigned line)
  {
    std::stringstream s;
    s << "[" << category() << "] ";
    for (unsigned i = 0; i < indent_; ++i)
      s << " ";
    s << msg;
    if (locations())
      s << "    (" << fun << ", " << file << ":" << line << ")";
    int prio = type_to_prio(type) | LOG_DAEMON;
    syslog(prio, "%s", s.str().c_str());
  }

  void
  SyslogDebug::message_push(const std::string& msg,
                             const std::string& fun,
                             const std::string& file,
                             unsigned line)
  {
    debug(msg, types::info, fun, file, line);
    indent_ += 2;
  }

  void
  SyslogDebug::pop()
  {
    indent_ -= 2;
  }
#endif

  boost::function0<Debug*> make_debugger;

  Debug* debugger()
  {
#ifndef WIN32
    static std::map<pthread_t, Debug*> debuggers;
    pthread_t id = pthread_self();
    if (!libport::mhas(debuggers, id))
      debuggers[id] = make_debugger();
    return debuggers[id];
#else
    static Debug* debug = make_debugger();
    return debug;
#endif
  }

}

#endif
