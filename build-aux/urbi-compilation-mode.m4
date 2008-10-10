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
  local stacksize=128
  local mode
  for mode
  do
    case $mode in
      (build)
        compiler_flags -O0
        ;;

      (cov)
        compiler_flags -fprofile-arcs -ftest-coverage
        ;;

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
        AC_SUBST([BISON_FLAGS], ["$BISON_FLAGS -Dassert"])
        # Define USE_VALGRIND only if valgrind/valgrind.h exists.
        AC_CHECK_HEADER([valgrind/valgrind.h],
                        [CPPFLAGS="$CPPFLAGS -DUSE_VALGRIND"])
        stacksize=1024
        ;;

      (final)
        urbi_compilation_mode_set ndebug symbols
        ;;

      (ndebug)
        CPPFLAGS="$CPPFLAGS -DNDEBUG"
        ;;

      (prof)
        compiler_flags -pg
        ;;

      (space)
        compiler_flags -Os -fomit-frame-pointer \
                       -fdata-sections -ffunction-sections
        LDFLAGS="$LDFLAGS --gc-sections"
        urbi_compilation_mode_set final
        ;;

      (speed)
        compiler_flags -O3
        AC_DEFINE([SPEED], [1],
                  [Define to optimize for speed the kernel at the detriment of compilation time.])
        urbi_compilation_mode_set final
        ;;

      (symbols)
        AC_DEFINE([SYMBOLS_PRECOMPILED], 1,
                  [Define if Urbi symbols should be precompiled])
        ;;

      (threads)
        AC_DEFINE([SCHEDULER_CORO_OSTHREAD], [1],
                  [Define to use the OS-thread implementation of coroutines])
        ;;

      (*)
        AC_MSG_ERROR([invalid compilation mode: $mode])
        ;;
    esac
  done
  AC_DEFINE_UNQUOTED([URBI_KERNEL_STACK_SIZE], [$stacksize],
                     [Default coroutine stack size in kB])
}

URBI_ARGLIST_ENABLE([enable-compilation-mode=MODE],
               [Compilation mode],
               [build|cov|debug|ndebug|final|prof|space|speed|symbols|threads],
               [debug],
               [
     MODE: comma-separated list of:
        Overall Levels:
          - build: Disable optimization for faster compilation.
          - debug: Enable debug information.
          - space: Optimize for smaller space (implies final).
          - speed: Optimize for speed (implies final).

        Finer grain components:
          - cov: Request code coverage instrumentation
          - ndebug: Define NDEBUG.
          - final: Implies ndebug and symbols
          - prof: Request profiling instrumentation
          - symbols: Activate precompiled-symbols.
          - threads: Implement coroutines with threads.])

AC_MSG_CHECKING([for compilation mode])
urbi_compilation_mode=$(echo $enable_compilation_mode | tr ',' ' ')
AC_MSG_RESULT([$urbi_compilation_mode])
urbi_compilation_mode_set $urbi_compilation_mode
])

## Local Variables:
## mode: Autoconf
## End:
