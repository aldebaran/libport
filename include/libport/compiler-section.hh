/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/compiler-section.hh
///
/// \brief Introduce a new section which can be used to annotate symbol
/// declarations.
///
/// This header has parameters used to define sections property.  It
/// undefines all of its parameters.

#ifndef LIBPORT_SECTION_NAME
# error "Section name is not defined."
#endif

/// read
///   Allows read operations on data.
#ifdef LIBPORT_SECTION_READ
# define __SEC_READ ,read
# undef LIBPORT_SECTION_READ
#else
# define __SEC_READ
#endif

/// write
///   Allows write operations on data.
#ifdef LIBPORT_SECTION_WRITE
# define __SEC_WRITE ,write
# undef LIBPORT_SECTION_WRITE
#else
# define __SEC_WRITE
#endif

/// execute
///   Allows code to be executed.
#ifdef LIBPORT_SECTION_EXEC
# define __SEC_EXEC ,exec
# undef LIBPORT_SECTION_EXEC
#else
# define __SEC_EXEC
#endif

#ifdef _MSC_VER
# pragma section(LIBPORT_SECTION_NAME __SEC_READ __SEC_WRITE __SEC_EXEC)
#else
// How do we define section rights with GCC ?  It seems to infer them based
// on the symbols put in it.
#endif

#undef LIBPORT_SECTION_NAME
#undef __SEC_READ
#undef __SEC_WRITE
#undef __SEC_EXEC
