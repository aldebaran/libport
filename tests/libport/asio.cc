#define LIBPORT_NO_SSL
#include <libport/asio.hh>
#include <libport/utime.hh>
#include <libport/assert.hh>
#include <libport/unistd.h>

bool abort_ctor = false;
template<class T> void hold_for(T, libport::utime_t duration)
{
  usleep(duration);
  std::cerr << "Done holding your T" << std::endl;
}

void delayed_response(boost::shared_ptr<libport::UDPLink> l, libport::utime_t duration)
{
  usleep(duration);
  l->reply("hop hop\n");
}

// Hack until we can kill listenig sockets.
static bool enable_delay = false;
void reply_delay(const void* , int , boost::shared_ptr<libport::UDPLink> l, libport::utime_t duration)
{
  libport::startThread(boost::bind(&delayed_response, l, duration));
}

void echo(const void* d, int s, boost::shared_ptr<libport::UDPLink> l)
{
  if (enable_delay)
    reply_delay(d, s, l, 500000);
  else
    l->reply(d, s);
}

class TestSocket: public libport::Socket
{
  public:
  TestSocket()
    : nRead(0), echo(false), dump(false)
    {
      assert(!abort_ctor);
      nInstance++;
    }
  TestSocket(bool echo, bool dump)
    : nRead(0), echo(echo), dump(dump)
    {
      assert(!abort_ctor);
      nInstance++;
    }
  virtual ~TestSocket()
  {
    nInstance--;
    //std::cerr << this <<" dying, in " << K_get() << " lasterror=" << lastError.message() << std::endl;
    //assert(false);
  }
  int onRead(const void* data, int size)
  {
    nRead++;
    //std::cerr << this << " read " << size << std::endl;
    if (echo)
      write(data, size);
    if (dump)
      received += std::string((const char*)data, size);
    return size;
  }

  void onError(boost::system::error_code erc)
  {
    lastError = erc;
    destroy();
  }
  // Number of times read callback was called
  int nRead;
  // Echo back what is received.
  bool echo;
  // Store what is received in received.
  bool dump;
  std::string received;
  boost::system::error_code lastError;
  static TestSocket* factory()
    {
      return lastInstance = new TestSocket();
    }
  static TestSocket* factoryEx(bool echo, bool dump)
    {
      return lastInstance = new TestSocket(echo, dump);
    }
  static int nInstance;
  // Last factory-created instance.
  static TestSocket* lastInstance;
};


int TestSocket::nInstance = 0;
TestSocket* TestSocket::lastInstance = 0;

// Delay in microseconds used to sleep when something asynchronous
// is happening.
static const int delay = 200000;

static const int AVAIL_PORT = 7890;
static const std::string S_AVAIL_PORT = "7890";

void test_one(bool proto)
{
  TestSocket* client = new TestSocket(false, true);
  boost::system::error_code err;
  err = client->connect("localhost", S_AVAIL_PORT , proto);
  passert(err.message(), !err);
  client->send("coincoin\n");
  usleep(delay);
  passert(TestSocket::nInstance, TestSocket::nInstance == (proto?1:2));
  passert(client->received, client->received == "coincoin\n");
  if (!proto)
    passert(TestSocket::lastInstance->received,
	"coincoin\n" == TestSocket::lastInstance->received);
  passert(client->getRemotePort(), client->getRemotePort() == AVAIL_PORT);
  if (!proto)
    passert(TestSocket::lastInstance->getLocalPort(),
	TestSocket::lastInstance->getLocalPort() == AVAIL_PORT);
  if (!proto)
    assert(TestSocket::lastInstance->getRemotePort() == client->getLocalPort());
  // Close client-end. Should send error both ways and destroy all sockets.
  client->close();
  usleep(delay);
  passert(TestSocket::nInstance, !TestSocket::nInstance);
}


