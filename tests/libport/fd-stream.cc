#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <libport/unit-test.hh>
#include <libport/fd-stream.hh>


using boost::bind;
using libport::FdStream;
using libport::test_suite;

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

  assert(!pipe(fd));
  assert(!fstat(fd[0], &stat));
  assert(!fstat(fd[1], &stat));

  {
    FdStream s(fd[1], fd[0]);
    s.own_fd(own);
  }
  if (own)
  {
    BOOST_CHECK(fstat(fd[0], &stat));
    BOOST_CHECK(fstat(fd[1], &stat));
  }
  else
  {
    BOOST_CHECK(!fstat(fd[0], &stat));
    BOOST_CHECK(!fstat(fd[1], &stat));
    close(fd[0]);
    close(fd[1]);
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
  assert(write(fd, str.c_str(), str.length()) == str.length());
}

static void
put(char c, int fd)
{
  std::string s;
  s += c;
  put(s, fd);
}

static std::string
get(int fd)
{
  char buf[BUFSIZ + 1];
  int c;

  c = read(fd, buf, sizeof (buf) - 1);
  buf[c] = 0;
  return buf;
}

static void
fd_read_string(FdStream*& stream, int fd)
{
  std::string test;

  put("test", fd);
  close(fd);

  *stream >> test;
  BOOST_CHECK_EQUAL(test, "test");
}

static void
fd_write_string(FdStream*& stream, int fd)
{
  static const std::string test = "test";

  *stream << test;
  stream->sync();
  BOOST_CHECK_EQUAL(get(fd), test);
}

static void
fd_read_ints(FdStream*& stream, int fd)
{
  int test;

  // Write
  put("42 ", fd);
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 42);
  put("51 69", fd);
  close(fd);
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 51);
  *stream >> test;
  BOOST_CHECK_EQUAL(test, 69);
}

static void
fd_write_ints(FdStream*& stream, int fd)
{
  static const std::string test = "test";

  *stream << std::hex << 42 << ' ';
  *stream << std::oct << 42;
  delete stream;
  stream = 0;
  BOOST_CHECK_EQUAL(get(fd), "2a 52");
}

static void
fd_read_jumbo(FdStream*& stream, int fd)
{
  static const int size = BUFSIZ * 1.5;

  for (int i = 0; i < size; ++i)
    put(i % 256, fd); // This might fail if the pipe isn't large enough ...
  close(fd);

  char c;
  int count = 0;
  for (count = 0; stream->get(c); ++count)
    BOOST_CHECK_EQUAL(c, (char)(count % 256));
  BOOST_CHECK_EQUAL(count, size);
}

static void
fd_write_jumbo(FdStream*& stream, int fd)
{
  static const int size = BUFSIZ * 1.5;

  for (int i = 0; i < size; ++i)
    *stream << (char) (i % 256);
  stream->own_fd(true);
  delete stream;
  stream = 0;
  char c;
  for (int i = 0; i < size; ++i)
  {
    BOOST_CHECK(read(fd, &c, 1) == 1);
    BOOST_CHECK_EQUAL(c, (char)i % 256);
  }
  BOOST_CHECK_EQUAL(read(fd, &c, 1), 0);
}

typedef boost::function2<void, FdStream*&, int> f_type;

static void
fd_read(f_type action)
{
  int fd[2];
  assert(!pipe(fd));

  FdStream* stream = new FdStream(STDOUT_FILENO, fd[0]);
  action(stream, fd[1]);

  close(fd[0]);
  if (stream)
    delete stream;
}

static void
fd_write(f_type action)
{
  int fd[2];
  assert(!pipe(fd));

  FdStream* stream = new FdStream(fd[1], STDIN_FILENO);
  action(stream, fd[0]);

  close(fd[1]);
  if (stream)
    delete stream;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::FdStream test suite");
  test_suite* current;

#define SUITE(Desc)                                                     \
  current = BOOST_TEST_SUITE("Construction test suite");                \
  suite->add(current);                                                  \

#define TEST(Args)                                                      \
  current->add(BOOST_TEST_CASE                                          \
               (boost::bind Args));                                     \

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
  TEST((fd_write, fd_write_string));
  TEST((fd_write, fd_write_ints));
  TEST((fd_write, fd_write_jumbo));

  return suite;
}
