/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/foreach.hh>
#include <libport/vector.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

#define TEMPLATE                                \
  template <int I,                              \
            typename T,                         \
            typename AllocationPolicy,          \
            typename ConstructionPolicy,        \
            typename CapacityPolicy>            \

#define TYPEDEF                                         \
  typedef libport::Vector<T,                            \
                          AllocationPolicy,             \
                          ConstructionPolicy,           \
                          CapacityPolicy> Vector        \

class Content
{
  public:
    Content(int i_)
      : i(i_)
    {
      ++count;
    }

    Content(const Content& model)
      : i(model.i)
    {
      ++count;
    }

    ~Content()
    {
      --count;
    }

    static int count;
    int i;

    static void before()
    {
      BOOST_CHECK_EQUAL(count, 0);
    }

    static void after()
    {
      BOOST_CHECK_EQUAL(count, 0);
    }
};
int Content::count = 0;

TEMPLATE
static void test_push_back()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    BOOST_CHECK_EQUAL(v.size(), 0u);
    BOOST_CHECK(v.empty());
    for (int i = 0; i < I; ++i)
    {
      BOOST_CHECK_EQUAL(v.size(), size_t(i));
      v.push_back(T(i));
    }
    if (I)
      BOOST_CHECK(!v.empty());
    else
      BOOST_CHECK(v.empty());
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
  }
  T::after();
}

TEMPLATE
static void test_push_back_pop_back()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    BOOST_CHECK_EQUAL(v.size(), 0u);
    for (int i = 0; i < I; ++i)
    {
      BOOST_CHECK_EQUAL(v.size(), size_t(i));
      v.push_back(T(i));
      int c = 0;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
    }
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
    for (unsigned i = I; i; --i)
    {
      BOOST_CHECK_EQUAL(v.size(), size_t(i));
      v.pop_back();
      int c = 0;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
    }
    BOOST_CHECK_EQUAL(v.size(), 0u);
  }
  T::after();
}

TEMPLATE
static void test_push_pop_back()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (int i = 0; i < I; ++i)
    {
      v.push_back(T(i));
      v.push_back(T(i));
      v.pop_back();
      int c = 0;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
    }
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
    for (unsigned i = I; i; --i)
    {
      v.pop_back();
      int c = 0;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
    }
  }
  T::after();
}

TEMPLATE
static void test_clear()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (unsigned i = I; i; --i)
      v.push_back(T(i));
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
    v.clear();
    BOOST_CHECK_EQUAL(v.size(), 0u);
    T::after();
  }
}

TEMPLATE
static void test_copy()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (int i = 0; i < I; ++i)
      v.push_back(T(i));

    Vector v2(v);
    BOOST_CHECK_EQUAL(v2.size(), size_t(I));
    v.clear();
    BOOST_CHECK_EQUAL(v2.size(), size_t(I));
    int c = 0;
    foreach (T content, v2)
      BOOST_CHECK_EQUAL(content.i, c++);

    Vector v3(v2.begin(), v2.end());
    BOOST_CHECK_EQUAL(v3.size(), size_t(I));
    v2.clear();
    BOOST_CHECK_EQUAL(v3.size(), size_t(I));
    c = 0;
    foreach (T content, v3)
      BOOST_CHECK_EQUAL(content.i, c++);
  }
  T::after();
}

TEMPLATE
static void test_push_front_pop_front()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (int i = I - 1; i >= 0; --i)
    {
      v.push_front(T(i));
      int c = i;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
    }
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
    for (int i = 0; i < I; ++i)
    {
      int c = i;
      foreach (T content, v)
        BOOST_CHECK_EQUAL(content.i, c++);
      v.pop_front();
    }
  }
  T::after();
}