int main()
{
  // Basic TCP
  std::cerr << "##Safe destruction" << std::endl;
  boost::system::error_code err;
  TestSocket* s = new TestSocket(false, false);
  s->connect("localhost", S_AVAIL_PORT, false);
  s->destroy();
  usleep(delay);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  libport::Socket* h = new libport::Socket();
  err = h->listen(
      boost::bind(&TestSocket::factoryEx, true, true), "", S_AVAIL_PORT, false);
  passert(err.message(), !err);
  passert(h->getLocalPort(), h->getLocalPort() == AVAIL_PORT);
  TestSocket* client;

  std::cerr << "##One client" << std::endl;
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  test_one(false);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  test_one(false);
  libport::resolve<boost::asio::ip::tcp>("localhost", S_AVAIL_PORT, err);
  test_one(false);
  std::cerr << "Socket on stack"<< std::endl;
  {
    TestSocket s(false, true);
    err = s.connect("localhost", S_AVAIL_PORT, false);
    passert(err.message(), !err);
    s.send("coincoin\n");
    usleep(delay);
  }
  usleep(delay*2);
  passert(TestSocket::nInstance, !TestSocket::nInstance);

  test_one(false);
  std::cerr << "##many clients" << std::endl;
  std::vector<TestSocket*> clients;
  for (int i=0; i<10; i++)
  {
    TestSocket* client = new TestSocket(false, true);
    err = client->connect("localhost", S_AVAIL_PORT, false);
    passert(err.message(), !err);
    client->send("coincoin\n");
    clients.push_back(client);
  }
  usleep(delay*3);
  passert(TestSocket::nInstance, TestSocket::nInstance == 20);
  foreach(TestSocket* s, clients)
  {
    passert(s->received, "coincoin\n" == s->received);
    s->close();
  }
  usleep(delay*3);
  passert(TestSocket::nInstance, !TestSocket::nInstance);


  std::cerr << "##Failing connections" << std::endl;
  client = new TestSocket();
  err = client->connect("auunsinsr.nosuch.hostaufisgiu.com.", "20000", false);
  std::cerr << err.message() << std::endl;
  passert(err.message(), err);

  err = client->connect("localhost", "nosuchport", false);
  std::cerr << err.message() << std::endl;
  passert(err.message(), err);

  // Try to reuse that wasted socket.
  err = client->connect("localhost", S_AVAIL_PORT, false);
  passert(err.message(), !err);
  // Destroy without closing.
  client->destroy();
  usleep(delay);
  passert(TestSocket::nInstance, !TestSocket::nInstance);


  // Timeout
  std::cerr << "##Timeout connect" << std::endl;
  client = new TestSocket();
  libport::utime_t start = libport::utime();
  err = client->connect("1.1.1.1", "10000", false, 1000000);
  passert(err.message(), err);
  libport::utime_t timeout = libport::utime() - start;
  // Give it a good margin.
  passert(timeout, timeout < 1400000);
  client->destroy();


  std::cerr << "##Destruction locking" << std::endl;
  client = new TestSocket();
  err = client->connect("localhost", S_AVAIL_PORT, false);
  passert(err.message(), !err);
  usleep(delay);
  libport::startThread(boost::bind(&hold_for<libport::Destructible::DestructionLock>, client->getDestructionLock(), 1000000));
  client->destroy();
  usleep(500000);
  // There can be 1 or 2 sockets at this point. Client must be still alive because of the lock, but server might have died.
  passert(TestSocket::nInstance, TestSocket::nInstance);
  usleep(500000+delay);
  passert(TestSocket::nInstance, !TestSocket::nInstance);

  std::cerr << "Destroy listener" << std::endl;
  h->close();
  usleep(delay);
  client = new TestSocket();
  abort_ctor = true;
  err = client->connect("localhost", S_AVAIL_PORT, false);
  passert(err.message(), err);
  std::cerr << err.message() << std::endl;
  client->destroy();
  h->destroy();
  usleep(delay);
  abort_ctor = false;


  std::cerr << "##UDP" << std::endl;
  libport::Socket::Handle hu = libport::Socket::listenUDP(
      "", S_AVAIL_PORT, &echo, err);
  (void)hu;
  passert(err.message(), !err);
  test_one(true);
  usleep(delay);
  test_one(true);

  client = new TestSocket();
  err = client->connect("auunsinsr.nosuch.hostaufisgiu.com.", "20000", true);
  std::cerr << err.message() << std::endl;
  passert(err.message(), err);

  err = client->connect("localhost", "nosuchport", true);
  std::cerr << err.message() << std::endl;
  passert(err.message(), err);

  // Try to reuse that wasted socket.
  err = client->connect("localhost", S_AVAIL_PORT, true);
  passert(err.message(), !err);
  // Destroy without closing.
  client->destroy();
  usleep(delay);
  passert(TestSocket::nInstance, !TestSocket::nInstance);

  // Check one-write-one-packet semantic
  client = new TestSocket(false, true);
  err = client->connect("localhost", S_AVAIL_PORT, true);
  passert(err.message(), !err);
  client->send("coin");
  client->send("pan");
  usleep(delay*2);
  passert(client->received, client->received == "coinpan");
  passert(client->nRead, client->nRead == 2);

  enable_delay = true;
  client = new TestSocket(false, true);
  err = client->connect("localhost", S_AVAIL_PORT, true);
  passert(err.message(), !err);
  client->send("coin");
  usleep(500000+delay);
  passert(client->received, client->received == "hop hop\n");
}
