/// \file libport/compiler.hh
/// \brief Specific features from some compilers.

#ifndef LIBPORT_COMPILER_HH
# define LIBPORT_COMPILER_HH

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#  define __attribute__(Spec) /* empty */
# endif
#endif

#endif // !LIBPORT_COMPILER_HH
