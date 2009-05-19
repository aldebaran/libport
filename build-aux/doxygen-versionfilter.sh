#!/bin/sh
##
## doxygen-versionfilter.sh: This file is part of build-aux.
## Copyright (C) 2006-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##


file=$1
if [ x"$file" = x ]; then
  echo "usage: $0 <file>"
  exit 1
fi

if [ ! -f "$file" ]; then
  echo "no such file '$file'"
  exit 1
fi

dir=`dirname "$file"`
if [ -d "$dir/.svn" ]; then
  svn info --xml "$file" | sed '/.*revision="/!d;s//r/;s/".*//;q'
elif [ -d "$GIT_DIR" ]; then
  dot_git=`dirname "$GIT_DIR"`
  file=`echo "$file" | sed "s,^$dot_git,.,"`
  git log --pretty=format:%h --max-count=1 "$file"
elif [ -d "$dir/CVS" ]; then
  cvs status "$file" | sed -n 's/^[ \]*Working revision:[ 	]*\([0-9][0-9\.]*\).*/\1/p'
else

  # Try git (find the .git directory)
  where=`pwd`
  old_where=
  dot_git=
  while [ x"$old_where" != x"$where" ]; do
    old_where=`pwd`
    test -d .git && export dot_git=`pwd` && break
    cd ..
    where=`pwd`
  done
  if [ -d "$dot_git" ]; then
    export GIT_DIR="$dot_git/.git"
    file=`echo "$file" | sed "s,^$dot_git,.,"`
    rev=`git log --pretty=format:%h --max-count=1 "$file" 2>&1`
    case "$rev" in
      fatal*) ;;
      *) echo $rev; exit 0 ;;
    esac
  fi

  exit 1
fi
