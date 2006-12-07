#ifndef LIBPORT_DETECT_WIN32_H_
# define LIBPORT_DETECT_WIN32_H_

# ifndef WIN32
#  ifdef LIBPORT_WIN32
#   define WIN32
#  endif
# endif

# ifndef WIN32
#  ifdef _MSC_VER /* Are we compiling with Microsoft's compiler? */
#    define WIN32
#  endif
# endif /* !WIN32 */

# ifdef WIN32
#  ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0400
#  endif
# endif /* !WIN32 */

#endif /* !LIBPORT_DETECT_WIN32_H_ */
