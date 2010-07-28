/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <fcntl.h>
#include <string>
#include <iostream>
#include <cassert>

void
effectiveDisplay(const char* t)
{
#define Echo(S)  std::cerr << #S ": " << S << std::endl
  Echo("----------------------");

  Echo("Save");
  int flags;
  Echo(fcntl(STDIN_FILENO, F_GETFL));
  Echo((flags = fcntl(STDOUT_FILENO, F_GETFL)));
  Echo(fcntl(STDERR_FILENO, F_GETFL));

  Echo("Set");
  int res = flags | O_NONBLOCK;
  Echo((res = fcntl(STDOUT_FILENO, F_SETFL, res)));
  Echo(fcntl(STDIN_FILENO, F_GETFL));
  Echo(fcntl(STDOUT_FILENO, F_GETFL));
  Echo(fcntl(STDERR_FILENO, F_GETFL));

  std::cout << t << std::endl;

  Echo("Restore");
  Echo((res = fcntl(STDOUT_FILENO, F_SETFL, flags)));
  Echo(fcntl(STDIN_FILENO, F_GETFL));
  Echo(fcntl(STDOUT_FILENO, F_GETFL));
  Echo(fcntl(STDERR_FILENO, F_GETFL));
}

int
main (int argc, const char* argv[])
{
  for (int i = 1; i < argc; ++i)
    effectiveDisplay(argv[i]);
}
