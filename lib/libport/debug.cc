/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>

#include <boost/thread/tss.hpp>

#include <libport/cassert>
#include <libport/compiler.hh>
#include <libport/containers.hh>
#include <libport/cstdio>
#include <libport/debug.hh>
#include <libport/escape.hh>
#include <libport/fnmatch.h>
#include <libport/foreach.hh>
#include <libport/format.hh>
#include <libport/ip-semaphore.hh>
#include <libport/lockable.hh>
#include <libport/pthread.h>
#include <libport/thread-data.hh>
#include <libport/tokenizer.hh>
#include <libport/unistd.h>
#include <libport/utime.hh>
#include <libport/windows.hh>
#include <sched/coroutine-data.hh>

#ifndef WIN32
# include <syslog.h>
#endif

#ifndef LIBPORT_DEBUG_DISABLE

GD_CATEGORY(Libport.Debug);

namespace libport
{
  LIBPORT_API boost::function0<local_data&> debugger_data;
  LIBPORT_API Debug* debugger = 0;
  LIBPORT_API Debug::levels::Level Debug::filter_(levels::log);

  local_data&
  debugger_data_thread_local()
  {
    static boost::thread_specific_ptr<local_data> storage;
    if (!storage.get())
      storage.reset(new local_data);
    return *storage;
  }


  local_data::local_data()
    : indent(0)
  {}


  namespace debug
  {
    // Whether categories are enabled by default.
    static bool default_category_state = true;

    LIBPORT_API void
    uninitialized_msg(const std::string& msg)
    {
      static bool dump =
        getenv("GD_LEVEL") && libport::streq(getenv("GD_LEVEL"), "DUMP");
      if (dump)
      {
        static bool first = true;
        if (first)
        {
          std::cerr << "[Libport.Debug] "
                    << "Uninitialized debug, fallback to stderr." << std::endl;
          first = false;
        }

        std::cerr << "[Libport.Debug] " << msg;
        if (msg.empty() || msg[msg.size() - 1] != '\n')
          std::cerr << std::endl;
      }
    }

    // Categories added so far.
    ATTRIBUTE_PURE
    categories_type&
    categories()
    {
      static categories_type categories;
      return categories;
    }

    namespace
    {

      ATTRIBUTE_PURE
      static unsigned
      current_pattern()
      {
        static unsigned current = 0;
        return current++;
      }

      // Patterns added so far.
      ATTRIBUTE_PURE
      patterns_type&
      patterns()
      {
        static patterns_type patterns;
        return patterns;
      }

      // Maximum category width.
      ATTRIBUTE_PURE
      size_t&
      categories_largest()
      {
        static size_t res = 0;
        return res;
      }

      inline
      bool
      match(Symbol globbing, Symbol string)
      {
        return fnmatch(globbing.name_get(), string.name_get()) == 0;
      }
    }

    /// Add a new category, look if it matches a pattern.
    Symbol
    add_category(Symbol name)
    {
      int order = -1;
      bool value = default_category_state;
      foreach (patterns_type::value_type& s, patterns())
        if (match(s.first, name)
            && order < int(s.second.second))
        {
          value = s.second.first;
          order = s.second.second;
        }

      categories()[name] = value;

      categories_largest() =
        std::max(categories_largest(), name.name_get().size());

      return name;
    }

    /// Add a new category pattern.
    int
    enable_category(Symbol pattern, bool enabled)
    {
      patterns()[pattern] = std::make_pair(enabled, current_pattern());
      foreach (categories_type::value_type& s, categories())
        if (match(pattern, s.first))
          s.second = enabled;

      return 42;
    }

    // Disable a new category pattern.
    int
    disable_category(Symbol pattern)
    {
      return enable_category(pattern, false);
    }

    // Enable/Disable a new category pattern with modifier.
    int
    auto_category(Symbol pattern)
    {
      std::string p = pattern.name_get();
      char modifier = p[0];
      if (modifier == '+' || modifier == '-')
        p = p.substr(1);
      else
        modifier = '+';
      return enable_category(Symbol(p), modifier == '+');
    }

    bool test_category(Symbol name)
    {
      return categories()[name];
    }

    typedef enum
    {
      ENABLE,
      DISABLE,
      AUTO,
    } category_modifier_type;

    /// Enable/disable categories, already seen or future.
    ///
    /// Called by GD_INIT.
    ///
    /// \param list  the value of the environment var (GD_CATEGORY, etc.).
    /// \param state the associated state (AUTO, ENABLE, DISABLE).
    static
    void
    set_category_state(const char* list, const category_modifier_type state)
    {
      // If the mode is "AUTO", then if the first specs is to enable
      // ("-...") then the default is to enable, otherwise disable.
      // If the mode if not AUTO, then the default is the converse of
      // the mode: ENABLE -> false, and DISABLE => true.
      default_category_state =
        state == AUTO ? (*list == '-') : (state != ENABLE);

      // Set all the existing categories to the default behavior
      // before running the per-pattern tests.
      foreach (categories_type::value_type& v, categories())
        v.second = default_category_state;

      std::string s(list); // Do not pass temporary to make_tokenizer.
      tokenizer_type t = make_tokenizer(s, ",");
      foreach (const std::string& elem, t)
      {
        Symbol pattern(elem);
        switch (state)
        {
          case ENABLE:
          case DISABLE:
            enable_category(pattern, state == ENABLE);
            break;
          case AUTO:
            auto_category(pattern);
            break;
        }
      }
    }
  }

