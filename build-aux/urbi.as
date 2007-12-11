#					-*- shell-script -*-

m4_pattern_allow([^URBI_SERVER$])

m4_divert_text([URBI-INIT],
[
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