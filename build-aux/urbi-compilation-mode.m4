AC_DEFUN([URBI_COMPILATION_MODE],
[
compiler_flags ()
{
  CFLAGS="$CFLAGS $[@]"
  CXXFLAGS="$CXXFLAGS $[@]"
}

# Compilation mode.
urbi_compilation_mode_set ()
{
  local mode
  for mode
  do
    case $mode in
      (debug)
        compiler_flags -O2 -ggdb
        # Not all the code includes config.h.
        CPPFLAGS="$CPPFLAGS -DURBI_DEBUG -D_GLIBCXX_DEBUG"
        AC_DEFINE([URBI_DEBUG], [1],
                  [Define to enable Urbi debugging tools.])
        AC_DEFINE([_GLIBCXX_DEBUG], [1],
                  [Define to enable STL debug mode.])
        AC_DEFINE([YYDEBUG], [1],
                  [Define to enable parser runtime debug traces.])
        # Pass USE_VALGRIND only if valgrind/valgrind.h exists.
        AC_CHECK_HEADER([valgrind/valgrind.h],
                        [AC_DEFINE([USE_VALGRIND], [1],
                                   [Define if <valgrind/valgrind.h> exists.])])
        ;;

      (fast)
        compiler_flags -O0
        ;;

      (ndebug)
        CPPFLAGS="$CPPFLAGS -DNDEBUG"
        ;;

      (space)
        compiler_flags -Os -fomit-frame-pointer \
                       -fdata-sections -ffunction-sections
        LDFLAGS="$LDFLAGS --gc-sections"
        ;;

      (speed)
        compiler_flags -O3
        urbi_compilation_mode_set ndebug symbols
        ;;

      (symbols)
        enable_precompiled_symbols=yes
        ;;

      (threads)
        CPPFLAGS="$CPPFLAGS -DSCHEDULER_CORO_OSTHREAD"
        ;;

      (*)
        AC_MSG_ERROR([invalid compilation mode: $mode])
        ;;
    esac
  done
}

#
# Levels:
#  - space: Optimize for smaller space (implies ndebug).
#  - speed: Optimize for speed (implies ndebug).
#  - debug: Enable debug information.
#  - fast: Enable fast compilation.
#
# Indididual components:
#  - ndebug: Define NDEBUG.
#  - symbols: Activate precompiled-symbols.
#  - threads: Implement coroutines with threads.
URBI_ARGLIST_ENABLE([compilation-mode],
                    [Compilaton mode],
                    [debug|fast|ndebug|space|speed|symbols|threads],
                    [debug])
AC_MSG_CHECKING([for compilation mode])
urbi_compilation_mode=$(echo $enable_compilation_mode | tr ',' ' ')
AC_MSG_RESULT([$urbi_compilation_mode])
urbi_compilation_mode_set $urbi_compilation_mode
])

## Local Variables:
## mode: Autoconf
## End:
