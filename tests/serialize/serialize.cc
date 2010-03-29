/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <fstream>
#include <string>

#include <libport/bind.hh>

#include <libport/export.hh>
#include <libport/hierarchy.hh>
#include <libport/unit-test.hh>

#include <serialize/serialize.hh>

using libport::test_suite;
using namespace libport::serialize;

#define BASE "tests/serialize/"

void binary_pod()
{
  {
    std::ofstream f(BASE "binary_pod");
    BinaryOSerializer ser(f);

    ser.serialize<int>("test", 42);
    ser.serialize<bool>("test", true);
    ser.serialize<bool>("test", false);
    ser.serialize<std::string>("test", "string ");
    int* p1 = new int(51);
    int* p2 = new int(69);
    ser.serialize<int*>("test", p1);
    ser.serialize<int*>("test", p2);
    ser.serialize<int*>("test", p2);
    ser.serialize<int*>("test", p1);
    ser.serialize<int*>("test", NULL);
  }
  {
    std::ifstream f(BASE "binary_pod");
    BOOST_CHECK(f.good());
    BinaryISerializer ser(f);

    BOOST_CHECK_EQUAL(ser.unserialize<int>("test"), 42);
    BOOST_CHECK(ser.unserialize<bool>("test"));
    BOOST_CHECK(!ser.unserialize<bool>("test"));
    BOOST_CHECK_EQUAL(ser.unserialize<std::string>("test"), "string ");
    int* p1 = ser.unserialize<int*>("test");
    int* p2 = ser.unserialize<int*>("test");
    int* p3 = ser.unserialize<int*>("test");
    int* p4 = ser.unserialize<int*>("test");
    int* p5 = ser.unserialize<int*>("test");
    BOOST_CHECK_EQUAL(*p1, 51);
    BOOST_CHECK_EQUAL(*p2, 69);
    BOOST_CHECK_EQUAL(p1, p4);
    BOOST_CHECK_EQUAL(p2, p3);
    BOOST_CHECK_EQUAL(p5, reinterpret_cast<int*>(NULL));
  }
}

struct Person
{
  Person(const std::string& name, const std::string& surname)
    : name_(name)
    , surname_(surname)
  {}

  template <typename S>
  Person(ISerializer<S>& input)
    : name_(input.template unserialize<std::string>("name"))
    , surname_(input.template unserialize<std::string>("surname"))
  {

  }

  template <typename S>
  void serialize(OSerializer<S>& output) const
  {
    output.serialize<std::string>("name", name_);
    output.serialize<std::string>("surname", surname_);
  }

  std::string name_, surname_;
};

void binary_class()
{
  {
    std::ofstream f(BASE "binary_class");
    BinaryOSerializer ser(f);

    Person ed("Draven", "Eric");
    Person cs("Slade", "Cutter");
    ser.serialize<Person>("test", ed);
    ser.serialize<Person>("test", cs);
  }
  {
    std::ifstream f(BASE "binary_class");
    BinaryISerializer ser(f);

    Person ed = ser.unserialize<Person>("test");
    Person cs = ser.unserialize<Person>("test");
    BOOST_CHECK_EQUAL(ed.name_, "Draven");
    BOOST_CHECK_EQUAL(ed.surname_, "Eric");
    BOOST_CHECK_EQUAL(cs.name_, "Slade");
    BOOST_CHECK_EQUAL(cs.surname_, "Cutter");
  }
}

class Unix;
class Linux;
class Gentoo;
class Debian;

class Unix: public libport::meta::Hierarchy<Unix, TYPELIST_2(Gentoo, Debian)>
{

};


struct Linux: public Unix
{
  Linux(const std::string& k)
    : kernel(k)
  {}

  template <typename T>
  Linux(ISerializer<T>& ser)
  {
    kernel = ser.template unserialize<std::string>("kernel");
  }

  template <typename T>
  void serialize(OSerializer<T>& ser) const
  {
    ser.template serialize<std::string>("kernel", kernel);
  }

  std::string kernel;
};

struct Debian: public Linux
{
  Debian(const std::string& kernel, const std::string& v)
    : Linux(kernel)
    , version(v)
  {}

  template <typename T>
  Debian(ISerializer<T>& ser)
    : Linux(ser)
  {
    version = ser.template unserialize<std::string>("version");
  }

  template <typename T>
  void serialize(OSerializer<T>& ser) const
  {
    Linux::serialize(ser);
    ser.serialize<std::string>("version", version);
  }

  std::string version;
};

struct Gentoo: public Linux
{
  Gentoo(const std::string& kernel, int v)
    : Linux(kernel)
    , version(v)
  {}

  template <typename T>
  Gentoo(ISerializer<T>& ser)
    : Linux(ser)
  {
    version = ser.template unserialize<int>("version");
  }

  template <typename T>
  void serialize(OSerializer<T>& ser) const
  {
    Linux::serialize(ser);
    ser.serialize<int>("version", version);
  }

  int version;
};

void binary_hierarchy()
{
  {
    std::ofstream f(BASE "binary_hier");
    BinaryOSerializer ser(f);

    Debian d("2.4", "sarge");
    Gentoo g("2.6", 2008);
    ser.serialize<Debian>("test", d);
    ser.serialize<Gentoo>("test", g);
  }
  {
    std::ifstream f(BASE "binary_hier");
    BinaryISerializer ser(f);

    Unix* d_ = ser.unserialize<Unix>("test");
    Unix* g_ = ser.unserialize<Unix>("test");
    Debian* d = dynamic_cast<Debian*>(d_ );
    Gentoo* g = dynamic_cast<Gentoo*>(g_) ;
    BOOST_CHECK(d);
    BOOST_CHECK(g);
    BOOST_CHECK_EQUAL(d->kernel, "2.4");
    BOOST_CHECK_EQUAL(d->version, "sarge");
    BOOST_CHECK_EQUAL(g->kernel, "2.6");
    BOOST_CHECK_EQUAL(g->version, 2008);
  }
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Serialization test suite");
  suite->add(BOOST_TEST_CASE(binary_pod));
  suite->add(BOOST_TEST_CASE(binary_class));
  suite->add(BOOST_TEST_CASE(binary_hierarchy));
  return suite;
}
