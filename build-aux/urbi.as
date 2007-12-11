#					-*- shell-script -*-

m4_pattern_allow([^URBI_SERVER$])

m4_divert_text([URBI-INIT],
[
# find_srcdir WITNESS
# -------------------
# Find the location of the src directory of the tests suite.
# Make sure the value is correct by checking for the presence of WITNESS.
find_srcdir ()
{
  # Guess srcdir if not defined.
  if test -z "$srcdir"; then
    # Try to compute it from $0.
    srcdir=$(dirname "$0")
  fi

  # We may change directory.
  srcdir=$(absolute "$srcdir")

  # Check that srcdir is valid.
  test -f "$srcdir/$1" ||
    fatal "cannot find $srcdir/$1: define srcdir"

  echo "$srcdir"
}


# find_top_builddir [POSSIBILITIES]
# ---------------------------------
# Set/check top_builddir.
# - $top_builddir if the user wants to define it,
# - ../..         for the common case where we're in tests/my-test.dir
# - ..            if we're in tests/
# - .             if we're in top-builddir.
find_urbi_server ()
{
  if test $# = 0; then
    set ../.. .. .
  fi

  if test x"$top_builddir" = x; then
    for d
    do
       if test -f "$d/libtool"; then
         top_builddir=$d
         break
       fi
    done
  fi
  
  test -n "$top_builddir" ||
    fatal "cannot find top build directory, define top_builddir"
  
  test -d "$top_builddir" ||
    fatal "top_builddir is not a directory: $top_builddir"
  
  test -f "$top_builddir/config.status" ||
    fatal "top_builddir does not contain config.status: $top_builddir"
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
       URBI_SERVER=$(find_prog "urbi-server" "$PATH")
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