						-*- shell-script -*-
m4_pattern_allow([^URBI_SERVER_FLAGS$])
URBI_INIT()[

## --------------------- ##
## Auxiliary functions.  ##
## --------------------- ##

]URBI_GET_OPTIONS(
[  (-h|--help)    usage;;
   (-t|--to)      shift; where=$1;;
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

Arguments can also be passed via the envvar
\`WINE_LD_LIBRARY_PATH_ARGS'.  The DIRs are added to the pseudo
LD_LIBRARY_PATH that Windows supports.

Options:
  -h, --help           display this message and exit
  -t, --to=DIR         install the \`wine' directory inside DIR
                       in other words, make WINEPREFIX=DIR/wine
                       [$where]
  -w, --wrapper=FILE   also add the DIRs required to load the libraries
                       used by the Libtool wrapper FILE
                       multiple invocations accumulate
EOF
    exit 0
}

# add_directory DIR
# -----------------
# Add DIR to the output path.
add_directory ()
{
  local dir=$1
  if test -d "$dir"; then
    dir=$(winepath -w "$dir")
    windirs="$windirs;$dir"
  fi
}

# process_wrapper WRAPPER
# -----------------------
# Get the PATH spec from the libtool WRAPPER, and put its components
# in the output path.
process_wrapper ()
{
  local wrapper=$1
  require_file "$wrapper"
  local path
  path=$(sed -ne 's/^ *PATH=//p' "$wrapper")
  local save_IFS=$IFS
  IFS=:
  for p in $path
  do
    IFS=$save_IFS
    add_directory "$p"
  done
}

create_wine_directory ()
{
  verbose "creating $WINEPREFIX"
  rm -rf "$WINEPREFIX"
  mkdir -p "$WINEPREFIX"
  cp -a ~/.wine/dosdevices ~/.wine/*.reg ~/.wine/config "$WINEPREFIX"
  # Symlink everything from the .wine directory just in case.
  for f in ~/.wine/*
  do
    bf=$(basename $f)
    if ! test -a "$WINEPREFIX/$bf"; then
      ln -s $f "$WINEPREFIX/$bf"
    fi
  done
  # Add all the added path to the Path variable in user.reg config
  # file.  That idiotic file may have multiple "Path=".  Only one
  # will have ';'.

  # A safe means to pass information to Perl: envvars.
  export windirs
  perl -pi                                                              \
    -e 'BEGIN { $dirs=$ENV{windirs}; $dirs =~ s/\\/\\\\/g; }'           \
    -e 's/"Path"="(.*;.*)"/"Path"="\1$dirs"/' "$WINEPREFIX/user.reg"
}

# By default, installed this here.
where=$(pwd)
# List of directories to add to the Path.
windirs=
# If we need wine, setup it.
host=@host@

get_options $WINE_LD_LIBRARY_PATH_ARGS "$@"
verbose "host: $host"

# We used to check that we are not running Cygwin underneath.  Does
# not seem to make any sense here, and MN does not remember why he
# checked that.  So this check is removed.
case $host in
  (*pw32*|*mingw32*)
    export WINEPREFIX=$where/wine
    test -d "$WINEPREFIX" ||
      create_wine_directory
    verbose "WINEPREFIX='$WINEPREFIX'"
    echo "WINEPREFIX='$WINEPREFIX'"
    ;;
esac

exit 0
]
