/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>

int bar();
int baz(int);

int main(int argc, char *argv[])
{
  using std::cout;
  using std::endl;

  cout << baz(bar()) << endl;

  return 0;
}
