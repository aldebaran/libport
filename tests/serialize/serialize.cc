#include <string>

#include <boost/bind.hpp>

#include <libport/export.hh>
#include <libport/unit-test.hh>

#include <serialize/binary-i-serializer.hh>
#include <serialize/binary-o-serializer.hh>
#include <serialize/serializable.hh>
//#include <serialize/serializer.hh>
// #include <serialize/xml-i-serializer.hh>
// #include <serialize/xml-o-serializer.hh>

using namespace libport::serialize;
using libport::test_suite;

class Datas: public Serializable
{
public:
  Datas(int a, int b, int c)
  {
    datas_.push_back(a);
    datas_.push_back(b);
    datas_.push_back(c);
  }

  std::vector<int> datas_;
  SERIALIZABLE(Datas, , , (datas));
};

class Test: public Serializable
{
public:
  Test(const std::string& name)
    : name_(name)
  {}

protected:
  std::string name_;
  SERIALIZABLE(Test, , , (name));
};

class TestData: public Test
{
public:
  TestData(const std::string& name)
    : Test(name)
    , description_("Draven")
    , datas_(42, 51, 69)
  {}

  bool operator==(const TestData& other)
  {
    return name_ == other.name_
      && description_ == other.description_
      && datas_.datas_ == other.datas_.datas_;
  }

protected:
  std::string description_;
  Datas datas_;
  SERIALIZABLE(TestData, (Test), (datas), (description));
};

static const std::string path = "tests/serialize/test";

void test()
{
  TestData src("Eric");

  {
    BinaryOSerializer out(path + ".bin");
    out.serialize("data", src);
  }

  {
    BinaryISerializer in(path + ".bin");
    TestData dst(in);
    BOOST_CHECK(src == dst);
  }
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("serialization test suite");

  suite->add(BOOST_TEST_CASE(test));

  return suite;
}