TEMPLATE
static void test_insert()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (int i = 0; i < I / 4; ++i)
      v.push_back(T(i));
    for (int i = 3 * I / 4; i < I; ++i)
      v.push_back(T(i));

    for (int i = I / 4; i < 3 * I / 4; ++i)
      v.insert(v.begin() + i, T(i));
    BOOST_CHECK_EQUAL(v.size(), size_t(I));
    int c = 0;
    foreach (T content, v)
      BOOST_CHECK_EQUAL(content.i, c++);
  }
  T::after();
}

TEMPLATE
static void test_erase()
{
  TYPEDEF;

  T::before();
  {
    Vector v;
    for (int i = 0; i < I; ++i)
    {
      v.push_back(T(i));
      v.push_back(T(i));
    }

    for (typename Vector::iterator it = v.begin(); it < v.end(); ++it)
      it = v.erase(it);
    int c = 0;
    foreach (T content, v)
      BOOST_CHECK_EQUAL(content.i, c++);
  }
  T::after();
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::vector test suite");
#define TEST_ITERS(Name, A, Ctor, Cap, Iters)                           \
  suite->add(BOOST_TEST_CASE((&test_##Name                              \
                              <Iters, Content, A, Ctor, Cap>)))

#define TEST_POLICIES(Name, A, Ctor, Cap)      \
    TEST_ITERS(Name, A, Ctor, Cap, 0);         \
    TEST_ITERS(Name, A, Ctor, Cap, 1);         \
    TEST_ITERS(Name, A, Ctor, Cap, 2);         \
    TEST_ITERS(Name, A, Ctor, Cap, 3);         \
    TEST_ITERS(Name, A, Ctor, Cap, 4);         \
    TEST_ITERS(Name, A, Ctor, Cap, 5);         \
    TEST_ITERS(Name, A, Ctor, Cap, 6);         \
    TEST_ITERS(Name, A, Ctor, Cap, 7);         \
    TEST_ITERS(Name, A, Ctor, Cap, 8);         \
    TEST_ITERS(Name, A, Ctor, Cap, 9);         \
    TEST_ITERS(Name, A, Ctor, Cap, 14);        \
    TEST_ITERS(Name, A, Ctor, Cap, 15);        \
    TEST_ITERS(Name, A, Ctor, Cap, 16);        \
    TEST_ITERS(Name, A, Ctor, Cap, 17);        \
    TEST_ITERS(Name, A, Ctor, Cap, 30);        \
    TEST_ITERS(Name, A, Ctor, Cap, 31);        \
    TEST_ITERS(Name, A, Ctor, Cap, 32);        \
    TEST_ITERS(Name, A, Ctor, Cap, 33);        \
    TEST_ITERS(Name, A, Ctor, Cap, 62);        \
    TEST_ITERS(Name, A, Ctor, Cap, 63);        \
    TEST_ITERS(Name, A, Ctor, Cap, 64);        \
    TEST_ITERS(Name, A, Ctor, Cap, 65);        \
    TEST_ITERS(Name, A, Ctor, Cap, 126);       \
    TEST_ITERS(Name, A, Ctor, Cap, 127);       \
    TEST_ITERS(Name, A, Ctor, Cap, 128);       \
    TEST_ITERS(Name, A, Ctor, Cap, 129);

  typedef libport::FlooredAllocator<Content, 8> FlooredAllocator;

#define TEST(Name)                                              \
  TEST_POLICIES(Name,                                           \
                libport::Allocator<Content>,                    \
                libport::Constructor<Content>,                  \
                libport::ExponentialCapacity);                  \
  TEST_POLICIES(Name,                                           \
                libport::Allocator<Content>,                    \
                libport::Constructor<Content>,                  \
                libport::FlooredExponentialCapacity<8>);        \
  TEST_POLICIES(Name,                                           \
                FlooredAllocator,                               \
                libport::Constructor<Content>,                  \
                libport::FlooredExponentialCapacity<8>);        \

    TEST(push_back);
    TEST(push_back_pop_back);
    TEST(push_pop_back);
    TEST(clear);
    TEST(copy);
    TEST(push_front_pop_front);
    TEST(insert);
    TEST(erase);
    return suite;
}
