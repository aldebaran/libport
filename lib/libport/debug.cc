/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <iostream>

#include <libport/cassert>
#include <libport/containers.hh>
#include <libport/cstdio>
#include <libport/debug.hh>
#include <libport/escape.hh>
#include <libport/foreach.hh>
#include <libport/ip-semaphore.hh>
#include <libport/lockable.hh>
#include <libport/pthread.h>
#include <libport/windows.hh>
#include <libport/unistd.h>

#ifndef WIN32
# include <syslog.h>
#endif

#ifndef LIBPORT_DEBUG_DISABLE

GD_ADD_CATEGORY(NONE);

namespace libport
{

  namespace debug
  {
    categories_type& get_categories()
    {
      static categories_type categories;
      return categories;
    }

    size_t&
    categories_largest()
    {
      static size_t res = 0;
      return res;
    }

    Symbol
    add_category(Symbol name)
    {
      get_categories()[name] = true;
      size_t size = name.name_get().size();
      if (categories_largest() < size)
        categories_largest() = size;
      return name;
    }

    int enable_category(Symbol name)
    {
      get_categories()[name] = true;
      return 42;
    }

    int disable_category(Symbol name)
    {
      get_categories()[name] = false;
      return 42;
    }

    bool test_category(Symbol name)
    {
      return get_categories()[name];
    }
  }

  Debug::Debug()
    : categories_stack_()
    , level_stack_()
    , locations_(getenv("GD_LOC"))
    , timestamps_(getenv("GD_TIME"))
    , filter_(levels::log)
  {
    push_level(levels::log);
    if (const char* lvl_c = getenv("GD_LEVEL"))
      filter(lvl_c);
  }

  Debug::~Debug()
  {}

  void
  Debug::filter(levels::Level lvl)
  {
    filter_ = lvl;
  }

  void
  Debug::filter(const std::string& lvl)
  {
    if (lvl == "NONE" || lvl == "0")
      filter(Debug::levels::none);
    else if (lvl == "LOG" || lvl == "1")
      filter(Debug::levels::log);
    else if (lvl == "TRACE" || lvl == "2")
      filter(Debug::levels::trace);
    else if (lvl == "DEBUG" || lvl == "3")
      filter(Debug::levels::debug);
    else if (lvl == "DUMP" || lvl == "4")
      filter(Debug::levels::dump);
    else
      // Don't use GD_ABORT here, we can be in the debugger constructor!
      aver(!"invalid debug level (NONE, LOG, TRACE, DEBUG, DUMP or [0-4])");
  }

