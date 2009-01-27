#include <limits>
#include <libport/ufloat.h>

#include <libport/unit-test.hh>

using libport::test_suite;

static void test_comparison()
{
  ufloat f = 1.0;
  ufloat g = 10.0;

  BOOST_CHECK_LT(f, g);
  BOOST_CHECK_GT(g, f);

  BOOST_CHECK_LE(f, g);
  BOOST_CHECK_GE(g, f);

  BOOST_CHECK_NE(f, g);

  BOOST_CHECK_EQUAL(f, f);
  BOOST_CHECK_LE(f, f);
  BOOST_CHECK_GE(f, f);
}


# define CHECK_TO_AND_FRO(Value)                                \
  do {                                                          \
    T value = Value;                                            \
    BOOST_TEST_MESSAGE("Checking " #Value " = " << value);      \
    BOOST_CHECK_EQUAL(libport::numeric_cast<T>(ufloat(value)),  \
                      value);                                   \
  } while (0)

# define CHECK_LIMIT(Name)                                     \
  CHECK_TO_AND_FRO(std::numeric_limits<T>::Name())


template <typename T>
static void test_max()
{
  CHECK_LIMIT(max);
}

template <typename T>
static void test_unsigned_range()
{
  CHECK_LIMIT(epsilon);
  CHECK_LIMIT(min);
  CHECK_TO_AND_FRO(0.0);
  CHECK_TO_AND_FRO(1.0);
  CHECK_TO_AND_FRO(10000.0);
}

template <typename T>
static void test_signed_range()
{
  test_unsigned_range<T>();
  CHECK_TO_AND_FRO(-1.0);
  CHECK_TO_AND_FRO(-10000.0);
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::ufloat");
  suite->add(BOOST_TEST_CASE(test_comparison));

  suite->add(BOOST_TEST_CASE(test_signed_range<int>));
  suite->add(BOOST_TEST_CASE(test_max<int>));

  suite->add(BOOST_TEST_CASE(test_unsigned_range<unsigned int>));
  suite->add(BOOST_TEST_CASE(test_max<unsigned int>));

  suite->add(BOOST_TEST_CASE(test_signed_range<long>));
  suite->add(BOOST_TEST_CASE(test_max<long>));

  suite->add(BOOST_TEST_CASE(test_unsigned_range<unsigned long>));
  suite->add(BOOST_TEST_CASE(test_max<unsigned long>));

  suite->add(BOOST_TEST_CASE(test_signed_range<long long>));

  suite->add(BOOST_TEST_CASE(test_unsigned_range<unsigned long long>));

  // We can't represent these values in doubles.
  // suite->add(BOOST_TEST_CASE(test_max<long long>));
  // suite->add(BOOST_TEST_CASE(test_max<unsigned long long>));

  return suite;
}
