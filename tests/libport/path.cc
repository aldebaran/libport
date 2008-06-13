#include <boost/bind.hpp>
#include <libport/unit-test.hh>
#include <libport/path.hh>


using boost::bind;
using libport::path;
using libport::test_suite;

void path_ctor(const std::string& path, bool valid)
{
  // For some reason, the BOOST_CHECK_* macros are not blocks :(
  // So we need the braces.
  if (valid)
  {
    BOOST_CHECK_NO_THROW(libport::path p(path));
  }
  else
  {
    BOOST_CHECK_THROW(libport::path p(path), path::invalid_path);
  }
}

void path_absolute(const std::string& path, bool expected = true)
{
  BOOST_CHECK(libport::path(path).absolute_get() == expected);
}

void path_name(const std::string& spath,
               const std::string& dir,
               const std::string& base)
{
  path path(spath);
  BOOST_CHECK_EQUAL(path.dirname(), dir);
  BOOST_CHECK_EQUAL(path.basename(), base);
}

void concat(const std::string& lhs,
            const std::string& rhs,
            const std::string& res)
{
  BOOST_CHECK(path(lhs) / path(rhs) == res);
}

void invalid_concat(const std::string& lhs,
                    const std::string& rhs)
{
  BOOST_CHECK_THROW(path(lhs) / path(rhs), path::invalid_path);
}

void exist(const path& p, bool expected = true)
{
  BOOST_CHECK_EQUAL(p.exists(), expected);
}

void equal(const path& lhs, const path& rhs)
{
  BOOST_CHECK_EQUAL(lhs, rhs);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::path test suite");

  // Test constructors
  test_suite* ctor_suite = BOOST_TEST_SUITE("Construction test suite");
  suite->add(ctor_suite);
#define def(path, v)                                                      \
  ctor_suite->add(BOOST_TEST_CASE(bind(path_ctor, path, v)));
  def("urbi.u", true);
  def("foo/bar.cc", true);
  def("/usr/local", true);
  def("", false);
#ifdef WIN32
  def("C:\\Documents and Settings", true);
#endif
#undef def

  // Test absoluteness
  test_suite* abs_suite = BOOST_TEST_SUITE("Absolute path detection test suite");
  suite->add(abs_suite);
#define def(path, abs)                                                          \
  abs_suite->add(BOOST_TEST_CASE(bind(path_absolute, path, abs)));
  def("relative", false);
  def("relative/with/directories", false);
  def("relative/with/trailing/slash/", false);
  def("/", true);
  def("/absolute", true);
  def("/absolute/but/longer", true);
  def("/absolute/with/trailing/slash/", true);
  def("/./still/../absolute", true);
#ifdef WIN32
  def("C:\\", true);
  def("C:\\absolute", true);
  def("C:\\absolute\\but\\on\\stupid\\OS", true);
#endif
#undef def

  // Test equality operator
  test_suite* eq_suite = BOOST_TEST_SUITE("Comparison test suite");
  suite->add(eq_suite);
#define def(lhs, rhs)                                           \
  eq_suite->add(BOOST_TEST_CASE(bind(equal, lhs, rhs)));
  def("foo", "foo");
  def("foo/bar/baz/quux", "foo/bar/baz/quux");
  def("/foo/bar/baz/quux", "/foo/bar/baz/quux");
  def("just/stay", "./just/././stay/./");
  def("in/is/fun", "in/and/out/../../is/fun/too/..");
#undef def

  // Test basename/dirname
  test_suite* name_suite = BOOST_TEST_SUITE("Basename/dirname test suite");
  suite->add(name_suite);
#define def(path, dir, base)                                    \
  name_suite->add(BOOST_TEST_CASE(bind(path_name, path, dir, base)));
  def("dirname/basename", "dirname", "basename");
  def("foo", ".", "foo");
  def("../../bar", "../..", "bar");
  def("/usr/local/gostai", "/usr/local", "gostai");
  // Standard (and somehow logical) GNU coreutils behavior:
  def("/", "/", "/");
  def(".", ".", ".");
#undef def

  // Test concatenation
  test_suite* concat_suite = BOOST_TEST_SUITE("Concatenation test suite");
  suite->add(concat_suite);
#define def(lhs, rhs, res)                                      \
  concat_suite->add(BOOST_TEST_CASE(bind(concat, lhs, rhs, res)));
  def("foo", "bar", "foo/bar");
  def("foo/bar", "baz/quux", "foo/bar/baz/quux");
  def("foo/bar", ".", "foo/bar");
  def("foo/bar", "../..", ".");
  def("/foo", "bar", "/foo/bar");
  def("/", "foo", "/foo");
  def("/", ".", "/");
#undef def
#define def(lhs, rhs)                                           \
  concat_suite->add(BOOST_TEST_CASE(bind(invalid_concat, lhs, rhs)));
  def("/", "/");
  def("foo", "/bar");
#undef def

  test_suite* exist_suite = BOOST_TEST_SUITE("File existence test suite");
  suite->add(exist_suite);
#define def(path, res)                                          \
  exist_suite->add(BOOST_TEST_CASE(bind(exist, path, res)));
  def("/", true);
  def("/this/directory/does/not/exist", false);
#undef def

  return suite;
}
