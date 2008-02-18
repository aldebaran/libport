s/#undef  *\([ABCDEFGHIJKLMNOPQRSTUVWXYZ_]\)/#undef LIBPORT_\1/
s/#undef  *\([abcdefghijklmnopqrstuvwxyz]\)/#undef _libport_\1/
s/#define  *\([ABCDEFGHIJKLMNOPQRSTUVWXYZ_][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_]*\)\(.*\)/#ifndef LIBPORT_\1 \
# define LIBPORT_\1 \2 \
#endif/
s/#define  *\([abcdefghijklmnopqrstuvwxyz][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_]*\)\(.*\)/#ifndef _libport_\1 \
# define _libport_\1 \2 \
#endif/
/by autoheader/a \
\
/* Needed for std::min and std::max on Windows */ \
#ifndef NOMINMAX \
# define NOMINMAX \
#endif
