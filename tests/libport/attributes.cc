/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/attributes.hh>
#include <libport/unit-test.hh>

/*----------------.
| Getter / setter |
`----------------*/

class GetterSetter
{
public:
  GetterSetter()
    : i_(0)
    , r_("r")
    , w_("w")
    , rw_("rw")
  {}

  void inc()
  {
    i_++;
  }

  void cat(std::string suffix)
  {
    r_ += suffix;
  }

  void check(int i, std::string w)
  {
    BOOST_CHECK_EQUAL(i_,  i);
    BOOST_CHECK_EQUAL(w_,  w);
  }

  ATTRIBUTE   (int, i);
  ATTRIBUTE_R (std::string, r);
  ATTRIBUTE_W (std::string, w);
  ATTRIBUTE_RW(std::string, rw);
};

void
getter_setter()
{
  GetterSetter t;
  t.check(0, "w");
  BOOST_CHECK_EQUAL(t.r_get(), "r");
  BOOST_CHECK_EQUAL(t.rw_get(), "rw");

  t.w_set("w'");
  t.inc();
  t.check(1, "w'");

  t.cat("ectum!");
  BOOST_CHECK_EQUAL(t.r_get(), "rectum!");

  t.rw_set("etirw");
  BOOST_CHECK_EQUAL(t.rw_get(), "etirw");
}

/*-------------.
| Custom impls |
`-------------*/

class Custom
{
public:
  Custom()
    : i_(0)
  {}

  ATTRIBUTE_rw(int, i);
};

int
Custom::i_get() const
{
  return i_ * 2;
}

void
Custom::i_set(int v)
{
  i_ = v + 1;
}


void
custom()
{
  Custom t;
  t.i_set(20);
  BOOST_CHECK_EQUAL(t.i_get(), 42);
}

/*-----------.
| Test suite |
`-----------*/

using libport::test_suite;

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::attributes test suite");
  suite->add(BOOST_TEST_CASE(getter_setter));
  suite->add(BOOST_TEST_CASE(custom));
  return suite;
}
