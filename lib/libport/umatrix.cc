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

#include <stdexcept>
#include <libport/format.hh>
#include <libport/umatrix.hh>
#include <libport/uvector.hh>
#include <boost/numeric/ublas/lu.hpp>

#define FRAISE(Format, ...)                                             \
  throw std::runtime_error(libport::format(Format, ## __VA_ARGS__))

namespace boost
{
  namespace numeric
  {
    namespace ublas
    {

      bool
      operator==(const ::libport::matrix_type& e1,
                 const ::libport::matrix_type& e2)
      {
        return
          (e1.size1() == e2.size1()
           && e1.size2() == e2.size2()
           && norm_inf(e1 - e2) == 0);
      }

      std::ostream&
      operator<<(std::ostream& o, const ::libport::matrix_type& v)
      {
        const size_t height = v.size1();
        o << '<';
        for (unsigned i = 0; i < height; ++i)
        {
          if (i)
            o << ", ";
          o << boost::numeric::ublas::row(v, i);
        }
        return o << '>';
      }

      ::libport::matrix_type
      inverse(const ::libport::matrix_type& m)
      {
        size_t size1 = m.size1();
        size_t size2 = m.size2();
        if (size1 != size2)
          FRAISE("expected square matrix, got %dx%d", size1, size2);

        // Create a working copy of the input.
        ::libport::matrix_type A(m);

        // Create a permutation matrix for the LU-factorization.
        permutation_matrix<size_t> pm(size1);

        // Perform LU-factorization.
        if (lu_factorize(A, pm) != 0)
          FRAISE("non-invertible matrix: %s", m);

        // Create identity matrix of "inverse".
        ::libport::matrix_type res(size1, size2);
        res.assign(identity_matrix<libport::ufloat>(size1));
        // Backsubstitute to get the inverse.
        lu_substitute(A, pm, res);
        return res;
      }

    }
  }
}
