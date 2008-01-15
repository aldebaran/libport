#					-*- shell-script -*-
##
## children.as: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##


m4_defun([URBI_CHILDREN_PREPARE],
[

# run TITLE COMMAND...
# --------------------
# Run the COMMAND... (which may use its stdin) and output detailed
# logs about the execution.
#
# Leave un $run_prefix.{cmd, out, sta, err, val} the command, standard
# output, exit status, standard error, and instrumentation output.
run_counter=0
run_prefix=
run ()
{
  local title=$[1]
  run_prefix=$run_counter-$(echo $[1] |
			     sed -e 's/[[^a-zA-Z0-9][^a-zA-Z0-9]]*/-/g;s/-$//')
  run_counter=$(($run_counter + 1))
  shift

  echo "$[@]"> $run_prefix.cmd

  # Beware of set -e.
  local sta
  if "$[@]" >$run_prefix.out 2>$run_prefix.err; then
    sta=0
  else
    sta=$?
    title="$title FAIL ($sta)"
  fi
  echo $sta >$run_prefix.sta

  rst_subsection "$me: $title"
  rst_run_report "$title" "$run_prefix"

  return $sta
}



## ------------------------------------------------- ##
## PID sets -- Lower layer for children management.  ##
## ------------------------------------------------- ##


# pids_alive PIDS
# ---------------
# Return whether some of the PIDs point to alive processes,
# i.e., return 1 iff there are no children alive.
pids_alive ()
{
  local pid
  for pid
  do
    # Using "ps PID" to test whether a processus is alive is,
    # unfortunately, non portable.  OS X Tiger always return 0, and
    # outputs the ps-banner and the line of the processus (if there is
    # none, it outputs just the banner).  On Cygwin, "ps PID" outputs
    # everything, and "ps -p PID" outputs the banner, and the process
    # line if alive.  In both cases it exits with success.
    #
    # We once used grep to check the result:
    #
    # ps -p $pid | grep ["^[	]*$pid[^0-9]"]
    #
    # Unfortunately sometimes there are flags displayed before the
    # process number.  Since we never saw a "ps -p PID" that does not
    # display the title line, we expect two lines.
    case $(ps -p $pid | wc -l | sed -e '[s/^[	 ]*//]') in
      1) # process is dead.
	 ;;
      2) # Process is live.
	 return 0;;
      *) error SOFTWARE "unexpected ps output:" "$(ps -p $pid)" ;;
    esac
  done
  return 1
}


# pids_kill PIDS
# --------------
# Kill all the PIDS.  This function can be called twice: once
# before cleaning the components, and once when exiting, so it's
# robust to children no longer in the process table.
pids_kill ()
{
  local pid
  for pid
  do
    if pids_alive $pid; then
      local name=$(cat $pid.name)
      echo "Killing $name (kill -ALRM $pid)"
      kill -ALRM $pid 2>&1 || true
    fi
  done
}


# pids_wait TIMEOUT PIDS
# ----------------------
pids_wait ()
{
  local timeout=$[1]
  shift

  if $INSTRUMENT; then
    timeout=$(($timeout * 5))
  fi

  while pids_alive "$[@]"; do
    if test $timeout -le 0; then
      pids_kill "$[@]"
      break
    fi
    sleep 1
    timeout=$(($timeout - 1))
  done
}




## ---------- ##
## Children.  ##
## ---------- ##


# rst_run_report $TITLE $FILE-PREFIX
# ----------------------------------
# Report the input and output for $FILE-PREFIX.
rst_run_report ()
{
  local title=$[1]
  case $title in
    ?*) title="$title ";;
  esac

  rst_pre "${title}Command"   $[2].cmd
  rst_pre "${title}Pid"       $[2].pid
  rst_pre "${title}Status"    $[2].sta
  rst_pre "${title}Input"     $[2].in
  rst_pre "${title}Output"    $[2].out
  rst_pre "${title}Error"     $[2].err
  rst_pre "${title}Valgrind"  $[2].val
}


