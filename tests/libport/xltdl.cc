/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#include <libport/xltdl.hh>
#include "test.hh"

using libport::test_suite;

using libport::xlt_advise;
using libport::xlt_handle;

static void
check()
{
  xlt_advise a;
  xlt_handle h;

  // Open libport.la.
  BOOST_CHECK_NO_THROW(h = a.open("lib/libport/libport" LIBPORT_LIBSFX ".la"));
  BOOST_CHECK(h.handle);

  // The test function, and its type.
  typedef int (*identity_type) (int);
  const char* idname = "libport_xlt_details_identity";

  // Load libport_xlt_details_identity, untyped.
  {
    void* id = 0;
    BOOST_CHECK_NO_THROW(id = lt_dlsym(h.handle, idname));
    BOOST_CHECK_MESSAGE(id,
                        libport::format("failed to dlsym %s",
                                        idname));
    BOOST_CHECK_MESSAGE(id,
                        libport::format("error message: %s",
                                        lt_dlerror() ? lt_dlerror() : "NULL"));

    unsigned long idl = reinterpret_cast<unsigned long>(id);
    BOOST_CHECK_MESSAGE(idl, "failed to cast to unsigned long");

    identity_type idtyped = reinterpret_cast<identity_type>(idl);
    BOOST_CHECK_MESSAGE(idtyped, "failed to cast to function type");

    BOOST_CHECK_EQUAL(idtyped(123), 123);
  }

  // Load libport_xlt_details_identity, typed, and use it.
  {
    identity_type id = 0;
    BOOST_CHECK_NO_THROW(id = h.sym<identity_type>(idname));
    BOOST_CHECK(id);
    BOOST_CHECK_EQUAL(id(123456), 123456);
  }

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
