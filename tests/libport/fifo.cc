#include <cassert>

#include <libport/fifo.hh>

// Use a queue with a small limit to test the
// reorganization.
static libport::Fifo<char, '\0'> queue(8);

// Check size and capacity.
static void
csp(size_t sz, size_t cp)
{
  assert(queue.size() == sz);
  assert(queue.capacity() == cp);
}

int
main()
{
  queue.push("Hello");
  csp(5, 8);

  // Check that the peek() doesn't modify the queue.
  assert(std::string(queue.peek()) == "Hello");
  csp(5, 8);
  assert(std::string(queue.peek()) == "Hello");

  // Check that consuming the queue works as expected.
  queue.pop(5);
  csp(0, 8);
  assert(*queue.peek() == '\0');

  // Check that partial retrieval works as well.
  queue.push("Hello");
  csp(5, 8);
  assert(std::string(queue.peek()) == "Hello");
  assert(std::string(queue.pop(3), 3) == "Hel");
  csp(2, 8);
  assert(std::string(queue.peek()) == "lo");

  // Check that we can add data without reallocating.
  queue.push("Hello");
  csp(7, 8);
  assert(std::string(queue.peek()) == "loHello");

  // Check that we reallocation occurs automatically.
  queue.push(", world, this is me!");
  csp(27, 32);
  assert(std::string(queue.peek()) == "loHello, world, this is me!");
}
