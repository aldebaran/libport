#include <string>

#include <boost/bind.hpp>

#include <libport/export.hh>
#include <libport/unit-test.hh>

#include <serialize/serializable.hh>
#include <serialize/serializer.hh>
#include <serialize/xml-o-serializer.hh>
#include <serialize/xml-i-serializer.hh>

using namespace libport::serialize;
using libport::test_suite;

struct Data: public Serializable
{
  Data(const std::string& d1 = "",
       const std::string& d2 = "",
       const std::string& d3 = "")
    : data1(d1)
    , data2(d2)
    , data3(d3)
  {}

  virtual void serialize(libport::serialize::Serializer& ser)
  {
    ser.serialize("data1", data1);
    ser.serialize("data2", data2);
    ser.serialize("data3", data3);
  }

  std::string data1;
  std::string data2;
  std::string data3;
};

void test()
{
  {
    Data o("foo", "bar", "<baz");
    XmlOSerializer os("test.xml");
    os.serialize("test", o);
  }

  {
    Data i;
    XmlISerializer is("test.xml");
    is.serialize("test", i);
    BOOST_CHECK_EQUAL(i.data1, "foo");
    BOOST_CHECK_EQUAL(i.data2, "bar");
    BOOST_CHECK_EQUAL(i.data3, "<baz");
  }
}

LIBPORT_API test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("serialization test suite");

  suite->add(BOOST_TEST_CASE(test));

  return suite;
}
