#include <string>

#include <libport/pthread.h>
#include <libport/unit-test.hh>

using libport::test_suite;

std::string truth("vim is future");

void* thread1(void* data)
{
  std::string del((char *) data);
  size_t pos = truth.find(del);
  truth.erase(pos, pos + del.size());
  return 0;
}

void* thread2(void* data)
{
  pthread_join(*(pthread_t *)data, 0);
  truth.insert(0, "emacs");
  return (void*)0xdeadbeef;
}

void test_pthread()
{
  pthread_t my_thread[2];
  const char* vim = "vim";
  void* ret;

  BOOST_CHECK(!pthread_create(my_thread, 0, &thread1, (char*)vim));
  BOOST_CHECK(!pthread_create(my_thread + 1, 0, &thread2, my_thread));
  pthread_join(my_thread[1], &ret);
#if !defined WIN32
  BOOST_CHECK_EQUAL(ret, (void*)0xdeadbeef);
#endif
  BOOST_CHECK_EQUAL(truth, "emacs is future");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::semaphore");
  suite->add(BOOST_TEST_CASE(test_pthread));
  return suite;
}
