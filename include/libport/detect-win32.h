#ifndef LIBPORT_DETECT_WIN32_H
# define LIBPORT_DETECT_WIN32_H

# ifndef WIN32
#  if defined LIBPORT_WIN32 || defined _MSC_VER
#   define WIN32
#  endif
# endif

# ifdef WIN32
#  ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0400
#  endif
# endif /* !WIN32 */

# ifdef WIN32
#  define WIN32_IF(Then, Else) Then
#  define EXEEXT ".exe"
# else
#  define WIN32_IF(Then, Else) Else
#  define EXEEXT ""
# endif

#endif // !LIBPORT_DETECT_WIN32_H
