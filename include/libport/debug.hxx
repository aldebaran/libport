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

  // Whether the message should be displayed.
  inline
  bool Debug::enabled() const
  {
    return (level_stack_.back() <= filter_
            && !categories_stack_.empty()
            && debug::test_category(categories_stack_.back()));
  }


#define ATTRIBUTE(Name)                         \
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

  ATTRIBUTE(locations);
  ATTRIBUTE(timestamps);

#undef ATTRIBUTE

}
