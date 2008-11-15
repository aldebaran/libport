##                                                         -*- shell-script -*-
## base.as: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

m4_pattern_forbid([^_?URBI_])dnl

# m4_define([_m4_divert(M4SH-INIT)],   5)
m4_define([_m4_divert(URBI-INIT)],    10)

m4_defun([_URBI_ABSOLUTE_PREPARE],
[
# is_absolute PATH
# ----------------
is_absolute ()
{
  case $[1] in
    ([[\\/]]* | ?:[[\\/]]*) return 0;;
    (                    *) return 1;;
  esac
}

# absolute NAME -> ABS-NAME
# -------------------------
# Return an absolute path to NAME.
absolute ()
{
  local res
  AS_IF([is_absolute "$[1]"],
  [# Absolute paths do not need to be expanded.
   res=$[1]],
  [local dir=$(pwd)/$(dirname "$[1]")
   if test -d "$dir"; then
     res=$(cd "$dir" 2>/dev/null && pwd)/$(basename "$[1]")
   else
     fatal "absolute: not a directory: $dir"
   fi])

  # On Windows, we must make sure we have a Windows-like UNIX-friendly path (of
  # the form C:/cygwin/path/to/somewhere instead of /path/to/somewhere, notice
  # the use of forward slashes in the Windows path.  Windows *does* understand
  # paths with forward slashes).
  case $(uname -s) in
    (CYGWIN*) res=$(cygpath "$res")
  esac
  echo "$res"
}
])


m4_defun([_URBI_FIND_PROG_PREPARE],
[# find_file FILE PATH
# -------------------
# Return full path to FILE in PATH ($PATH_SEPARATOR separated),
# nothing if not found.
find_prog ()
{
  _AS_PATH_WALK([$[2]],
                [AS_IF([test -f $as_dir/$[1]],
                       [echo "$as_dir/$[1]"; break])])
}

# xfind_file PROG PATH
# --------------------
# Same as xfind_prog, but don't take "no" for an answer.
xfind_file ()
{
  local res
  res=$(find_prog "$[1]" "$[2]")
  test -n "$res" ||
    error OSFILE "cannot find $[1] in $[2]"
  echo "$res"
}


# find_prog PROG [PATH=$PATH]
# ---------------------------
# Return full path to PROG in PATH ($PATH_SEPARATOR separated),
# nothing if not found.
find_prog ()
{
  local path=$PATH
  test -z "$[2]" || path=$[2]

  _AS_PATH_WALK([$path],
                [AS_IF([AS_TEST_X([$as_dir/$[1]])],
                       [echo "$as_dir/$[1]"; break])])
}

# xfind_prog PROG [PATH=$PATH]
# ----------------------------
# Same as xfind_prog, but don't take "no" for an answer.
xfind_prog ()
{
  local path=$PATH
  test -z "$[2]" || path=$[2]
  local res
  res=$(find_prog "$[1]" "$path")
  test -n "$res" ||
    error OSFILE "cannot find executable $[1] in $path"
  echo "$res"
}

# require_thing TEST-FLAG ERROR_MESSAGE THING [HINT]
# --------------------------------------------------
require_thing ()
{
  local flag=$[1]
  local error="$[2]: $[3]"
  local thing=$[3]
  shift 3
  test $flag "$thing" ||
    fatal OSFILE "$error" "$[@]"
}

# require_dir DIR [HINT]
# ----------------------
require_dir ()
{
  require_thing -d "no such directory" "$[@]"
}

# require_file FILE [HINT]
# ------------------------
require_file ()
{
  require_thing -f "no such file" "$[@]"
}

])