  Debug::levels::Level
  Debug::level() const
  {
    return filter_;
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
      static bool useLock = getenv("GD_USE_LOCK") || getenv("GD_PID");
      libport::Finally f;
      if (useLock)
      {
        --sem();
        f << boost::bind(&IPSemaphore::operator++, boost::ref(sem()));
      }
# endif
      message(msg, type, fun, file, line);
    }
  }

  libport::Finally::action_type
  Debug::push_category(Symbol category)
  {
    categories_stack_.push_back(category);
    return boost::bind(&Debug::pop_category, this);
  }

  libport::Finally::action_type
  Debug::push_level(levels::Level lvl)
  {
    level_stack_.push_back(lvl);
    return boost::bind(&Debug::pop_level, this);
  }

  bool
  Debug::disabled()
  {
    return (level_stack_.back() > filter_
            || categories_stack_.empty()
            || !debug::test_category(categories_stack_.back()));
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
    aver(!categories_stack_.empty());
    categories_stack_.pop_back();
  }

  void
  Debug::pop_level()
  {
    level_stack_.pop_back();
  }

  bool
  Debug::test_category(Symbol c) const
  {
    return (categories_stack_.empty()
            ? GD_GET_CATEGORY(NONE) == c
            : categories_stack_.back() == c);
  }

  std::string
  Debug::category()
  {
    std::string res = (categories_stack_.empty()
                       ? GD_GET_CATEGORY(NONE).name_get()
                       : categories_stack_.back().name_get());
    size_t size = res.size();
    size_t largest = debug::categories_largest();
    if (size < largest)
    {
      size_t diff = largest - size;
      res = std::string(diff / 2, ' ')
        + res
        + std::string(diff / 2 + diff % 2, ' ');
    }

    return res;
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
    libport::abort();
  }

  namespace opts
  {

    void cb_debug_fun(const std::string& lvl)
    {
      GD_DEBUGGER->filter(lvl);
    }

    OptionValued::callback_type cb_debug(&cb_debug_fun);

    OptionValue
    debug("set the debug level in NONE, LOG (default), TRACE, DEBUG, DUMP",
          "debug", 'd', "LEVEL", &cb_debug);

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
    static bool force = getenv("GD_COLOR");
    boost::format format("[33;0%s;%sm");
    if (tty || force)
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
    std::ostringstream ostr;
    Debug::colors::Color c = msg_color(type);
    if (timestamps())
    {
      color(c);
      show_time();
      ostr << "    ";
    }
    color(colors::purple);
    ostr << "[" << category() << "] ";
    {
      static bool pid = getenv("GD_PID");
      if (pid)
        ostr << "[" << getpid() << "] ";
    }
#ifndef WIN32
    {
      static bool thread = getenv("GD_THREAD");
      if (thread)
        ostr << "[" << pthread_self() << "] ";
    }
#endif
    color(c);
    for (unsigned i = 0; i < indent_; ++i)
      ostr << " ";
    // As syslog would do, don't issue the users' \n.
    if (!msg.empty() && msg[msg.size() - 1] == '\n')
      ostr.write(msg.c_str(), msg.size() - 1);
    else
      ostr << msg;
    if (locations())
    {
      color(colors::blue);
      ostr << "    (" << fun << ", " << file << ":" << line << ")";
    }
    color(colors::white);
    ostr << std::endl;
    std::cerr << ostr.str();
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
    std::stringstream ss;
    ss << libport::escape(std::string((const char*)data, (size_t)(size)));
    std::string res = "\"" + ss.str() + "\" ";
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
    libport::abort();
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
    // As syslog would do, don't issue the users' \n.
    if (!msg.empty() && msg[msg.size() - 1] == '\n')
      s.write(msg.c_str(), msg.size() - 1);
    else
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
  AbstractLocalData<Debug>* debugger_data;

  typedef std::map<pthread_t, Debug*> map_type;
  // Do not make it an actual object, as it is sometimes used on
  // dtors, and the order to destruction is not specified.  Using a
  // dynamically allocated object protects us from this.
  static map_type* pdebuggers = new map_type;
  libport::Lockable debugger_mutex_;

  namespace debug
  {
    void clear()
    {
#if FIXME
      typedef std::pair<pthread_t, Debug*> entry;
      foreach (entry p, *pdebuggers)
      {
        delete p.second;
        p.second = 0;
      }
#endif
      delete pdebuggers;
      pdebuggers = 0;
    }
  }

  Debug* debugger()
  {
    libport::BlockLock lock(debugger_mutex_);
    bool no_gd_init = false;
    Debug* d;

    if (!debugger_data)
    {
      debugger_data =
        new LocalData<Debug, ::libport::localdata::Thread>;
      no_gd_init = true;
    }

    d = debugger_data->get();
    if (!d)
    {
      if (make_debugger.empty())
      {
        d = new ConsoleDebug;
        no_gd_init = true;
      }
      else
        d = make_debugger();
      debugger_data->set(d);
      d->push_category(GD_GET_CATEGORY(NONE));
    }

    if (no_gd_init)
    {
# define _GD_WARN(Message)                              \
  d->debug(Message, ::libport::Debug::types::warn,      \
           GD_FUNCTION, __FILE__, __LINE__)

      _GD_WARN("GD_INIT was not invoked, defaulting to console logs");
# undef _GD_WARN
    }

    return d;
  }

}

#endif
