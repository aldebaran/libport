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
  bool Debug::enabled(levels::Level lvl, debug::category_type category) const
  {
    return (lvl <= filter_
            && debug::test_category(category));
  }

  inline
  void
  Debug::debug(const std::string& msg,
               types::Type type,
               debug::category_type category,
               const std::string& fun,
               const std::string& file,
               unsigned line)
  {
    debug(msg, type, level_stack_.back(), category, fun, file, line);
  }

  inline
  Debug*
  Debug::push(debug::category_type category,
              const std::string& msg,
              const std::string& fun,
              const std::string& file,
              unsigned line)
  {
    return push(level_stack_.back(), category, msg, fun, file, line);
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
