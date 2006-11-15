#ifndef LIBPORT_STRING_HH
# define LIBPORT_STRING_HH

# include <cstring>

/// Whether the C string \a Lhs and \a Rhs are equal.
# define STREQ(Lhs, Rhs) (!strcmp (Lhs, Rhs))

#endif
