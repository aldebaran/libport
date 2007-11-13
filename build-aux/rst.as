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

# rst_section TITLE
# -----------------
rst_section ()
{
   echo "$[@]" | sed 'p;s/./=/g;p;g'
}

# rst_subsection TITLE
# --------------------
rst_subsection ()
{
  echo "$[@]" | sed 'p;s/./-/g;p;g'
}

# rst_tab [FILES = stdin]
# -----------------------
rst_tab ()
{
  sed -e 's/^/	/' "$[@]"
}

# rst_pre TITLE [FILE]
# --------------------
# FILE may be empty to denote stdin.
rst_pre ()
{
  if test $# = 1 || test -s "$[2]"; then
    echo "$[1]::"
    echo
    shift
    rst_tab "$[@]"
    echo
  fi
}


# rst_expect EXPECTED EFFECTIVE
# -----------------------------
# Compare expected output with effective, actual, output.
# Set $exit to false if there is a failure.
rst_expect ()
{
  rst_subsection "$me: $[2]"
  if ! diff -u --label="Expected $[1] ($[1].exp)" $[1].exp  \
	       --label="Effective $[1] ($[2].eff)" $[2].eff \
	       >$[2].diff; then
    rst_pre "Expected $[1] for $me"      $[1].exp
    rst_pre "Raw effective $[2] for $me" $[2].raw
    rst_pre "Effective $[2] for $me"     $[2].eff
    rst_pre "Diffs on $[2] for $me"      $[2].diff
    if test x"$exit" = xtrue; then
      exit=false
    fi
  else
    # Dump something, it is really surprising in the logs to see
    # nothing.
    rst_pre "Raw effective $[2] for $me" $[2].raw
  fi
}
])

m4_defun([URBI_RST_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_RST_PREPARE])])
