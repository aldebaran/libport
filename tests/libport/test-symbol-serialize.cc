/**
 ** Test code for libport/shared-ptr.hh features.
 */

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <libport/symbol.hh>
#include <libport/shared-ptr.hh>

using libport::Symbol;

int
main()
{
  std::stringstream s;
  Symbol foo("foo"), bar("bar");
  boost::archive::text_oarchive oa(s);
  oa << foo << foo << bar;

  boost::archive::text_iarchive ia(s);
  Symbol s1, s2, s3;
  ia >> s1 >> s2 >> s3;

  assert(s1 == s2);
  assert(s1 != s3);
  assert(s1.name_get() == "foo");
  assert(s3.name_get() == "bar");

  return 0;
}
