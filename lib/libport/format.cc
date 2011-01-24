/*
 * Copyright (C) 2010, 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/format.hh>
#include <libport/debug.hh>
#include <boost/unordered_map.hpp>

namespace libport
{

  LIBPORT_API
  boost::format
  format_get(const std::string& s)
  {
    typedef boost::unordered_map<std::string, boost::format> map_type;
    static map_type map;
    map_type::iterator i = map.find(s);
    if (i == map.end())
      return map[s].parse(s);
    else
      return i->second;
  }

  LIBPORT_API
  void
  format_failure(const std::string& fmt, const std::exception& e)
  {
    GD_CATEGORY(Libport.Format);
    GD_FERROR("format: \"%s\": %s", fmt, e.what());
    throw;
  }

}
