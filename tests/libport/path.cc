/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include "test.hh"
#include <libport/path.hh>

using boost::bind;
using libport::path;
using libport::test_suite;

static const std::string separator = WIN32_IF("\\", "/");

// Join path components.
static
std::string
operator/(const std::string& lhs, const std::string& rhs)
{
  return (lhs.empty() ? rhs
          : rhs.empty() ? lhs
          : lhs + separator + rhs);
}

void path_ctor(const std::string& path,
               const std::string& WIN32_IF(volume, /* nothing */),
               bool valid)
{
  if (valid)
  {
    BOOST_CHECK_NO_THROW(libport::path p(path));
#ifdef WIN32
    libport::path p(path);
    BOOST_CHECK_EQUAL(p.volume_get(), volume);
#endif
  }
  else
    BOOST_CHECK_THROW(libport::path p(path), path::invalid_path);
}

void path_absolute(const std::string& path,
                   bool expected = true)
{
  libport::path p(path);
  BOOST_CHECK_EQUAL(p.absolute_get(), expected);
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

void create(const path& p)
{
  // If the file exists, remove it.
  if (p.exists())
    BOOST_CHECK_NO_THROW(p.remove());
  exist(p, false);
  p.create();
  exist(p, true);
  BOOST_CHECK_NO_THROW(p.remove());
  exist(p, false);
}

void temporary_files(void)
{
  path temp = path::temporary_file();
  exist(temp, true);
  BOOST_CHECK_NO_THROW(temp.remove());
  exist(temp, false);
}

void components(const path& p, const libport::path::path_type& expected)
{
  BOOST_CHECK(p.components() == expected);
}

void parent(const path& p, const path& expected)
{
  BOOST_CHECK(p == expected);
}

void is_root(const path& p, bool expected)
{
  BOOST_CHECK(p.is_root() == expected);
}

void equal(const path& lhs, const path& rhs)
{
  BOOST_CHECK_EQUAL(lhs, rhs);
}

void not_equal(const path& lhs, const path& rhs)
{
  BOOST_CHECK_NE(lhs, rhs);
}

void to_string(const path& in, const std::string& out)
{
  BOOST_CHECK_EQUAL(in.to_string(), out);
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::path test suite");

  // Test constructors
  test_suite* ctor_suite = BOOST_TEST_SUITE("Construction test suite");
  suite->add(ctor_suite);
# define def(Path, Vol, Valid)                                            \
  ctor_suite->add(BOOST_TEST_CASE(bind(path_ctor, Path, Vol, Valid)));
  def("urbi.u", "", true);
  def(std::string("foo")/"bar.cc", "", true);
  def("", "", false);
#ifndef WIN32
  def("/usr/local", "", true);
#else
  def("C:\\Documents and Settings", "C:", true);
  def("c:", "c:", true);
  def("\\\\shared_volume\\subdir", "\\\\shared_volume", true);
#endif
#undef def

  // Test absoluteness
  test_suite* abs_suite =
    BOOST_TEST_SUITE("Absolute path detection test suite");
  suite->add(abs_suite);
#define def(Path, Abs)                                                  \
  abs_suite->add(BOOST_TEST_CASE(bind(path_absolute, Path, Abs)));
  def("relative", false);
  def(std::string("relative")/"with"/"directories", false);
  def(std::string("relative")/"with"/"trailing"/"slash"/"", false);
#ifndef WIN32
  def("/", true);
  def("/absolute", true);
  def("/absolute/but/longer", true);
  def("/absolute/with/trailing/slash/", true);
  def("/./still/../absolute", true);
#else
  def("c:", false);
  def("C:", false);
  def("C:\\", true);
  def("C:\\absolute", true);
  def("C:\\absolute\\but\\on\\stupid\\OS", true);
  def("\\\\shared_volume\\subdir", true);
#endif
#undef def

  // Test printing.
  test_suite* to_string_suite = BOOST_TEST_SUITE("to_string");
  suite->add(to_string_suite);
# define def(In)                                      \
  to_string_suite->add(BOOST_TEST_CASE(bind(to_string, In, In)));
#ifdef WIN32
  def("C:\\Documents and Settings");
  def("c:");
  def("\\\\shared_volume\\subdir");
  // def("\\");
#else
  def(".");
  def("/");
  def("urbi.u");
  def("foo/bar.cc");
  def("/usr/local");
#endif
#undef def

  // Test equality operator
  test_suite* eq_suite = BOOST_TEST_SUITE("Comparison test suite");
  suite->add(eq_suite);
#define def(lhs, rhs)                                           \
  eq_suite->add(BOOST_TEST_CASE(bind(equal, lhs, rhs)));
  def("foo", "foo");
  def(std::string("foo")/"bar"/"baz"/"quux",
      std::string("foo")/"bar"/"baz"/"quux");
#ifndef WIN32
  def("/foo/bar/baz/quux", "/foo/bar/baz/quux");
#endif
  def(std::string("just")/"stay",
      std::string(".")/"just"/"."/"."/"stay"/"."/"");
  def(std::string(".")/""/""/""/""/"."/""/""/"foo"/"", "foo");
  def("foo", std::string("foo")/"");
//  def("in/is/fun",
//      "in/and/out/../../is/fun/too/..");
  def(std::string("in")/"and"/"out"/".."/".."/"is"/"fun"/"too"/"..",
      std::string("in")/"and"/"out"/".."/".."/"is"/"fun"/"too"/"..");
#undef def
#ifdef WIN32
#define def(lhs, rhs)                                           \
  eq_suite->add(BOOST_TEST_CASE(bind(not_equal, lhs, rhs)));
  def("foo", "\\\\foo");
#undef def
#endif // WIN32

  // Test basename/dirname
  test_suite* name_suite = BOOST_TEST_SUITE("Basename/dirname test suite");
  suite->add(name_suite);
#define def(path, dir, base)                                    \
  name_suite->add(BOOST_TEST_CASE(bind(path_name, path, dir, base)));
  def(std::string("dirname")/"basename", "dirname", "basename");
  def("foo", ".", "foo");
  def(std::string("..")/".."/"bar", std::string("..")/"..", "bar");
  // Standard (and somehow logical) GNU coreutils behavior:
#ifndef WIN32
  def("/usr/local/gostai", "/usr/local", "gostai");
  def("/", "/", "/");
#else
  def("\\\\share\\foo\\bar\\", "\\\\share\\foo", "bar");
#endif
  def(".", ".", ".");
#undef def

  // Test concatenation
  test_suite* concat_suite = BOOST_TEST_SUITE("Concatenation test suite");
  suite->add(concat_suite);
#define def(lhs, rhs, res)                                      \
  concat_suite->add(BOOST_TEST_CASE(bind(concat, lhs, rhs, res)));
  def("foo", "bar", std::string("foo")/"bar");
  def(std::string("foo")/"bar",
      std::string("baz")/"quux",
      std::string("foo")/"bar"/"baz"/"quux");
  def(std::string("foo")/"bar", ".",
      std::string("foo")/"bar");
//  def("foo/bar", "../..", ".");
  def(std::string("foo")/"bar", std::string("..")/"..",
      std::string("foo")/"bar"/".."/"..");
#ifndef WIN32
  def("/foo", "bar", "/foo/bar");
  def("/", "foo", "/foo");
  def("/", ".", "/");
#else
  def("\\\\share\\foo", "bar", "\\\\share\\foo\\bar");
  def("c:\\", "foo", "c:\\foo");
  def("c:", "bar", "c:bar");
#endif
#undef def
#define def(lhs, rhs)                                           \
  concat_suite->add(BOOST_TEST_CASE(bind(invalid_concat, lhs, rhs)));
  def(std::string("")/"", std::string("")/"");
#ifndef WIN32
  def("foo", "/bar");
#else
  def("foo", "C:\\\\bar");
  def("\\\\share\\foo", "\\\\share\\bar");
  def("c:", "d:");
#endif
#undef def

  test_suite* exist_suite = BOOST_TEST_SUITE("File existence test suite");
  suite->add(exist_suite);
#define def(path, res)                                          \
  exist_suite->add(BOOST_TEST_CASE(bind(exist, path, res)));
#ifndef WIN32
  def("/", true);
  def("/this/directory/does/not/exist", false);
#else
  def("C:\\", true);
  def("C:\\this\\directory\\does\\not\\exist", false);
#endif
#undef def

  test_suite* create_suite = BOOST_TEST_SUITE("File creation test suite");
  suite->add(create_suite);
#define def(path)                                          \
  create_suite->add(BOOST_TEST_CASE(bind(create, path)));
  def(std::string("tests")/"libport"/"creation_test");
#undef def
#define def()                                              \
  create_suite->add(BOOST_TEST_CASE(temporary_files))
  def();
#undef def

  // Components tests
  test_suite* components_suite = BOOST_TEST_SUITE("Components test suite");
  suite->add(components_suite);
#define def(path, expected)                                \
  components_suite->add(BOOST_TEST_CASE(bind(components, path, expected)))
  const char* comp[] = {"base", "foo", "bar"};
  libport::path::path_type expected(comp, comp+3);
  def("/base/foo/bar", expected);
#undef def

  // Test parent and is_root
  test_suite* parent_suite = BOOST_TEST_SUITE("Parent test suite");
  suite->add(parent_suite);

#define def(p, e)                                                      \
  parent_suite->add(BOOST_TEST_CASE(bind(parent,                       \
                                         libport::path(p).parent(),    \
                                         libport::path(e))))

  def("foo/bar/baz/qux", "foo/bar/baz");
  def("foo/bar/baz",     "foo/bar");
  def("foo/bar",         "foo");
  def("foo",             libport::path::cwd());
#undef def

#define def(p, e)                                                      \
  parent_suite->add(BOOST_TEST_CASE(bind(is_root,                      \
                                         libport::path(p),             \
                                         e)))
  def("foo",  false);
  def("/foo", false);
  def("/",    true);
#undef def

  return suite;
}
