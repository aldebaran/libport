/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UMATRIX_HH
# define LIBPORT_UMATRIX_HH

# include <libport/uvector.hh>

# include <libport/system-warning-push.hh>
#  include <boost/numeric/ublas/blas.hpp>
#  include <boost/numeric/ublas/matrix.hpp>
#  include <boost/numeric/ublas/vector.hpp>
# include <libport/system-warning-pop.hh>

namespace libport
{

  typedef boost::numeric::ublas::matrix<ufloat> matrix_type;

}

namespace boost
{
  namespace numeric
  {
    namespace ublas
    {

      LIBPORT_API
      bool
      operator==(const ::libport::matrix_type& e1,
                 const ::libport::matrix_type& e2);

      LIBPORT_API
      std::ostream&
      operator<<(std::ostream& o, const ::libport::matrix_type& v);

      LIBPORT_API
      ::libport::matrix_type
      inverse(const ::libport::matrix_type& m);

    }
  }
}

#endif
