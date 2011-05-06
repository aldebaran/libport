/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UVECTOR_HH
# define LIBPORT_UVECTOR_HH

# include <libport/ufloat.hh>
# include <libport/system-warning-push.hh>
#  include <boost/numeric/ublas/blas.hpp>
#  include <boost/numeric/ublas/matrix.hpp>
#  include <boost/numeric/ublas/vector.hpp>
# include <libport/system-warning-pop.hh>

namespace libport
{
  /*--------------.
  | vector_type.  |
  `--------------*/

  typedef boost::numeric::ublas::vector<ufloat> vector_type;

}

namespace boost
{
  namespace numeric
  {
    namespace ublas
    {

      LIBPORT_API
      bool
      operator==(const ::libport::vector_type& e1,
                 const ::libport::vector_type& e2);

      LIBPORT_API
      std::ostream&
      operator<<(std::ostream& o, const ::libport::vector_type& v);

    }
  }
}
#endif
