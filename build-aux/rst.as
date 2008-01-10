						-*- shell-script -*-

## --------------- ##
## Rst functions.  ##
## --------------- ##

# These functions produce their result on stdout.

m4_defun([_URBI_RST_PREPARE],
[# rst_title TITLE
# ---------------
rst_title ()
{
   echo "$[@]" | sed 's/.*/   &   /;h;s/./=/g;p;x;p;g;p;s/.*//'
}

# rst_section_ LINER TITLE
# ------------------------
rst_section_ ()
{
   local liner=$[1]
   shift
   echo "$[@]" | sed "p;s/./$liner/g;p;g"
   echo
}

# rst_section TITLE
# -----------------
rst_section ()
{
  rst_section_ "=" "$[@]"
}

# rst_subsection TITLE
# --------------------
rst_subsection ()
{
  rst_section_ "-" "$[@]"
}

# rst_subsubsection TITLE
# -----------------------
rst_subsubsection ()
{
  rst_section_ "." "$[@]"
}

# rst_tab [FILES = stdin]
# -----------------------
rst_tab ()
{
  sed -e 's/^/	/' "$[@]"
  echo
}

# rst_pre TITLE [FILE]
# --------------------
# FILE may be empty to denote stdin.
rst_pre ()
{
  if test $[#] -eq 1 || test -s "$[2]"; then
    echo "$[1]::"
    echo
    shift
    rst_tab "$[@]"
  fi
}


# rst_expect EXPECTED EFFECTIVE
# -----------------------------
# Compare expected output with effective, actual, output.
# Set $exit to false if there is a failure.
rst_expect ()
{
  rst_subsubsection "$me: $[2]"
  if ! diff -u --label="Expected $[1] ($[1].exp)" $[1].exp  \
	       --label="Effective $[1] ($[2].eff)" $[2].eff \
	       >$[2].diff; then
    rst_pre "Expected $[1] for $me"      $[1].exp
    rst_pre "Raw effective $[2] for $me" $[2].raw
    rst_pre "Effective $[2] for $me"     $[2].eff
    rst_pre "Diffs on $[2] for $me"      $[2].diff
    # If exit is already set, e.g. to "exit SKIP", don't alter it.
    case $exit in
      '' | true) exit=false;;
    esac
  else
    # Dump something, it is really surprising in the logs to see
    # nothing.
    rst_pre "Raw effective $[2] for $me" $[2].raw
  fi
}
])

m4_defun([URBI_RST_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_RST_PREPARE])])
