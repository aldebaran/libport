/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/umatrix.cc
/// \brief ufloat matrices

#include <libport/uvector.hh>

namespace boost
{
  namespace numeric
  {
    namespace ublas
    {

      bool
      operator==(const ::libport::vector_type& e1,
                 const ::libport::vector_type& e2)
      {
        if (e1.size() != e2.size())
          return false;
        for (unsigned i = 0; i < e1.size(); ++i)
          if (e1(i) != e2(i))
            return false;
        return true;
      }

      std::ostream&
      operator<<(std::ostream& o, const ::libport::vector_type& v)
      {
        o << '<';
        for (unsigned i = 0; i < v.size(); ++i)
        {
          if (i)
            o << ", ";
          o << v(i);
        }
        return o << '>';
      }

    }
  }
}
