#ifndef LIBPORT_WINDOWS_HH
# define LIBPORT_WINDOWS_HH

# if defined WIN32 || defined LIBPORT_WIN32

#  ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0400
#  endif
# include <windows.h>

# define usleep(a) Sleep((a) < 1000 ? 1 : (a) / 1000)

# endif // !WIN32

#endif // !LIBPORT_WINDOWS_HH
