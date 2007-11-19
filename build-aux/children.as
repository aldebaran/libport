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
  run_counter=$((run_counter + 1))
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
  local title=$1
  case $title in
    ?*) title="$title "
	;;
  esac

  rst_pre "${title}Command"   $2.cmd
  rst_pre "${title}Pid"       $2.pid
  rst_pre "${title}Status"    $2.sta
  rst_pre "${title}Input"     $2.in
  rst_pre "${title}Output"    $2.out
  rst_pre "${title}Error"     $2.err
  rst_pre "${title}Valgrind"  $2.val
}


# register_child NAME
# -------------------
register_child ()
{
  children="$children $[1]"
  echo $! >$[1].pid
}


# kill_children
# -------------
# Kill all the children.  This function can be called twice: once
# before cleaning the components, and once when exiting.
kill_children ()
{
  for i in $children
  do
    pid=$(cat $i.pid)
    if ps $pid 2>&1; then
      echo "Killing $i ($pid)"
      kill $pid 2>&1 || true
    fi
  done
}


# harvest_children
# ----------------
# Report the exit status of the children.
harvest_children ()
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

])