#					-*- shell-script -*-

m4_pattern_allow([^URBI_SERVER$])

m4_divert_text([URBI-INIT],
[
# check_dir VARIABLE WITNESS
# --------------------------
check_dir ()
{
  local var=$[1]
  local val
  eval "val=\$$[1]"
  test x"$val" != x ||
    fatal "undefined variable: $var"
  # Normalize the directory name, and as safety belts, run the same
  # tests on it.  But save time if possible.
  AS_IF([is_absolute "$val"],
        [set x $val],
        [set x $val $(absolute $val)])
  shift
  for res
  do
    test -e "$res" ||
      fatal "$var does not exist: $res" "(pwd = $(pwd))"
    test -d "$res" ||
      fatal "$var is not a directory: $res" "(pwd = $(pwd))"
    if x"$[2]" != x; then
      test -f "$res/$[2]" ||
	fatal "$var does not contain $[2]: $res" "(pwd = $(pwd))"
    fi
  done
  eval "$var='$res'"
}

# find_srcdir WITNESS
# -------------------
# Find the location of the src directory of the tests suite.
# Make sure the value is correct by checking for the presence of WITNESS.
find_srcdir ()
{
  # Guess srcdir if not defined.
  if test -z "$srcdir"; then
    # Try to compute it from $[0].
    srcdir=$(dirname "$[0]")
  fi

  check_dir srcdir "$[1]"
}


# find_top_builddir [POSSIBILITIES]
# ---------------------------------
# Set/check top_builddir.
# - $top_builddir   if the user wants to define it,
# - (dirname $0)/.. since that's where we are supposed to be
# - ../..           for the common case where we're in tests/my-test.dir
# - ..              if we're in tests/
# - .               if we're in top-builddir.
find_top_builddir ()
{
  if test x"$top_builddir" = x; then
    if test $[#] = 0; then
      set $(dirname "$0")/.. ../.. .. .
    fi
    for d
    do
       if test -f "$d/config.status"; then
	 top_builddir=$d
	 break
       fi
    done
  fi

  check_dir top_builddir "config.status"
}


# find_urbi_server
# ----------------
# Set URBI_SERVER to the location of urbi-server.
find_urbi_server ()
{
  case $URBI_SERVER in
    '')
       # If URBI_SERVER is not defined, try to find one.  If we are in
       # $top_builddir/tests/TEST.dir, then look in $top_builddir/src.
       URBI_SERVER=$(find_prog "urbi-server" \
			       "$top_builddir/src${PATH_SEPARATOR}.")
       ;;

    *[[\\/]]* ) # A path, relative or absolute.  Make it absolute.
       URBI_SERVER=$(absolute "$URBI_SERVER")
       ;;

    *) # A simple name, most certainly urbi-server.
       # Find it in the PATH.
       URBI_SERVER=$(find_prog "$URBI_SERVER" "$PATH")
       # If we can't find it, then ucore-pc was probably not installed.
       # Skip.
       test x"$URBI_SERVER" != x || exit 77
       ;;
  esac

  if test -z "$URBI_SERVER"; then
    fatal "cannot find urbi-server, please define URBI_SERVER"
  fi

  if test ! -f "$URBI_SERVER"; then
    fatal "cannot find urbi-server, please check \$URBI_SERVER: $URBI_SERVER"
  fi

  # Check its version.
  if ! run "Server version" "$URBI_SERVER" --version >&3; then
    fatal "cannot run $URBI_SERVER --version"
  fi
}
])
