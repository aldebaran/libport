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


m4_defun([_URBI_PREPARE],
[_URBI_ABSOLUTE_PREPARE
_URBI_FIND_PROG_PREPARE
])


# URBI_PREPARE
# ------------
# Output all the M4sh possible initialization into the initialization
# diversion.
m4_defun([URBI_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_PREPARE])])
