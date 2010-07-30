/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{

  inline
  bool Debug::enabled(levels::Level lvl) const
  {
    return (lvl <= filter_
            && !categories_stack_.empty()
            && debug::test_category(categories_stack_.back()));
  }

  inline
  void
  Debug::debug(const std::string& msg,
               types::Type type,
               const std::string& fun,
               const std::string& file,
               unsigned line)
  {
    debug(msg, type, level_stack_.back(), fun, file, line);
  }

  inline
  Debug*
  Debug::push(const std::string& msg,
              const std::string& fun,
              const std::string& file,
              unsigned line)
  {
    return push(level_stack_.back(), msg, fun, file, line);
  }

#define GD_ATTRIBUTE(Name)                      \
  inline                                        \
  void Debug::Name(bool v)                      \
  {                                             \
    Name##_ = v;                                \
  }                                             \
                                                \
  inline                                        \
  bool Debug::Name() const                      \
  {                                             \
    return Name##_;                             \
  }                                             \

  GD_ATTRIBUTE(locations);
  GD_ATTRIBUTE(timestamps);

#undef GD_ATTRIBUTE

}
