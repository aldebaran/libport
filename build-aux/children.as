#					-*- shell-script -*-

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

  {
    ("$[@]") >$run_prefix.out 2>$run_prefix.err
    local sta=$?
    echo $sta >$run_prefix.sta

    case $sta in
	0) ;;
	*) title="$title FAIL ($sta)";;
    esac

    echo "$[@]"> $run_prefix.cmd

    rst_subsection "$me: $title"
    rst_run_report "$title" "$run_prefix"

    return $(cat $run_prefix.sta)
  }
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


# children_register NAME
# ----------------------
children_register ()
{
  children="$children $[1]"
  echo $! >$[1].pid
}


# children_alive
# --------------
# Return whether there are still childs running.
children_alive ()
{
  local cids
  for i in $children
  do
    if ! ps $(cat $i.pid) 2>&1 >/dev/null; then
      return 1
    fi
  done
  return 0
}


# children_kill
# -------------
# Kill all the children.  This function can be called twice: once
# before cleaning the components, and once when exiting, so it's
# robust to children no longer in the process table.
children_kill ()
{
  for i in $children
  do
    pid=$(cat $i.pid)
    if ps $pid 2>&1 >/dev/null; then
      echo "Killing $i ($pid)"
      kill -ALRM $pid 2>&1 || true
    fi
  done
}


# children_harvest
# ----------------
# Report the exit status of the children.
children_harvest ()
{
  # Harvest exit status.
  for i in $children
  do
    pid=$(cat $i.pid)
    # Beware of set -e.
    if wait $pid 2>&1; then
      sta=$?
    else
      sta=$?
    fi
    echo "$sta$(ex_to_string $sta)" >$i.sta
  done
}


# children_wait TIMEOUT
# ---------------------
# Wait for the registered children, and passed TIMEOUT, kill the remaining
# ones.  TIMEOUT is increased by 5 if instrumenting.
children_wait ()
{
  local timeout=$[1]
  if $INSTRUMENT; then
    timeout=$(($timeout * 5))
  fi
  while children_alive; do
    if test $timeout -le 0; then
      children_kill
      break
    fi
    sleep 1
    timeout=$(($timeout - 1))
  done
}

])
