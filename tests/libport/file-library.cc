/*
 * Copyright (C) 2009, Gostai S.A.S.
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
  def("/foo:/bar", ":", ".:/foo:/bar");
  def("/foo:",     ":", ".:/foo");
  def("::/foo:",   ":", ".:/foo");
#undef def

#define STRIP_PARENS(S) S

  typedef std::list<std::string> strings_type;
# define def(Path, Sep, Expected)                                       \
  ctor_suite->add(BOOST_TEST_CASE(bind(ctor<strings_type>,              \
                                       list_of Path, Sep, Expected)))

  // First component overrides everything.
  def(("/outer1:/outer2")("/inner1:/inner2"), ":",
      ".:/outer1:/outer2");

  // First component enhances.
  def(("/outer1::/outer2")("/inner1:/inner2"), ":",
      ".:/outer1:/inner1:/inner2:/outer2");
  def((":/outer1:/outer2")("/inner1:/inner2"), ":",
      ".:/inner1:/inner2:/outer1:/outer2");
  def(("/outer1:/outer2:")("/inner1:/inner2"), ":",
      ".:/outer1:/outer2:/inner1:/inner2");

  // First component enhances with superfluous colons.
  def((":/outer1::/outer2::")("/inner1:/inner2"), ":",
      ".:/inner1:/inner2:/outer1:/outer2");

  // Superfluous colons.
  def((":/outer1::/outer2::"), ":",
      ".:/outer1:/outer2");

  // Three components.
  def(("/outer1:/outer2:")("/inner1::/inner2")("/inmost1::/inmost2"), ":",
      ".:/outer1:/outer2:/inner1:/inmost1:/inmost2:/inner2");

  // First are empty.
  def(("")("")("/inmost1::/inmost2"), ":",
      ".:/inmost1:/inmost2");
  def(("")("")(""), ":",
      ".");
#undef def

  return suite;
}
