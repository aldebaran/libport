/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <libport/bind.hh>
using namespace boost::assign;

#include <libport/file-library.hh>
#include <libport/lexical-cast.hh>
#include <libport/unit-test.hh>
using boost::bind;
using libport::file_library;
using libport::test_suite;

#ifdef WIN32
static const std::string root_dir = "C:\\";
#else
static const std::string root_dir = "/";
#endif

template <typename Library>
void ctor(const Library& lib, const char* sep,
          const std::string& _expected)
{
#ifndef WIN32
  std::string expected(_expected);
#else
  std::string expected(boost::replace_all_copy(_expected, "/", "\\"));
#endif
  BOOST_CHECK_NO_THROW(file_library l(lib, sep));
  file_library l(lib, sep);
  BOOST_CHECK_EQUAL(string_cast(l), expected);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::file-library test suite");

  // Test constructors.
  test_suite* ctor_suite = BOOST_TEST_SUITE("Construction test suite");
  suite->add(ctor_suite);
# define def(Path, Sep, Expected)                                       \
  ctor_suite->add(BOOST_TEST_CASE(bind(ctor<std::string>,               \
                                       Path, Sep, Expected)));
  def(root_dir + "foo:"+ root_dir + "bar", ":",
      ".:" + root_dir + "foo:" + root_dir + "bar");
  def(root_dir + "foo:", ":", ".:" + root_dir + "foo");
  def("::" + root_dir + "foo:",   ":", ".:" + root_dir + "foo");
#undef def

#define STRIP_PARENS(S) S

  typedef std::list<std::string> strings_type;
# define def(Path, Sep, Expected)                                       \
  ctor_suite->add(BOOST_TEST_CASE(bind(ctor<strings_type>,              \
                                       list_of Path, Sep, Expected)))

  // First component overrides everything.
  def((root_dir + "outer1:" + root_dir + "outer2")
        (root_dir + "inner1:" + root_dir + "inner2"), ":",
      ".:" + root_dir + "outer1:" + root_dir + "outer2");

  // First component enhances.
  def((root_dir + "outer1::" + root_dir + "outer2")
        (root_dir + "inner1:" + root_dir + "inner2"), ":",
      ".:" + root_dir + "outer1:" + root_dir + "inner1:"
        + root_dir + "inner2:" + root_dir + "outer2");
  def((":" + root_dir + "outer1:" + root_dir + "outer2")
        (root_dir + "inner1:" + root_dir + "inner2"), ":",
      ".:" + root_dir + "inner1:" + root_dir + "inner2:"
      + root_dir + "outer1:" + root_dir + "outer2");
  def((root_dir + "outer1:" + root_dir + "outer2:")
        (root_dir + "inner1:" + root_dir + "inner2"), ":",
      ".:" + root_dir + "outer1:" + root_dir + "outer2:"
        + root_dir + "inner1:" + root_dir + "inner2");

  // First component enhances with superfluous colons.
  def((":" + root_dir + "outer1::" + root_dir + "outer2::")
        (root_dir + "inner1:" + root_dir + "inner2"), ":",
      ".:" + root_dir + "inner1:" + root_dir + "inner2:"
        + root_dir + "outer1:" + root_dir + "outer2");

  // Superfluous colons.
  def((":" + root_dir + "outer1::" + root_dir + "outer2::"), ":",
      ".:" + root_dir + "outer1:" + root_dir + "outer2");

  // Three components.
  def((root_dir + "outer1:" + root_dir + "outer2:")
        (root_dir + "inner1::" + root_dir + "inner2")
        (root_dir + "inmost1::" + root_dir + "inmost2"), ":",
      ".:" + root_dir + "outer1:" + root_dir + "outer2:" + root_dir
        + "inner1:" + root_dir + "inmost1:" + root_dir + "inmost2:"
        + root_dir + "inner2");

  // First are empty.
  def((std::string(""))("")(root_dir + "inmost1::" + root_dir + "inmost2"), ":",
      ".:" + root_dir + "inmost1:" + root_dir + "inmost2");
  def(("")("")(""), ":", ".");
#undef def

  return suite;
}
