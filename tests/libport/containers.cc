/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/containers.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void
check_operator_lt_lt()
{
  typedef std::vector<std::string> string_list;
  string_list s1, s2;

  s1 << "a" << "b";
  BOOST_CHECK_EQUAL(s1.size(), 2u);
  BOOST_CHECK_EQUAL(s1[0], "a");
  BOOST_CHECK_EQUAL(s1[1], "b");

  s2 << "c" << "d";
  s1 << s2;
  BOOST_CHECK_EQUAL(s1.size(), 4u);
  BOOST_CHECK_EQUAL(s1[2], "c");
  BOOST_CHECK_EQUAL(s1[3], "d");

  typedef std::set<std::string> string_set;
  string_set s3;
  s3 << s1;
  BOOST_CHECK_EQUAL(s3.size(), 4u);
  s3 << s1;
  BOOST_CHECK_EQUAL(s3.size(), 4u);

  typedef boost::unordered_set<std::string> boost_string_set;
  boost_string_set s4;
  BOOST_CHECK_EQUAL(s4.size(), 0u);
  s4 << "a";
  s4 << "b";
  BOOST_CHECK_EQUAL(s4.size(), 2u);
  s4 << "b";
  BOOST_CHECK_EQUAL(s4.size(), 2u);
}

class logging_string: public std::string
{
public:
  logging_string(const char* v)
    : std::string(v)
  {}
};

unsigned logging_string_comparisons = 0;
bool operator == (const logging_string& lhs, const logging_string& rhs)
{
  ++logging_string_comparisons;
  return std::operator == (reinterpret_cast<const std::string&>(lhs),
                           reinterpret_cast<const std::string&>(rhs));
}

static void check_find_vector()
{
  typedef std::vector<logging_string> container_type;

  logging_string a("a");
  logging_string b("b");
  logging_string c("c");

  container_type cont;
  cont << a << b << c;
  BOOST_CHECK_EQUAL(cont.size(), 3u);
  logging_string_comparisons = 0;
  container_type::iterator it(libport::find(cont, logging_string("b")));
  BOOST_CHECK(it == cont.begin() + 1);
  BOOST_CHECK_EQUAL(logging_string_comparisons, 2u);
  logging_string_comparisons = 0;
  container_type::iterator end(libport::find(cont, logging_string("d")));
  BOOST_CHECK(end == cont.end());
  BOOST_CHECK_EQUAL(logging_string_comparisons, 3u);
}

static void check_find_unordered_set()
{
  typedef boost::unordered_set<logging_string> container_type;

  logging_string a("a");
  logging_string b("b");
  logging_string c("c");

  container_type cont;
  cont << a << b << c;
  BOOST_CHECK_EQUAL(cont.size(), 3u);
  logging_string_comparisons = 0;
  container_type::iterator it(libport::find(cont, logging_string("b")));
  BOOST_CHECK(it != cont.end());
  BOOST_CHECK_EQUAL(logging_string_comparisons, 1u);
  logging_string_comparisons = 0;
  container_type::iterator end(libport::find(cont, logging_string("d")));
  BOOST_CHECK(end == cont.end());
  BOOST_CHECK_EQUAL(logging_string_comparisons, 0u);
}

static void check_find_std_set()
{
  typedef std::set<logging_string> container_type;

  logging_string a("a");
  logging_string b("b");
  logging_string c("c");
  logging_string d("d");
  logging_string e("e");

  container_type cont;
  cont << a << b << c << d << e;
  BOOST_CHECK_EQUAL(cont.size(), 5u);
  logging_string_comparisons = 0;
  container_type::iterator it(libport::find(cont, logging_string("e")));
  BOOST_CHECK(it != cont.end());
  BOOST_CHECK_LT(logging_string_comparisons, 5u);
  logging_string_comparisons = 0;
  container_type::iterator end(libport::find(cont, logging_string("f")));
  BOOST_CHECK(end == cont.end());
  BOOST_CHECK_LT(logging_string_comparisons, 5u);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport.containers test suite");
  suite->add(BOOST_TEST_CASE(check_operator_lt_lt));
  suite->add(BOOST_TEST_CASE(check_find_vector));
  suite->add(BOOST_TEST_CASE(check_find_unordered_set));
  suite->add(BOOST_TEST_CASE(check_find_std_set));
  return suite;
}
