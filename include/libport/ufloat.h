/*
 * Copyright (C) 2008, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UFLOAT_H
# define LIBPORT_UFLOAT_H

// Floating point definition (emulated or real)
# include <libport/ufloat.hh>

// Currently we don't prefix all the uses in the kernel.
using libport::ufloat;
using libport::PI;
using libport::UINFINITY;

# include <libport/config.h>
# ifndef LIBPORT_HAVE_ROUND
inline double round(double x)
{
  return libport::round(x);
}
# endif /* !LIBPORT_HAVE_ROUND */
# ifndef LIBPORT_HAVE_TRUNC
inline double trunc(double x)
{
  return libport::trunc(x);
}
# endif /* !LIBPORT_HAVE_TRUNC */

#endif // !LIBPORT_UFLOAT_H
