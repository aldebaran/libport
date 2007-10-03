#ifndef LIBPORT_UFLOAT_H
# define LIBPORT_UFLOAT_H

// Floating point definition (emulated or real)
# include "libport/ufloat.hh"

// Currently we don't prefix all the uses in the kernel.
using libport::ufloat;
using libport::PI;
using libport::UINFINITY;

# include "libport/config.h"
# ifndef LIBPORT_HAVE_ROUND
using libport::round;
# endif /* !LIBPORT_HAVE_ROUND */
# ifndef LIBPORT_HAVE_TRUNC
using libport::trunc;
# endif /* !LIBPORT_HAVE_TRUNC */

#endif // !LIBPORT_UFLOAT_H
