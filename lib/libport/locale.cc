/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/locale.hh>
#include <libport/format.hh>

#define FRAISE(Format, ...)                                             \
  throw std::runtime_error(libport::format(Format, ## __VA_ARGS__))

namespace libport
{
  std::string
  setlocale(const std::string& cat, boost::optional<std::string> loc)
  {
    int c = -1;
#define CASE(Name)                              \
    if (cat == #Name) c = Name
    CASE(LC_ALL);
    CASE(LC_COLLATE);
    CASE(LC_CTYPE);
// Courtesy of MS Windows.
#if defined LC_MESSAGES
    CASE(LC_MESSAGES);
#endif
    CASE(LC_MONETARY);
    CASE(LC_NUMERIC);
    CASE(LC_TIME);
#undef CASE
    if (c == -1)
      FRAISE("invalid category: %s", cat);
    if (const char *r = std::setlocale(c, loc ? loc.get().c_str() : 0))
      return r;
    if (!loc)
      FRAISE("cannot get locale %s", cat);
    else if (loc.get() == "")
      FRAISE("cannot set locale %s", cat);
    else
      FRAISE("cannot set locale %s to %s", cat, loc.get());
  }
}
