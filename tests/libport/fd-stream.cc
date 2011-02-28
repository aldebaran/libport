/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/sys/stat.h>
#include <libport/sys/types.h>

#include <libport/cstdlib>

#include <libport/bind.hh>
#include <boost/function.hpp>

#include <libport/cstdio>
#include <libport/fd-stream.hh>
#include <libport/thread.hh>
#include <libport/unit-test.hh>
#include <libport/unistd.h>


using boost::bind;
using libport::FdStream;
using libport::test_suite;

// /!\ Boost check is not thread-safe, so it cannot be used everywhere.
//     This is why we have some "abort()" remaining in this file.

static void
ctor(int write, int read)
{
  BOOST_CHECK_NO_THROW(FdStream(write, read));
}

static void
own(bool own)
{
  int fd[2];
  struct stat stat;

  BOOST_CHECK(!pipe(fd));
  BOOST_CHECK(!fstat(fd[0], &stat));
  BOOST_CHECK(!fstat(fd[1], &stat));

  {
    FdStream s(fd[1], fd[0]);
    s.own_fd(own);
  }
  if (own)
  {
#ifndef WIN32
    // Fstat on a closed fd implodes under WIN32
    BOOST_CHECK(fstat(fd[0], &stat));
    BOOST_CHECK(fstat(fd[1], &stat));
#endif
  }
  else
  {
    BOOST_CHECK(!fstat(fd[0], &stat));
    BOOST_CHECK(!fstat(fd[1], &stat));
    BOOST_CHECK(!close(fd[0]));
    BOOST_CHECK(!close(fd[1]));
  }
}

static void
fd_get()
{
  FdStream s(42, 51);
  BOOST_CHECK_EQUAL(s.fd_write(), 42);
  BOOST_CHECK_EQUAL(s.fd_read(), 51);
}

static void
put(const std::string& str, int fd)
{
  BOOST_CHECK_EQUAL(unsigned(write(fd, str.c_str(), str.length())),
                    str.length());
}

static void
put(char c, int fd)
{
  if (write(fd, &c, 1) != 1)
    abort();
}

static std::string
get(int fd)
{
  char buf[LIBPORT_BUFSIZ];
  return std::string(buf, read(fd, buf, sizeof buf));
}

static void
fd_read_string(FdStream*& stream, int fd)
{
  put("test", fd);
  BOOST_CHECK(!close(fd));

  std::string test;
  *stream >> test;
  BOOST_CHECK_EQUAL(test, "test");
}

static void
fd_write_string(FdStream*& stream, int fd)
{
  const std::string test = "test";

  *stream << test;
  stream->sync();
  BOOST_CHECK_EQUAL(get(fd), test);
}

static void
fd_read_ints(FdStream*& stream, int fd)
{
  // Write
  put("42 ", fd);
  int test;
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 42);
  put("51 69", fd);
  BOOST_CHECK(!close(fd));
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 51);
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 69);
}

static void
fd_write_ints(FdStream*& stream, int fd)
{
  const std::string test = "test";

  *stream << std::hex << 42 << ' ';
  *stream << std::oct << 42;
  delete stream;
  stream = 0;
  BOOST_CHECK_EQUAL(get(fd), "2a 52");
}

static void thread_write(int fd, int size)
{
  for (int i = 0; i < size; ++i)
    // This might fail if the pipe isn't large enough ...
    put(i % 256, fd);
  if (close(fd))
    abort();
}

static void
fd_read_jumbo(FdStream*& stream, int fd)
{
  static const int size = LIBPORT_BUFSIZ * 3 / 2;

  pthread_t thread = libport::startThread(boost::bind(thread_write, fd, size));

  char c;
  int count = 0;
  for (count = 0; stream->get(c); ++count)
    BOOST_CHECK_EQUAL(c, (char)(count % 256));
  BOOST_CHECK(stream->eof());
  BOOST_CHECK_EQUAL(count, size);
// This join hangs forever with QEmu, altough the thread _has_ finished.
// Either a problem with libport::thread, either a QEmu pthread implementation
// bug. Joining the thread is not an absolute necessity anyway.
//  libport::joinThread(thread);
  LIBPORT_USE(thread);
}

static void
thread_write_stream(FdStream* stream, int size)
{
  for (int i = 0; i < size; ++i)
    *stream << (char) (i % 256);
  delete stream;
}

static void
fd_write_jumbo(FdStream*& stream, int fd)
{
  static const int size = LIBPORT_BUFSIZ * 3 / 2;

  stream->own_fd(true);
  libport::startThread(boost::bind(thread_write_stream, stream, size));
  stream = 0;
  char c;
  for (int i = 0; i < size; ++i)
  {
    BOOST_CHECK_EQUAL(read(fd, &c, 1), 1);
    BOOST_CHECK_EQUAL(c, (char)i % 256);
  }
  BOOST_CHECK_EQUAL(read(fd, &c, 1), 0);
}

typedef boost::function2<void, FdStream*&, int> f_type;

static void
fd_read(f_type action)
{
  int fd[2];
  BOOST_CHECK(!pipe(fd));

  FdStream* stream = new FdStream(STDOUT_FILENO, fd[0]);
  action(stream, fd[1]);

  BOOST_CHECK(!close(fd[0]));
  delete stream;
}

static void
fd_write(f_type action, bool close_p)
{
  int fd[2];
  BOOST_CHECK(!pipe(fd));

  FdStream* stream = new FdStream(fd[1], STDIN_FILENO);
  action(stream, fd[0]);

  // Don't close twice the fd, it throws on Windows.  In the
  // fd_write_jumbo case, the stream closes this fd.  The others
  // don't.  There is no simple means to guarantee a single close for
  // each case, so just don't close at all.
  if (close_p)
    BOOST_CHECK(!close(fd[1]));
  delete stream;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::FdStream test suite");
  test_suite* current;

#define SUITE(Desc)                             \
  current = BOOST_TEST_SUITE(Desc);             \
  suite->add(current);

#define TEST(Args)                                      \
  current->add(BOOST_TEST_CASE(boost::bind Args));

  SUITE("Construction test suite");
  TEST((ctor, 0, 0));

  SUITE("fd_own test suite");
  TEST((own, true));
  TEST((own, false));

  SUITE("fd_write and fd_read test suite");
  TEST((fd_get));

  SUITE("read test suite");
  TEST((fd_read, fd_read_string));
  TEST((fd_read, fd_read_ints));
  TEST((fd_read, fd_read_jumbo));

  SUITE("write test suite");
  TEST((fd_write, fd_write_string, true));
  TEST((fd_write, fd_write_ints, true));
  TEST((fd_write, fd_write_jumbo, false));

  return suite;
}
