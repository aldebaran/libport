#include <libport/fcntl.h>

#ifdef WIN32
# include <io.h>

// Wrap ::open
inline int
open(const char *filename, int oflag, int pmode)
{
  return ::_open(filename, oflag, pmode);
}
#endif
