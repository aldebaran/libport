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
#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <fstream>
#include <iostream>

int
main (int argc, const char* argv[])
{
  assert(argc == 2);
  errno = 0;
  DIR* sem_dir = opendir(argv[1]);
  if (sem_dir == NULL)
  {
    std::cerr << argv[1] << ": " << strerror(errno) << std::endl;
    exit(1);
  }
  struct dirent* f;
  size_t cleaned = 0;
  size_t number = 0;
  std::string name;
  while ((f = readdir(sem_dir)))
  {
    std::ifstream i(std::string(argv[1]) + "/" + f->d_name);
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
  }
  std::cerr << "Reclaimed " << cleaned << "/" << number
            << " (" << int (cleaned * 100 / number) << "%) semaphores."
            << std::endl;
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
