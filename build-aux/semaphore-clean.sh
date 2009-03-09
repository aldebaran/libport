#! /bin/sh

set -ex
me=$(basename $0 '.sh')

log=/tmp/urbi-semaphores
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
  size_t cleaned = 0;
  size_t number = 0;
  std::string name;
  std::ifstream i(argv[1]);
  while (i >> name)
   {
    ++number;
    std::string reason;
    errno = 0;
    sem_unlink(name.c_str());
    switch (errno)
    {
      case 0:      reason = "OK"; ++cleaned; break;
      case EINVAL /* ENOENT */:              break;
      default:     reason = strerror(errno); break;
    }
    if (!reason.empty())
      std::cerr << name << ": " << reason << std::endl;
  }
  std::cerr << "Reclaimed " << cleaned << "/" << number
            << " (" << int (cleaned * 100 / number) << "%) semaphores."
            << std::endl;
  return 0;
}
EOF
  g++ -Wall $me.cc -o $me
}

if test -d $log; then
  cp -r $log $log.$$
  generate_semaphore_clean
  for f in $(ls $log.$$)
  do
      if ps -p $f >> /dev/null ; then
          echo ">>> Process $f is running..."
      else
          /tmp/$me $log.$$/$f
          rm $log/$f
      fi
  done
  rm -Rf $log.$$
fi
