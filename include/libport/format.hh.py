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

# include <boost/format.hpp>

namespace libport
{
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
  std::string format(const std::string& fmt, %(formals)s)
  {
    boost::format f(fmt);
    f %% %(format)s;
    return str(f);
  }
''' % a

print '''\
}

#endif
'''
