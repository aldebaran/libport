# Copyright (C) 2008-2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.

me=$(basename "$0")

stderr ()
{
  local i
  for i
  do
    echo >&2 "$me (wrapper): $i"
  done
}

run ()
{
  verbose "run: $*"
  "$@"
}

error ()
{
  local exit="$1"
  shift
  stderr "$@"
  exit $exit
}

verbose ()
{
  case "$verbose: $VERBOSE " in
    (true:* | *" $me "* | *" x "*)
      stderr "$@"
      ;;
  esac
}

fatal ()
{
  error 1 "$@"
}

# Look for, and load, `msvc_env.sh'.
load_msvc_env ()
{
  local i
  # Skip if we can't run it.
  for i in ~/share/wine ~/.wine/drive_c "$(dirname "$0")" ~/bin
  do
    local f="$i/msvc_env.sh"
    verbose "trying to load $f"
    if test -f "$f"; then
      . "$f"
      verbose "loaded $f"
      return 0
    fi
  done
  return 1
}

# setup
# -----
# Load our configuration script and set up $repath; $VS_PATH,
# $VCXX_PATH and $VCXX_BIN (Unix paths).
setup ()
{
  load_msvc_env ||
    error 176 "cannot load msvc_env.sh"

  repaths='cygpath winepath'
  for repath in $repaths ''
  do
    $repath --version >/dev/null 2>&1 &&
      break
  done
  test -n "$repath" ||
    error 176 "program not found: $repaths"

  VS_PATH=$($repath -u "$VSINSTALLDIR")
  VCXX_PATH=$($repath -u "$VCINSTALLDIR")
  VCXX_BIN=$VCXX_PATH/bin

  test -x "$VCXX_BIN/cl.exe" ||
    error 176 "cl.exe not found in $VCXX_BIN"
}

