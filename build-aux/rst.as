						-*- shell-script -*-

## --------------- ##
## Rst functions.  ##
## --------------- ##

m4_defun([_URBI_RST_PREPARE],
[rst_title ()
{
   echo "$[@]" | sed 's/.*/   &   /;h;s/./=/g;p;x;p;g;p;s/.*//'
}

rst_section ()
{
   echo "$[@]" | sed 'p;s/./=/g;p;g'
}

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

# pre TITLE [FILE]
# ----------------
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
])

m4_defun([URBI_RST_PREPARE],
[m4_divert_text([M4SH-INIT], [_URBI_RST_PREPARE])])
