/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
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
  bool Debug::enabled(levels::Level lvl, debug::category_type category)
  {
    return (lvl <= filter_
            && debug::test_category(category));
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
