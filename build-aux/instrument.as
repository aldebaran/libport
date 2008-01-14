##                                                        -*- shell-script -*-
## instrument.as: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##



m4_defun([_URBI_INSTRUMENT_PREPARE],
[normalize_boolean ()
{
  local var=$[1]
  local val
  eval val=\$$var
  case $val in
      yes|true|1) eval $var=true;;
      *)          eval $var=false;;
  esac
  eval stderr "$var=\$$var"
}


# COMMAND-PREFIX instrument LOG-FILE
# ----------------------------------
# Return what's to be prepended to an executable so that it is instrumented
# to be checked: Valgrind or Darwin's Malloc features.
#
# Use LOG-FILE as output file.
#
# Shared with uconsole-check, please keep sync.
instrument ()
{
  local log=$[1]
  case $INSTRUMENT:$(uname -s) in
      true:Darwin)
	  # Instrument to use Darwin's malloc features.  The man page
	  # says "MallocBadFreeAbort" exists, but setting MallocHelp
	  # reports about "MallocErrorAbort".  The former does not
	  # work, the latter does.
	  echo "env"				\
	      "MallocGuardEdges=1"		\
	      "MallocPreScribble=1"		\
	      "MallocScribble=1"		\
	      "MallocErrorAbort=1"		\
	      "MallocCheckHeapAbort=1"		\
	      "MallocLogFile=$log"
	  ;;
      true:*)
	  # Instrument using Valgrind.
	  : ${VALGRIND=valgrind}
	  if ($VALGRIND --version) >/dev/null 2>&1; then
	    local suppressions
	    if test -f $srcdir/valgrind-suppressions; then
	      suppressions="--suppressions=$srcdir/valgrind-suppressions"
	    fi
	    echo "$VALGRIND"			\
		 "--error-exitcode=242"		\
		 "--log-file-exactly=$log"	\
		 "$suppressions"		\
		 "--"
	  else
	    stderr "cannot find valgrind as $VALGRIND"
	  fi
	  ;;
      *)
	  # Don't instrument.
	  ;;
  esac
}

# FIXME: This should be issued farther in the code, not with the functions.
normalize_boolean INSTRUMENT
])



m4_defun([URBI_INSTRUMENT_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_INSTRUMENT_PREPARE])])
