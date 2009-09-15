#
# urbi-with-telispeech.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_WITH_TELISPEECH
# ----------
# Look for teliSpeech.  Support --with-telispeech=PATH. Check the software
# location.
#
# Output variable TELISPEECH_PATH point to the installation location.
# Set "$telispeech" to "true" or "false".
AC_DEFUN([URBI_WITH_TELISPEECH],
[AC_ARG_WITH([telispeech],
	     [AC_HELP_STRING([--with-telispeech=telispeech-path],
			     [set the path to teliSpeech [/opt/telisma/teliSpeech]])],
	     [], [with_telispeech=no])

case $with_telispeech in
   no) telispeech=false
       ;;
  yes) telispeech=true
       TELISPEECH_PATH=/opt/telisma/teliSpeech
       ;;
    *) telispeech=true
       TELISPEECH_PATH=$with_telispeech
       ;;
esac

# Checking whether Telispeech is really there.
AC_MSG_CHECKING([for teliSpeech])
if $telispeech; then
  if (test -f "$TELISPEECH_PATH/bin/libtasr_so.so"); then
    AC_MSG_RESULT([$TELISPEECH_PATH])
    telispeech=true
  else
    AC_MSG_RESULT([no, telispeech not found in $TELISPEECH_PATH])
    telispeech=false
  fi
else
  AC_MSG_RESULT([no])
  telispeech=false
fi

if $telispeech; then
  # More tools for teliSpeecj.
  AC_SUBST([TELISPEECH_CPPFLAGS], ['-I$(TELISPEECH_PATH)/inc'])
  AC_SUBST([TELISPEECH_LDFLAGS],  ['-L$(TELISPEECH_PATH)/bin -ltasr_so -lTLogger_so -lxsystem_so -licudata -licuuc -lxerces-c -lsisr_so -lAcousticTool_so -laudio_processing_so -lxml2'])
fi

AM_CONDITIONAL([WITH_TELISPEECH], [$telispeech])
AC_SUBST([TELISPEECH_PATH])
])

AC_DEFUN([URBI_TELISPEECH_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_TELISPEECH])

   case $telispeech in
     '' | false)
       AC_MSG_ERROR([Failed to find teliSpeech. Please install it or check config.log])
     ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