m4_defun([_URBI_STDERR_PREPARE],
[
# stderr LINES
# ------------
stderr ()
{
  local i
  for i
  do
    echo >&2 "$as_me: $i"
  done
  echo >&2
}


# error EXIT MESSAGES
# -------------------
error ()
{
  local exit=$[1]
  shift
  stderr "$[@]"
  ex_exit $exit
}

# fatal MESSAGES
# --------------
fatal ()
{
  # To help the user, just make sure that she is not confused between
  # the prototypes of fatal and error: the first argument is unlikely
  # to be integer.
  case $[1] in
    (*[[!0-9]]*|'') ;;
    (*)  stderr "warning: possible confusion between fatal and error" \
                "fatal $[*]";;
  esac

  error 1 "$[@]"
}

# ex_to_string EXIT
# -----------------
# Return a decoding of EXIT status if available, nothing otherwise.
ex_to_string ()
{
  case $[1] in
     (  0) echo ' (EX_OK: successful termination)';;
     ( 64) echo ' (EX_USAGE: command line usage error)';;
     ( 65) echo ' (EX_DATAERR: data format error)';;
     ( 66) echo ' (EX_NOINPUT: cannot open input)';;
     ( 67) echo ' (EX_NOUSER: addressee unknown)';;
     ( 68) echo ' (EX_NOHOST: host name unknown)';;
     ( 69) echo ' (EX_UNAVAILABLE: service unavailable)';;
     ( 70) echo ' (EX_SOFTWARE: internal software error)';;
     ( 71) echo ' (EX_OSERR: system error (e.g., cannot fork))';;
     ( 72) echo ' (EX_OSFILE: critical OS file missing)';;
     ( 73) echo ' (EX_CANTCREAT: cannot create (user) output file)';;
     ( 74) echo ' (EX_IOERR: input/output error)';;
     ( 75) echo ' (EX_TEMPFAIL: temp failure; user is invited to retry)';;
     ( 76) echo ' (EX_PROTOCOL: remote error in protocol)';;
     ( 77) echo ' (EX_NOPERM: permission denied)';;
     ( 78) echo ' (EX_CONFIG: configuration error)';;
     (176) echo ' (EX_SKIP: skip this test with unmet dependencies)';;
     (177) echo ' (EX_HARD: hard error that cannot be saved)';;
     (242) echo ' (killed by Valgrind)';;
     (  *) if test 127 -lt $[1]; then
            echo " (SIG$(kill -l $[1] || true))"
          fi;;
  esac
}

# ex_to_int CODE
# --------------
# Decode the CODE and return the corresponding int.
ex_to_int ()
{
  case $[1] in
    (OK         |EX_OK)          echo 0;;
    (USAGE      |EX_USAGE)       echo 64;;
    (DATAERR    |EX_DATAERR)     echo 65;;
    (NOINPUT    |EX_NOINPUT)     echo 66;;
    (NOUSER     |EX_NOUSER)      echo 67;;
    (NOHOST     |EX_NOHOST)      echo 68;;
    (UNAVAILABLE|EX_UNAVAILABLE) echo 69;;
    (SOFTWARE   |EX_SOFTWARE)    echo 70;;
    (OSERR      |EX_OSERR)       echo 71;;
    (OSFILE     |EX_OSFILE)      echo 72;;
    (CANTCREAT  |EX_CANTCREAT)   echo 73;;
    (IOERR      |EX_IOERR)       echo 74;;
    (TEMPFAIL   |EX_TEMPFAIL)    echo 75;;
    (PROTOCOL   |EX_PROTOCOL)    echo 76;;
    (NOPERM     |EX_NOPERM)      echo 77;;
    (CONFIG     |EX_CONFIG)      echo 78;;

    (SKIP       |EX_SKIP)        echo 176;;
    (HARD       |EX_HARD)        echo 177;;

    (*)                          echo $[1];;
  esac
}

ex_exit ()
{
  exit $(ex_to_int $[1])
}
])

# URBI_GET_OPTIONS
# ----------------
# Generate get_options().
m4_define([URBI_GET_OPTIONS],
[# Parse command line options
get_options ()
{
  while test $[#] -ne 0; do
    case $[1] in
        (--*=*)
            opt=$(echo "$[1]" | sed -e 's/=.*//')
            val=$(echo "$[1]" | sed -e ['s/[^=]*=//'])
            shift
            set dummy "$opt" "$val" ${1+"$[@]"};
            shift
            ;;
    esac

    case $[1] in
      $@
    esac
    shift
  done
}
])


m4_defun([_URBI_PREPARE],
[
# truth TEST-ARGUMENTS...
# -----------------------
# Run "test TEST-ARGUMENTS" and echo true/false depending on the result.
truth ()
{
  if test "$[@]"; then
    echo true
  else
    echo false
  fi
}

_URBI_ABSOLUTE_PREPARE
_URBI_FIND_PROG_PREPARE
_URBI_STDERR_PREPARE

])


# URBI_PREPARE
# ------------
# Output all the M4sh possible initialization into the initialization
# diversion.
m4_defun([URBI_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_PREPARE])dnl
URBI_RST_PREPARE()dnl
URBI_INSTRUMENT_PREPARE()dnl
URBI_CHILDREN_PREPARE()dnl
])


# URBI_INIT
# ---------
# Replaces the AS_INIT invocation.
# Must be defined via "m4_define", not "m4_defun" since it is AS_INIT
# which will set up the diversions used for "m4_defun".
m4_define([URBI_INIT],
[AS_INIT()dnl
URBI_PREPARE()
set -e
case $VERBOSE in
  (x) set -x;;
esac

: ${abs_builddir='@abs_builddir@'}
: ${abs_srcdir='@abs_srcdir@'}
: ${abs_top_builddir='@abs_top_builddir@'}
: ${abs_top_srcdir='@abs_top_srcdir@'}
: ${builddir='@builddir@'}
: ${srcdir='@srcdir@'}
: ${top_builddir='@top_builddir@'}
: ${top_srcdir='@top_srcdir@'}
])
