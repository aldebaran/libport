#! /bin/sh

set -e
me=$(basename $0 '.sh')

stderr ()
{
  local i
  for i
  do
    echo >&2 "$me: $i"
  done
}

tmp=/tmp/urbi-semaphores
clean="/tmp/$me.$USERNAME"
generate_semaphore_clean ()
{
  cat >$clean.cc.tmp <<EOF
#include <semaphore.h>
#include <cassert>
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
  if ! test -x $clean ||
     ! diff $clean.cc.tmp $clean.cc >/dev/null 2>&1; then
    mv -f $clean.cc.tmp $clean.cc
    g++ -Wall $clean.cc -o $clean
  fi
}

# Backward compatibility: a single file with all the semaphore names.
if test -f $tmp.log; then
  mv $tmp.log $tmp.log.$$
  mkdir -p $tmp
  perl -p \
     -e "BEGIN { my \$tmp = \"$tmp\" };" \
     -e '
    if (m,^sema/(\d+)/\d+,)
    {
      if ($pid != $1)
      {
        $pid = $1;
        open ($fh, ">>", "$tmp/$pid");
      }
      print $fh "$_";
    }' $tmp.log.$$
  rm $tmp.log.$$
fi

if test -d $tmp; then
  cd $tmp
  # Put in all.$$ the list of all the semaphores that do not belong
  # to a running process.  Because OS X does not use sequential pids,
  # use "mv" to have the smallest possible window during which several
  # processes may access concurrently to it.
  for f in *
  do
    # Empty directory might return the globbing pattern.
    test "$f" != "*" ||
      break
    if ps -p $f >/dev/null; then
      stderr "process $f is running"
    else
      mv -f $f $f.$$
      cat $f.$$ >>all.$$
      rm -f $f.$$
    fi
  done

  if test -e all.$$; then
    generate_semaphore_clean
    $clean all.$$
    rm -f all.$$
  fi
fi
