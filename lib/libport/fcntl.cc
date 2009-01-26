#include <libport/fcntl.h>

#ifdef WIN32
# include <io.h>

// Wrap ::open
inline int
open(const char *filename, int oflag, int pmode = 0)
{
  return ::_open(filename, oflag, pmode);
}
#endif
