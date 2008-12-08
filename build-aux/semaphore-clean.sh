#! /bin/sh

set -ex
me=$(basename $0 '.sh')

log=/tmp/urbi-semaphores.log
generate_semaphore_clean ()
{
   cd /tmp
   rm -f $me $me.cc
   cat >$me.cc <<EOF
#include <semaphore.h>
#include <cerrno>
#include <fstream>
#include <iostream>

int
main (int argc, const char* argv[])
{
  assert(argc == 2);
  std::ifstream i(argv[1]);
  unsigned count = 0;
  std::string name;
  while (i >> name)
  {
    std::string reason;
    errno = 0;
    sem_unlink(name.c_str());
    switch (errno)
    {
      case 0:      reason = "OK"; ++count;   break;
      case EINVAL /* ENOENT */:              break;
      default:     reason = strerror(errno); break;
    }
    if (!reason.empty())
      std::cerr << name << ": " << reason << std::endl;
  }
  std::cerr << "Reclaimed " << count << " semaphores." << std::endl;
}
EOF
  g++ -Wall $me.cc -o $me
}

if test -f $log; then
  mv $log $log.$$
  generate_semaphore_clean
  /tmp/$me $log.$$
  rm $log.$$
fi

