#include <string>

#include <boost/bind.hpp>

#include <libport/export.hh>
#include <libport/unit-test.hh>

#include <serialize/binary-i-serializer.hh>
#include <serialize/binary-o-serializer.hh>
#include <serialize/serializable.hh>
#include <serialize/serializer.hh>
#include <serialize/xml-i-serializer.hh>
#include <serialize/xml-o-serializer.hh>

using namespace libport::serialize;
using libport::test_suite;

struct Data: public Serializable
{
  Data(const std::string& d1 = "",
       const std::string& d2 = "")
    : data1(d1)
    , data2(d2)
  {}

  virtual void serialize(libport::serialize::Serializer& ser)
  {
    ser.serialize("data1", data1);
    ser.serialize("data2", data2);
  }

  bool operator == (const Data& other) const
  {
    return data1 == other.data1 && data2 == other.data2;
  }

  std::string data1;
  std::string data2;
};

struct Datas: public Serializable
{
  Datas(const std::string& n)
    : name(n)
    , datas()
  {}

  Datas& operator << (Data d)
  {
    datas.push_back(d);
    return *this;
  }

  bool operator == (const Datas& other) const
  {
    bool res = true;
    res = res && name == other.name;
    res = res && datas.size() == other.datas.size();
    for (unsigned i = 0; res && i < datas.size(); ++i)
      res = res && datas[i] == other.datas[i];
    return res;
  }

  virtual void serialize(libport::serialize::Serializer& ser)
  {
    ser.serialize("name", name);
    ser.serialize("datas", datas);
  }

  std::string name;
  std::vector<Data> datas;
};

void test()
{
  // The test file base name.
  const std::string basename = "serialize/test";

  // Test datas
  Datas o("name");
  o << Data("foo&", "<bar>");
  o << Data("baz", "quux");

  // Export them as XML
  {
    XmlOSerializer os(basename + ".xml");
    os.serialize("test", o);
  }
  // Import them as XML
  {
    Datas i("");
    {
      XmlISerializer is(basename + ".xml");
      is.serialize("test", i);
    }
    BOOST_CHECK(i == o);
  }

  // Export them as binary
  {
    BinaryOSerializer os(basename + ".bin");
    os.serialize("test", o);
  }
  // Import them as binary
  {
    Datas i("");
    {
      BinaryISerializer is(basename + ".bin");
      is.serialize("test", i);
    }
    std::cerr << i.name << std::endl;
    BOOST_CHECK(i == o);
  }
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("serialization test suite");

  suite->add(BOOST_TEST_CASE(test));

  return suite;
}