# children_alive [CHILDREN]
# -------------------------
# Return whether there are still some children running,
# i.e., return 1 iff there are no children alive.
children_alive ()
{
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local i
  for i
  do
    local pid=$(cat $i.pid)
    # Using "ps PID" to test whether a processus is alive is,
    # unfortunately, non portable.  OS X Tiger always return 0, and
    # outputs the ps-banner and the line of the processus (if there is
    # none, it outputs just the banner).  On Cygwin, "ps PID" outputs
    # everything, and "ps -p PID" outputs the banner, and the process
    # line if alive.  In both cases it exits with success.
    #
    # We once used grep to check the result:
    #
    # ps -p $pid | grep ["^[	]*$pid[^0-9]"]
    #
    # Unfortunately sometimes there are flags displayed before the
    # process number.  Since we never saw a "ps -p PID" that does not
    # display the title line, we expect two lines.
    case $(ps -p $pid | wc -l | sed -e '[s/^[	 ]*//]') in
      1) # process is dead.
	 ;;
      2) # Process is live.
	 return 0;;
      *) error SOFTWARE "unexpected ps output:" "$(ps -p $pid)" ;;
    esac
  done
  return 1
}


# children_clean [CHILDREN]
# --------------------------
# Remove the children files.
children_clean ()
{
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local i
  for i
  do
    rm -f $i.{cmd,pid,sta,in,out,err,val}
  done
}


# children_pid [CHILDREN]
# -----------------------
# Return the PIDs of the CHILDREN (whether alive or not).
children_pid ()
{
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local i
  for i
  do
    cat $i.pid
  done
}


# children_register NAME
# ----------------------
# It is very important that we do not leave some random exit status
# from some previous runs.  Standard output etc. are not a problem:
# they *are* created during the run, while *.sta is created *only* if
# it does not already exists (this is because you can call "wait" only
# once per processus, so to enable calling children_harvest multiple
# times, we create *.sta only if it does not exist).
#
# Creates NAME.pid which contains the PID, and PID.name which contains
# the name.
children_register ()
{
  test $[#] -eq 1
  rm -f $[1].sta
  children="$children $[1]"
  echo $! >$[1].pid
  echo $[1] >$!.name
}


# children_report [CHILDREN]
# --------------------------
# Produce an RST report for the CHILDREN.
children_report ()
{
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local i
  for i
  do
    rst_run_report "$i" "$i"
  done
}


# children_kill [CHILDREN]
# ------------------------
# Kill all the children.  This function can be called twice: once
# before cleaning the components, and once when exiting, so it's
# robust to children no longer in the process table.
children_kill ()
{
  pids_kill $(children_pid "$[@]")
}


# children_harvest [CHILDREN]
# ---------------------------
# Report the exit status of the children.  Can be run several times.
# You might want to call it once in the regular control flow to fetch
# some exit status, but also in a trap, if you were interrupted.  So
# it is meant to be callable multiple times, which might be a danger
# wrt *.sta (cf., children_register).
children_harvest ()
{
  # Harvest exit status.
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local i
  for i
  do
    # Don't look for the status of children we already waiting for.
    if ! test -e $i.sta; then
      local pid=$(cat $i.pid)
      # Beware of set -e.
      local sta
      if wait $pid 2>&1; then
	sta=$?
      else
	sta=$?
      fi
      echo "$sta$(ex_to_string $sta)" >$i.sta
    fi
  done
}


# children_status [CHILDREN]
# --------------------------
# Return the exit status of CHILD.  Must be run after harvesting.
# If several CHILD are given, return the highest exit status.
children_status ()
{
  test $[#] -ne 0 ||
    { set x $children; shift; }

  local res=0
  local i
  for i
  do
    # We need to have waited for these children.
    test -f $i.sta ||
      error SOFTWARE "children_status: cannot find $i.sta." \
		     "Was children_harvest called?"         \
		     "Maybe children_cleanup was already called..."
    local sta=$(sed -n '1{s/^\([[0-9][0-9]]*\).*/\1/;p;q;}' <$i.sta)
    if test $res -lt $sta; then
      res=$sta
    fi
  done
  echo $res
}


# children_wait TIMEOUT [CHILDREN]
# --------------------------------
# Wait for the registered children, and passed TIMEOUT, kill the remaining
# ones.  TIMEOUT is increased by 5 if instrumenting.
children_wait ()
{
  local timeout=$[1]
  shift
  pids_wait $timeout $(children_pid "$[@]")
}

])