  Debug::Debug()
    : locations_(getenv("GD_LOC"))
    , timestamps_(getenv("GD_TIME") || getenv("GD_TIMESTAMP_US"))
  {
    // Process enabled/disabled/auto categories in environment.
    if (const char* cp = getenv("GD_CATEGORY"))
      debug::set_category_state(cp, debug::AUTO);
    else
    {
      if (const char* cp = getenv("GD_ENABLE_CATEGORY"))
        debug::set_category_state(cp, debug::ENABLE);
      if (const char* cp = getenv("GD_DISABLE_CATEGORY"))
        debug::set_category_state(cp, debug::DISABLE);
    }

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
      pabort("invalid debug level (NONE, LOG, TRACE, DEBUG, DUMP or [0-4])");
  }

  unsigned
  Debug::indentation() const
  {
    return debugger_data().indent;
  }

  Debug::levels::Level
  Debug::level()
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
               debug::category_type category,
               const std::string& fun,
               const std::string& file,
               unsigned line)
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
    message(category, msg, type, fun, file, line);
  }

  Debug*
  Debug::push(debug::category_type category,
              const std::string& msg,
              const std::string& fun,
              const std::string& file,
              unsigned line)
  {
    message_push(category, msg, fun, file, line);
    return this;
  }

  std::string
  Debug::category_format(debug::category_type cat) const
  {
    std::string res = cat;
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

  namespace opts
  {

    void cb_debug_fun(const std::string& lvl)
    {
      GD_DEBUGGER->filter(lvl);
    }

    OptionValued::callback_type cb_debug(cb_debug_fun);

    OptionValue
    debug("set log verbosity (NONE, LOG, TRACE, DEBUG, DUMP) [LOG]",
          "debug", 'd', "LEVEL", cb_debug);

  }

  ConsoleDebug::ConsoleDebug()
  {}

  namespace
  {
    inline
    std::string
    time()
    {
      static bool us = getenv("GD_TIMESTAMP_US");
      if (us)
        return string_cast(utime());
      time_t now = std::time(0);
      struct tm* ts = std::localtime(&now);
      char buf[80];
      strftime(buf, sizeof buf, "%a %Y-%m-%d %H:%M:%S %Z", ts);
      return buf;
    }

    inline
    std::string
    color(int color, bool bold = true)
    {
      static bool tty = isatty(STDERR_FILENO);
      static bool force = getenv("GD_COLOR");
      static bool force_disable = getenv("GD_NO_COLOR");
      return (((tty || force) && !force_disable)
              ? format("[33;0%s;%sm", bold ? 1 : 0, color)
              : "");
    }
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
  ConsoleDebug::message(debug::category_type category,
                        const std::string& msg,
                        types::Type type,
                        const std::string& fun,
                        const std::string& file,
                        unsigned line)
  {
    std::ostringstream ostr;
    Debug::colors::Color c = msg_color(type);
    if (timestamps())
      ostr << color(c) << time() << "    ";
    ostr << color(colors::purple)
         << "[" << category_format(category) << "] ";
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
    ostr << color(c);
    for (unsigned i = 0; i < debugger_data().indent; ++i)
      ostr << "  ";
    // As syslog would do, don't issue the users' \n.
    if (!msg.empty() && msg[msg.size() - 1] == '\n')
      ostr.write(msg.c_str(), msg.size() - 1);
    else
      ostr << msg;
    if (locations())
      ostr << color(colors::blue)
           << "    (" << fun << ", " << file << ":" << line << ")";
    ostr << color(colors::white);
    std::cerr << ostr.str() << std::endl;
  }

  void
  ConsoleDebug::message_push(debug::category_type category,
                             const std::string& msg,
                             const std::string& fun,
                             const std::string& file,
                             unsigned line)
  {
    debug(msg, types::info, category, fun, file, line);
    GD_INDENTATION_INC();
  }

  void
  ConsoleDebug::pop()
  {
    assert_gt(debugger_data().indent, 0u);
    GD_INDENTATION_DEC();
  }

  std::string gd_ihexdump(const unsigned char* data, unsigned size)
  {
    std::string res =
      format("\"%s\"",
             libport::escape(std::string((const char*)data, (size_t)(size))));

    bool first = true;
    for (unsigned i = 0; i < size; ++i)
    {
      if (first)
        first = false;
      else
        res += " ";
      // Cast to int, or boost::format will print the character.
      res += format("0x%x", static_cast<unsigned int>(data[i]));
    }
    return res;
  }

#ifndef WIN32
  /*-------------.
  | Syslog debug |
  `-------------*/

  SyslogDebug::SyslogDebug(const std::string& program)
  {
    openlog(strdup(program.c_str()), LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO | LOG_DAEMON, "%s",
           format("Opening syslog session for '%s'", program).c_str());
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
#undef CASE
    }
    // Pacify Gcc.
    libport::abort();
  }

  void
  SyslogDebug::message(debug::category_type category,
                       const std::string& msg,
                       types::Type type,
                       const std::string& fun,
                       const std::string& file,
                       unsigned line)
  {
    std::stringstream s;
    s << "[" << category_format(category) << "] ";
    for (unsigned i = 0; i < debugger_data().indent; ++i)
      s << "  ";
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
  SyslogDebug::message_push(debug::category_type category,
                            const std::string& msg,
                            const std::string& fun,
                            const std::string& file,
                            unsigned line)
  {
    debug(msg, types::info, category, fun, file, line);
    GD_INDENTATION_INC();
  }

  void
  SyslogDebug::pop()
  {
    assert_gt(debugger_data().indent, 0u);
    GD_INDENTATION_DEC();
  }
#endif

  boost::function0<Debug*> make_debugger;

  namespace debug
  {
    void clear()
    {
#if FIXME
      delete debugger();
#endif
    }
  }
}

#endif
