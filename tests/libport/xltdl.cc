#include <libport/xltdl.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

using libport::xlt_advise;
using libport::xlt_handle;

static void
check()
{
  xlt_advise a;
  xlt_handle h;

  // Open libport.la.
  BOOST_CHECK_NO_THROW(h = a.open("lib/libport/libport.la"));
  BOOST_CHECK(h.handle);

  // Load xgetenv, and use it.
  typedef const char* (*getenv_type) (const char*);
  getenv_type g = 0;
  BOOST_CHECK_NO_THROW(g = h.sym<getenv_type>("xgetenv"));
  BOOST_CHECK(g);
  BOOST_CHECK_EQUAL(g("PATH"), getenv("PATH"));

  // Close libport.la.
  BOOST_CHECK_NO_THROW(h.close());
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::xltdl test suite");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
