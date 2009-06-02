#
# urbi-prog-cxx.m4: This file is part of build-aux.
# Copyright (C) 2006-2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])
m4_pattern_forbid([^TC_])

AC_PREREQ([2.60])

AC_DEFUN([URBI_COMPILATION_MODE],
[# Compilation mode.
urbi_compilation_mode_set ()
{
  local stacksize=128
  local mode
  for mode
  do
    case $mode in
      (build)
        URBI_APPEND_COMPILERFLAGS([-O0])
        ;;

      (cov)
        URBI_APPEND_COMPILERFLAGS([-fprofile-arcs -ftest-coverage])
        ;;

      (debug)
        COMPILATION_MODE_DEBUG=true
        URBI_APPEND_COMPILERFLAGS([-O2 -ggdb])
        # Not all the code includes config.h.
        URBI_APPEND_CPPFLAGS([-DURBI_DEBUG])
        case $CXX_FLAVOR in
          (gcc-apple) ;; # Does not work with OS'X native GCC-4.2.
          (*) URBI_APPEND_CPPFLAGS([-D_GLIBCXX_DEBUG]);;
        esac
        AC_DEFINE([URBI_DEBUG], [1],
                  [Define to enable Urbi debugging tools.])
        AC_DEFINE([YYDEBUG], [1],
                  [Define to enable parser runtime debug traces.])
        URBI_APPEND_FLAGS([BISONFLAGS], [-Dparse.assert -Dparse.trace])
        # Define USE_VALGRIND only if valgrind/valgrind.h exists.
        AC_CHECK_HEADER([valgrind/valgrind.h],
                        [URBI_APPEND_CPPFLAGS([-DUSE_VALGRIND])])
        stacksize=1024
        ;;

      (final)
        urbi_compilation_mode_set ndebug symbols
        ;;

      (ndebug)
        URBI_APPEND_CPPFLAGS([-DNDEBUG])
        ;;

      (prof)
        URBI_APPEND_COMPILERFLAGS([-pg])
        ;;

      (space)
        COMPILATION_MODE_SPACE=true
        AC_DEFINE([COMPILATION_MODE_SPACE], [1],
                  [Define to 1 to optimize for space.])
        local i
        for i in -Os -fomit-frame-pointer -fdata-sections -ffunction-sections
        do
          TC_COMPILER_OPTION_IF([$i],
                                [URBI_APPEND_COMPILERFLAGS([$i])])
        done
        TC_COMPILER_OPTION_IF([--gc-sections],
                              [URBI_APPEND_FLAGS([LDFLAGS], [--gc-sections])])
        AC_SUBST([YYERROR_VERBOSE], [false])
        urbi_compilation_mode_set final
        ;;

      (speed)
        URBI_APPEND_COMPILERFLAGS([-O3])
        COMPILATION_MODE_SPEED=true
        AC_DEFINE([COMPILATION_MODE_SPEED], [1],
                  [Define to 1 to optimize for speed the kernel
                   at the detriment of compilation time.])
        urbi_compilation_mode_set final
        ;;

      (symbols)
        AC_DEFINE([SYMBOLS_PRECOMPILED], [1],
                  [Define if Urbi symbols should be precompiled.])
        ;;

      (threads)
        AC_DEFINE([SCHED_CORO_OSTHREAD], [1],
                  [Define to use the OS-thread implementation of coroutines.])
        ;;

      (*)
        AC_MSG_ERROR([invalid compilation mode: $mode])
        ;;
    esac
  done
  AC_DEFINE_UNQUOTED([URBI_KERNEL_STACK_SIZE], [$stacksize],
                     [Default coroutine stack size in kB.])

  # Whether we build for small space.
  AM_CONDITIONAL([COMPILATION_MODE_DEBUG], [$COMPILATION_MODE_DEBUG])
  AM_CONDITIONAL([COMPILATION_MODE_SPACE], [$COMPILATION_MODE_SPACE])
  AM_CONDITIONAL([COMPILATION_MODE_SPEED], [$COMPILATION_MODE_SPEED])
}

URBI_ARGLIST_ENABLE([enable-compilation-mode=MODE],
               [Compilation mode],
               [build|cov|debug|ndebug|final|prof|space|speed|symbols|threads],
               [debug],
               [
     MODE: comma-separated list of:
        Overall Levels:
          - build: Disable optimization for faster compilation.
          - debug: Enable debug information, and scanner/parser traces.
          - space: Optimize for smaller space (implies final).
          - speed: Optimize for speed (implies final).

        Finer grain components:
          - cov: Request code coverage instrumentation
          - ndebug: Define NDEBUG.
          - final: Implies ndebug and symbols
          - prof: Request profiling instrumentation
          - symbols: Activate precompiled-symbols.
          - threads: Implement coroutines with threads.])

AC_SUBST([YYERROR_VERBOSE], [true])
AC_SUBST([COMPILATION_MODE_DEBUG], [false])
AC_SUBST([COMPILATION_MODE_SPACE], [false])
AC_SUBST([COMPILATION_MODE_SPEED], [false])
AC_MSG_CHECKING([for compilation mode])
urbi_compilation_mode=$(echo $enable_compilation_mode | tr ',' ' ')
AC_MSG_RESULT([$urbi_compilation_mode])
urbi_compilation_mode_set $urbi_compilation_mode
])

## Local Variables:
## mode: Autoconf
## End:
