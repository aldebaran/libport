/*
 * Help build the dependencies on Windows.
 *
 * Compile with: gcc -O -o dephelper.so -fpic dephelper.c
 *
 * Use as: URBI_DEPFILE=xxx LD_PRELOAD=dephelper.so winserver ...
 *
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#define _FCNTL_H
#include <bits/fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define URBI_DEPFILE "URBI_DEPFILE"

static int (*_open)(const char* pathname, int flags, ...);
static int (*_open64)(const char* pathname, int flags, ...);

static int
ends_with(const char* pathname, const char* suffix)
{
  return strlen(pathname) >= strlen(suffix) &&
         !strcmp(pathname + strlen(pathname) - strlen(suffix), suffix);
}

static int
interesting_file(const char* pathname)
{
  return ends_with(pathname, "c") ||
         ends_with(pathname, "cc") ||
	 ends_with(pathname, "h") ||
	 ends_with(pathname, "hh") ||
	 ends_with(pathname, "hxx");
}

static void
setup_opens()
{
  if (!_open)
    _open = (int (*)(const char* pathname, int flags, ...)) dlsym(RTLD_NEXT, "open");
  if (!_open64)
    _open64 = (int (*)(const char* pathname, int flags, ...)) dlsym(RTLD_NEXT, "open64");
}

static const char*
env_value(const char* name)
{
  const char* res = getenv(name);
  if (!res)
    abort();
  return res;
}

static void
append_to_log(const char* pathname)
{
  if (!interesting_file(pathname))
    return;
  char* unix_pathname = index(pathname, ':');
  if (unix_pathname)
    unix_pathname++;
  else
    unix_pathname = (char*)pathname;
  int log = _open(env_value(URBI_DEPFILE), O_WRONLY | O_CREAT | O_APPEND, 0666);
  if (log < 0)
    abort();
  write(log, unix_pathname, strlen(unix_pathname));
  write(log, "\n", 1);
  close(log);
}

int
open(const char* pathname, int flags, mode_t mode)
{
  setup_opens();
  if (!(flags & O_WRONLY))
    append_to_log(pathname);
  return _open(pathname, flags, mode);
}

int
open64(const char* pathname, int flags, mode_t mode)
{
  setup_opens();
  if (!(flags & O_WRONLY))
    append_to_log(pathname);
  return _open64(pathname, flags, mode);
}
