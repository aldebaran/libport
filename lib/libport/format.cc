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
#include <libport/lockable.hh>
#include <boost/unordered_map.hpp>

namespace libport
{
  class FormatMap: public boost::unordered_map<std::string, boost::format>
  {
    public:
      FormatMap(bool& ward)
        : ward_(ward)
      {
        ward_ = true;
      }

      ~FormatMap()
      {
        ward_ = false;
      }

    private:
      bool& ward_;
  };

  LIBPORT_API
  boost::format
  format_get(const std::string& s)
  {
    static bool map_ok(false);
    static FormatMap map(map_ok);
    if (map_ok)
    {
      static libport::Lockable lock;
      libport::BlockLock bl(lock);
      FormatMap::iterator i = map.find(s);
      if (i == map.end())
        return map[s].parse(s);
      else
        return i->second;
    }
    else
      return boost::format(s);
  }

  LIBPORT_API
  void
  format_failure(const std::string& fmt, const std::exception& e)
  {
    LIBPORT_USE(fmt);
    GD_CATEGORY(Libport.Format);
    GD_FERROR("format: \"%s\": %s", fmt, e.what());
    throw e;
  }

}
