/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <limits>
#include <libport/ufloat.hh>
#include <libport/tr1/type_traits>
#include <libport/unit-test.hh>
#include <iostream>

using libport::test_suite;
using libport::ufloat;

static void check_comparison()
{
  ufloat f = 1;
  ufloat g = 10;

  BOOST_CHECK_LT(f, g);
  BOOST_CHECK_GT(g, f);

  BOOST_CHECK_LE(f, g);
  BOOST_CHECK_GE(g, f);

  BOOST_CHECK_NE(f, g);

  BOOST_CHECK_EQUAL(f, f);
  BOOST_CHECK_LE(f, f);
  BOOST_CHECK_GE(f, f);
}


/*---------------.
| numeric_cast.  |
`---------------*/

#define CHECK_TO_AND_FRO(Value)                                 \
  do {                                                          \
    T value = Value;                                            \
    BOOST_TEST_MESSAGE("Checking " #Value " = " << value);      \
    BOOST_CHECK_EQUAL(libport::numeric_cast<T>(ufloat(value)),  \
                      value);                                   \
  } while (0)

#define CHECK_LIMIT(Name)                               \
  CHECK_TO_AND_FRO(std::numeric_limits<T>::Name())

template <typename T>
static void check_max()
{
  CHECK_LIMIT(max);
}

template <typename T>
static void check_unsigned_range()
{
  CHECK_LIMIT(epsilon);
  CHECK_LIMIT(min);
  CHECK_TO_AND_FRO(0);
  CHECK_TO_AND_FRO(1);
  CHECK_TO_AND_FRO(100);
}

template <typename T>
static void check_signed_range()
{
  check_unsigned_range<T>();
  CHECK_TO_AND_FRO(-1);
  CHECK_TO_AND_FRO(-100);
}

#undef CHECK_LIMIT
#undef CHECK_TO_AND_FRO

template <typename T>
static void check_rounding_cast()
{
#define CHECK_(In, Out)                                          \
  BOOST_CHECK_EQUAL(libport::rounding_cast<T>(ufloat(In)),  Out)
#define CHECK(In, Out)                          \
  do {                                          \
    if (std::tr1::is_unsigned<T>::value)        \
      CHECK_(In, Out##u);                       \
    else                                        \
    {                                           \
      CHECK_( In,  Out);                        \
      CHECK_(-In, -Out);                        \
    }                                           \
  } while (false)
  CHECK(0.0,     0);
  CHECK(0.4999,  0);
  CHECK(0.5,     0);
  CHECK(0.50001, 1);
  CHECK(0.9999,  1);
  CHECK(1.0,     1);
  CHECK(1.4,     1);
  CHECK(1.45,    1);
  CHECK(1.49999, 1);
  CHECK(1.5,     2);
  CHECK(1.50001, 2);
  // Does not with too long types, where the precision is lost in
  // ufloat.
  if (sizeof(T) <= 4)
  {
    CHECK_(std::numeric_limits<T>::min(), std::numeric_limits<T>::min());
    CHECK_(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
  }
#undef CHECK
#undef CHECK_
}




/*-------------------.
| numeric_castable.  |
`-------------------*/

static void
check_castable()
{
  BOOST_CHECK( libport::numeric_castable<int>(65536.0));
  BOOST_CHECK(!libport::numeric_castable<int>(65536.1));

  BOOST_CHECK(!libport::numeric_castable<int>(4294967296.0));

  // Does not work on 32b machines where int == long.
  if (4 <= sizeof(long))
  {
    BOOST_CHECK( libport::numeric_castable<long>(4294967296.0));
    BOOST_CHECK(!libport::numeric_castable<long>(4294967296.1));

    BOOST_CHECK( libport::numeric_castable<long>( 21474836470.0));
    BOOST_CHECK( libport::numeric_castable<long>(-21474836470.0));
    BOOST_CHECK(!libport::numeric_castable<long>( 21474836470.1));
  }

  // These do not fit into doubles.
  // // std::numeric_limits<long long>::max()
  // BOOST_CHECK( libport::numeric_castable<long long>(9223372036854775807.0));
  // BOOST_CHECK(!libport::numeric_castable<long long>(9223372036854775807.1));
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::ufloat");
  suite->add(BOOST_TEST_CASE(check_comparison));
  suite->add(BOOST_TEST_CASE(check_castable));

  // The following use of "&" are required to please MSVC.  Otherwise
  // at runtime it just dies:
  //
  // unknown location(0): fatal error in "check_signed_range<int>":
  // memory access violation
#define CHECK(Type)                                             \
  suite->add(BOOST_TEST_CASE(&check_signed_range<Type>));       \
  suite->add(BOOST_TEST_CASE(&check_max<Type>));                \
  suite->add(BOOST_TEST_CASE(&check_rounding_cast<Type>));

  CHECK(char);
  CHECK(unsigned char);
  CHECK(short);
  CHECK(unsigned short);
  CHECK(int);
  CHECK(unsigned int);
#undef CHECK

  // long.
  suite->add(BOOST_TEST_CASE(&check_signed_range<long>));
  suite->add(BOOST_TEST_CASE(&check_rounding_cast<long>));
  // On 64 bits, long is like long long, see below...
#if !(defined LIBPORT_URBI_UFLOAT_DOUBLE && _LP64)
  suite->add(BOOST_TEST_CASE(&check_max<long>));
#endif

  // unsigned long.
  suite->add(BOOST_TEST_CASE(&check_unsigned_range<unsigned long>));
  suite->add(BOOST_TEST_CASE(&check_rounding_cast<unsigned long>));
#if !(defined LIBPORT_URBI_UFLOAT_DOUBLE && _LP64)
  suite->add(BOOST_TEST_CASE(&check_max<unsigned long>));
#endif

  // long long.
  suite->add(BOOST_TEST_CASE(&check_signed_range<long long>));
  suite->add(BOOST_TEST_CASE(&check_rounding_cast<long long>));

  // unsigned long long.
  suite->add(BOOST_TEST_CASE(&check_unsigned_range<unsigned long long>));
  suite->add(BOOST_TEST_CASE(&check_rounding_cast<unsigned long long>));

  // We can't represent these values in doubles.
  // suite->add(BOOST_TEST_CASE(check_max<long long>));
  // suite->add(BOOST_TEST_CASE(check_max<unsigned long long>));

  return suite;
}
