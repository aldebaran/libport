						-*- shell-script -*-
m4_pattern_allow([^URBI_SERVER_FLAGS$])
URBI_INIT()[

## --------------------- ##
## Auxiliary functions.  ##
## --------------------- ##

]URBI_GET_OPTIONS(
[  (-h|--help)    usage;;
   (-t|--to)      shift; dir=$1;;
   (-w|--wrapper) shift; process_wrapper "$1";;
   (*)            add_directory "$1";;
])[

# Print usage
usage ()
{
  cat <<EOF
Usage: $0 [OPTIONS] [DIR]...

Return a string which is to be used to define the environment.  I.e.,
typically returns (on stdout) something like

    WINEPREFIX=/SOME/PATH

Options:
  -h, --help           display this message and exit
  -t, --to=DIR         install the \`wine' directory inside DIR
                       [$where]
  -w, --wrapper=FILE   use PATH as defined by the libtool wrapper FILE
                       several invocation accumulate
EOF
    exit 0
}


add_directory ()
{
  local dir=$1
  test -d "$dir" ||
    stderr "warning: $dir does not exist or is not a directory"
  dir=$(winepath -w "$dir")
  windirs="$windirs;$dir"
}

process_wrapper ()
{
  local wrapper=$1
  require_file "$wrapper"
  local path
  path=$(sed -ne 's/^ *PATH=//p')
  local save_IFS=$IFS
  IFS=:
  for p
  do
    IFS=$save_IFS
    add_directory "$p"
  done
}

set -x

# By default, installed this here.
where=$(pwd)
# List of directories to add to the Path.
windirs=
# If we need wine, setup it.
host=@host@

stderr "host: $host"

create_wine_directory ()
{
  local w=$where/wine
  stderr "creating $where/wine"
  rm -rf "$w"
  mkdir -p "$w"
  cp -a ~/.wine/dosdevices ~/.wine/*.reg ~/.wine/config "$w"
  # Symlink everything from the .wine directory just in case.
  for f in ~/.wine/*
  do
    bf=$(basename $f)
    if ! test -a "$w/$bf"; then
      ln -s $f "$w/$bf"
    fi
  done
  # Add all the added path to the Path variable in user.reg config
  # file.  That idiotic file may have multiple "Path=".  Only one
  # will have ';'.

  # A safe means to pass information to Perl: envvars.
  export windirs
  perl -pi                                                              \
    -e 'BEGIN { $dirs=$ENV{windirs}; $dirs =~ s/\\/\\\\/g; }'           \
    -e 's/"Path"="(.*;.*)"/"Path"="\1$dirs"/' "$w/user.reg"
}

if echo "$host" | grep -q pw32 \
     && uname -a | grep -v -q Cygwin \
     && test -z $WINEPREFIX; then
  test -d "$w" ||
    create_wine_directory
  export WINEPREFIX=$w
  stderr "WINEPREFIX='$WINEPREFIX'"
  echo "WINEPREFIX='$WINEPREFIX'"
fi
]

exit 0
