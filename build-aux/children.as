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
    ("$[@]") >$run_prefix.out 2>$run_rst_prefix.err
    local sta=$?
    echo $sta >$run_prefix.sta

    case $sta in
	0) ;;
	*) title="$title FAIL ($sta)";;
    esac
    rst_subsection "$me: $title"

    echo
    echo "$[@]"> $run_prefix.cmd
    rst_pre "Command"   $run_prefix.cmd
    rst_pre "Output"    $run_prefix.out
    rst_pre "Status"    $run_prefix.sta
    rst_pre "Error"     $run_prefix.err
    rst_pre "Valgrind"  $run_prefix.val

    return $(cat $run_prefix.sta)
  }
}



## ---------- ##
## Children.  ##
## ---------- ##

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