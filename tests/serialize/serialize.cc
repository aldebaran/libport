/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <climits>
#include <fstream>
#include <ios>
#include <string>

#include <libport/debug.hh>
#include <libport/bind.hh>

#include <libport/export.hh>
#include <libport/hierarchy.hh>
#include <libport/unit-test.hh>

#include <serialize/serialize.hh>

using libport::test_suite;
using namespace libport::serialize;

#define BASE "tests/serialize/"

#define SERIALIZE(Type, Value)                                  \
    BOOST_CHECK_NO_THROW(ser.serialize<Type>("test", Value))

#define UNSERIALIZE(Type, Value)                                \
    BOOST_CHECK_EQUAL(ser.unserialize<Type>("test"), Value);

#define CHECK(Type, Value)                      \
  do {                                          \
    {                                           \
      std::ofstream f(fn);                      \
      BinaryOSerializer ser(f);                 \
      SERIALIZE(Type, (Type) (Value));          \
    }                                           \
    {                                           \
      std::ifstream f(fn);                      \
      BOOST_CHECK(f.good());                    \
      BinaryISerializer ser(f);                 \
      UNSERIALIZE(Type, (Type) (Value));        \
      f.peek();                                 \
      BOOST_CHECK(f.eof());                     \
    }                                           \
  } while (false)

GD_INIT();

void binary_pod()
{
  const char *fn = BASE "binary_pod";

  // Try isolated first, but also check when several values are
  // serialized in a row.
  CHECK(int, 42);
  CHECK(bool, true);
  CHECK(bool, false);
  CHECK(std::string, "string ");
  CHECK(float, 0.);
  CHECK(float, -1.);
  CHECK(float, 1234.567);
  CHECK(double, 0.);
  CHECK(double, -1.);
  CHECK(double, 1234.567);

  {
    std::ofstream f(fn);
    BinaryOSerializer ser(f);

    SERIALIZE(int, 42);
    SERIALIZE(bool, true);
    SERIALIZE(bool, false);
    SERIALIZE(std::string, "string ");
    int* p1 = new int(51);
    int* p2 = new int(69);
    SERIALIZE(int*, p1);
    SERIALIZE(int*, p2);
    SERIALIZE(int*, p2);
    SERIALIZE(int*, p1);
    SERIALIZE(int*, NULL);
  }
  {
    std::ifstream f(fn);
    BOOST_CHECK(f.good());
    BinaryISerializer ser(f);

    UNSERIALIZE(int, 42);
    UNSERIALIZE(bool, true);
    UNSERIALIZE(bool, false);
    UNSERIALIZE(std::string, "string ");
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

void binary_integers_size()
{
  const char *fn = BASE "binary_integers_size";

#define CHECK_MAX(Type)                                 \
  CHECK(Type, std::numeric_limits<Type>::max() / 2)

  CHECK_MAX(unsigned short);
  CHECK_MAX(unsigned int);
  CHECK_MAX(unsigned long);
  CHECK_MAX(unsigned long long);
#undef CHECK_MAX
}

void binary_integers_size_portability()
{
  const char *fn = BASE "binary_integers_size";
  // Simulate fancy short size.
  {
    std::ofstream f(fn);
    BinaryOSerializer ser(f);

    // Change integers size.
    char sizes[] = {0x4, 0x4, 0x4, 0x8,};
    f.flush();
    f.seekp(std::ios_base::beg);
    f.write(sizes, sizeof(sizes));
    f.flush();

    SERIALIZE(uint32_t, 0);
    SERIALIZE(uint32_t, 42);
    SERIALIZE(uint32_t, USHRT_MAX + 1);
    SERIALIZE(uint32_t, USHRT_MAX);
    f.flush();
    f.close();
  }
  {
    std::ifstream f(fn);
    BOOST_CHECK(f.good());
    BinaryISerializer ser(f);
    UNSERIALIZE(unsigned short, 0);
    UNSERIALIZE(unsigned short, 42);
    BOOST_CHECK_THROW(ser.unserialize<unsigned short>(),
                      libport::serialize::Exception);
    UNSERIALIZE(unsigned short, USHRT_MAX);
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
  const char *fn = BASE "binary_class";

  {
    std::ofstream f(fn);
    BinaryOSerializer ser(f);

    Person ed("Draven", "Eric");
    Person cs("Slade", "Cutter");
    ser.serialize<Person>("test", ed);
    ser.serialize<Person>("test", cs);
  }
  {
    std::ifstream f(fn);
    BinaryISerializer ser(f);

    Person ed = ser.unserialize<Person>("test");
    Person cs = ser.unserialize<Person>("test");
    BOOST_CHECK_EQUAL(ed.name_, "Draven");
    BOOST_CHECK_EQUAL(ed.surname_, "Eric");
    BOOST_CHECK_EQUAL(cs.name_, "Slade");
    BOOST_CHECK_EQUAL(cs.surname_, "Cutter");
  }
}

struct Unix;
struct Linux;
struct Gentoo;
struct Debian;

struct Unix: public libport::meta::Hierarchy<Unix, TYPELIST_2(Gentoo, Debian)>
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
  const char *fn = BASE "binary_hier";

  {
    std::ofstream f(fn);
    BinaryOSerializer ser(f);

    Debian d("2.4", "sarge");
    Gentoo g("2.6", 2008);
    ser.serialize<Debian>("test", d);
    ser.serialize<Gentoo>("test", g);
  }
  {
    std::ifstream f(fn);
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
  suite->add(BOOST_TEST_CASE(binary_integers_size));
  suite->add(BOOST_TEST_CASE(binary_integers_size_portability));
  suite->add(BOOST_TEST_CASE(binary_class));
  suite->add(BOOST_TEST_CASE(binary_hierarchy));
  return suite;
}
