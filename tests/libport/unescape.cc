/**
 ** Test code for libport/escape.hh.
 */

#include <libport/escape.hh>
#include <libport/assert.hh>
#include <sstream>
#include <iostream>
#include <stdexcept>

using libport::unescape;

int main ()
{
  std::string u = unescape("\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"");
  passert(u, u == "\a\b\f\n\r\t\v\\\"");
  u = unescape("\\x07");
  passert(u,u == "\a");
  u = unescape("\\x20");
  passert(u, u == " ");
  std::string except;
  try {
    unescape("aaa\\");
  }
  catch(std::runtime_error e)
  {
    except = e.what();
  }
  passert(except, except == "invalid escape: '\\' at end of string");
  try {
    unescape("aaa\\z");
  }
  catch(std::runtime_error e)
  {
    except = e.what();
  }
  passert(except, except == "invalid escape: '\\z'");
  try {
    unescape("aaa\\x1");
  }
  catch(std::runtime_error e)
  {
    except = e.what();
  }
  passert(except, except == "invalid escape: '\\x' not followed by two digits");
}
