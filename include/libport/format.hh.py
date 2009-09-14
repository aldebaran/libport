#!/usr/bin/python

# Copyright (C) 2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.


print '''\
#ifndef LIBPORT_FORMAT_HH
# define LIBPORT_FORMAT_HH

# include <libport/system-warning-push.hh>
# include <boost/format.hpp>
# include <libport/system-warning-pop.hh>

namespace libport
{

  // Also accept 0-ary format strings.  Don't return it directly,
  // still consult boost::format to make sure there are no trailing
  // %s.
  inline
  std::string format(const std::string& fmt)
  {
    boost::format f(fmt);
    return str(f);
  }
'''

def args(n, f, d = ', '):
    return d.join(map(f, range(n)))

for n in range(1, 9):
    a = {
        'template': args(n, lambda x : 'typename T%s' % x),
        'formals':  args(n, lambda x : 'const T%s& arg%s' % (x, x)),
        'format':   args(n, lambda x : 'arg%s' % x, ' % '),
        }
    print '''\
  template <%(template)s>
  inline
  std::string format(const std::string& fmt, %(formals)s)
  {
    boost::format f(fmt);
    f %% %(format)s;
    return str(f);
  }
''' % a

print '''\
}

#endif'''
