/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#include <libport/unit-test.hh>
#include <libport/visitor.hh>

using libport::test_suite;


// These probably come from some fwd.hh file
class Root;
class Child1;
class Child2;
class Child3;
class Child4;

// Visited nodes
//
//              Root
//               ||
//          ------------
//          |          |
//        Child1     Child2
//                     |
//                -----------
//                |         |
//              Child3    Child4
//

class Root: public libport::meta::Visitable
{
    // Every class shall call TYPE(herself)
    TYPE(Root);
    // The root class must declare all the concrete nodes in its hierarchy
    HIERARCHY(TYPELIST_4(Child1, Child2, Child3, Child4));

  public:
    // Whatever ...
};

class Child1: public Root
{
    TYPE(Child1);
    // Every concrete node must call VISITABLE()
    VISITABLE();
};

class Child2: public Root
{
    TYPE(Child2);
    VISITABLE();
};

class Child3: public Child2
{
    TYPE(Child3);
    VISITABLE();
};

class Child4: public Child2
{
    TYPE(Child4);
    VISITABLE();
};

// Visitor
class MyVisitor: public libport::meta::Visitor
<MyVisitor,                     // I am MyVisitor
 Root,                          // I visit Root and its children
 int,                           // I return integers
 libport::traits::ConstRef>     // I visit by const reference
{
  public:
    int visit(const Child1&)
    {
      return 1;
    }
    int visit(const Child2&)
    {
      return 2;
    }
    int visit(const Child4&)
    {
      return 4;
    }
};

static void check()
{
  MyVisitor v;
  Child1 c1;
  Child2 c2;
  Child3 c3;
  Child4 c4;
  BOOST_CHECK(v(c1) == 1);
  BOOST_CHECK(v(c2) == 2);
  BOOST_CHECK(v(c3) == 2);
  BOOST_CHECK(v(c4) == 4);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::Visitor test suite");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
