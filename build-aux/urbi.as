m4_pattern_forbid([^_?URBI_])dnl


m4_defun([_URBI_ABSOLUTE_PREPARE],
[# absolute NAME -> ABS-NAME
# -------------------------
# Return an absolute path to NAME.
absolute ()
{
  local res
  case $[1] in
   [[\\/]]* | ?:[[\\/]]*)
      # Absolute paths don't need to be expanded.
      res=$[1]
      ;;
   *) local dir=$(pwd)/$(dirname "$[1]")
      if test -d "$dir"; then
	res=$(cd "$dir" 2>/dev/null && pwd)/$(basename "$[1]")
      else
	fatal "not a directory: $dir"
      fi
      ;;
  esac

  # On Windows, we must make sure we have a Windows-like UNIX-friendly path (of
  # the form C:/cygwin/path/to/somewhere instead of /path/to/somewhere, notice
  # the use of forward slashes in the Windows path.  Windows *does* understand
  # paths with forward slashes).
  case $(uname -s) in
    CYGWIN*) res=$(cygpath --mixed "$res")
  esac
  echo "$res"
}
])


m4_defun([_URBI_FIND_PROG_PREPARE],
[# find_prog PROG PATH
# -------------------
# Return full path to PROG in PATH ($PATH_SEPARATOR separated),
# nothing if not found.
find_prog ()
{
  _AS_PATH_WALK([$[2]],
                [AS_IF([AS_TEST_X([$as_dir/$[1]])],
		       [echo "$as_dir/$[1]"; break])])
}
])


m4_defun([_URBI_STDERR_PREPARE],
[stderr ()
{
  echo >&2 "$as_me: $[@]"
  echo >&2
}


# error EXIT MESSAGE
# ------------------
error ()
{
  local exit=$[1]
  shift
  stderr "$[@]"
  ex_exit $exit
}

# fatal MESSAGE
# -------------
fatal ()
{
  error 1 "$[@]"
}

# ex_to_string EXIT
# -----------------
# Return a decoding of EXIT status if available, nothing otherwise.
ex_to_string ()
{
  case $[1] in
      0)  echo ' (EX_OK: successful termination)';;
      64) echo ' (EX_USAGE: command line usage error)';;
      65) echo ' (EX_DATAERR: data format error)';;
      66) echo ' (EX_NOINPUT: cannot open input)';;
      67) echo ' (EX_NOUSER: addressee unknown)';;
      68) echo ' (EX_NOHOST: host name unknown)';;
      69) echo ' (EX_UNAVAILABLE: service unavailable)';;
      70) echo ' (EX_SOFTWARE: internal software error)';;
      71) echo ' (EX_OSERR: system error (e.g., cannot fork))';;
      72) echo ' (EX_OSFILE: critical OS file missing)';;
      73) echo ' (EX_CANTCREAT: cannot create (user) output file)';;
      74) echo ' (EX_IOERR: input/output error)';;
      75) echo ' (EX_TEMPFAIL: temp failure; user is invited to retry)';;
      76) echo ' (EX_PROTOCOL: remote error in protocol)';;
      77) echo ' (EX_NOPERM: permission denied)';;
      78) echo ' (EX_CONFIG: configuration error)';;
      242)echo ' (killed by Valgrind)';;
      *)  if test 127 -lt $[1]; then
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
    OK		|EX_OK)          echo 0;;
    USAGE	|EX_USAGE)       echo 64;;
    DATAERR	|EX_DATAERR)     echo 65;;
    NOINPUT	|EX_NOINPUT)     echo 66;;
    NOUSER	|EX_NOUSER)      echo 67;;
    NOHOST	|EX_NOHOST)      echo 68;;
    UNAVAILABLE	|EX_UNAVAILABLE) echo 69;;
    SOFTWARE	|EX_SOFTWARE)    echo 70;;
    OSERR	|EX_OSERR)       echo 71;;
    OSFILE	|EX_OSFILE)      echo 72;;
    CANTCREAT	|EX_CANTCREAT)   echo 73;;
    IOERR	|EX_IOERR)       echo 74;;
    TEMPFAIL	|EX_TEMPFAIL)    echo 75;;
    PROTOCOL	|EX_PROTOCOL)    echo 76;;
    NOPERM	|EX_NOPERM)      echo 77;;
    CONFIG	|EX_CONFIG)      echo 78;;
    *)                           echo $[1];;
  esac
}

ex_exit ()
{
  exit $(ex_to_int $[1])
}
])


m4_defun([_URBI_PREPARE],
[_URBI_ABSOLUTE_PREPARE
_URBI_FIND_PROG_PREPARE
_URBI_STDERR_PREPARE])


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
