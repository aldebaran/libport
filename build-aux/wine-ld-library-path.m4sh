						-*- shell-script -*-
m4_pattern_allow([^URBI_SERVER_FLAGS$])
URBI_INIT()[

# Exit asap.
host=@host@
case $host in
  (*pw32*|*mingw32*);;
  (*) verbose "nothing to do on $host"
      exit 0;;
esac


## --------------------- ##
## Auxiliary functions.  ##
## --------------------- ##

]URBI_GET_OPTIONS(
[  (-f|--force)   force=true;;
   (-h|--help)    usage;;
   (-t|--to)      shift; where=$1;;
   (-w|--wrapper) shift; process_wrapper "$1";;
   (-L|--ldflags) shift; process_ldflags "$1";;
   (*)            windir "$1";;
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
  -f, --force          force the creation of wine/, even if it exists
                       previous content is lost
  -t, --to=DIR         install the \`wine' directory inside DIR
                       in other words, make WINEPREFIX=DIR/wine
                       [$where]
  -w, --wrapper=FILE   also add the DIRs required to load the libraries
                       used by the Libtool executable wrapper FILE
                       multiple invocations accumulate
  -L, --ldflags=FLAGS  decode dependencies from FLAGS
                       (typically \$(AM_LDFLAGS))
EOF
    exit 0
}


# process_ldflags LDFLAGS
# -----------------------
process_ldflags ()
{
  local ldflags=$1
  local i
  for i in $ldflags
  do
    case $i in
     (-L*)  windir "${i#-L}";;
     (*.la) # FIXME: Get the dependencies in Libtool library wrappers.
            windir "$(echo "$i" | sed -e 's,[^/]*$,.libs,')";;
    esac
  done
}


# process_wrapper WRAPPER
# -----------------------
# Get the PATH spec from the libtool WRAPPER, and put its components
# in the output path.
process_wrapper ()
{
  local wrapper=$1
  if test -f "$wrapper"; then
    local save_IFS=$IFS
    IFS=:
    windir $(sed -ne 's/^ *PATH=//p' "$wrapper")
    IFS=$save_IFS
  else
    verbose "no such file or directory: $wrapper"
  fi
}


# windir DIR...
# -------------
# Add DIR (converted into windows style) to $windirs.
windir ()
{
  local i
  for i
  do
    # Add all the directories, some might not yet exist (e.g. .libs)
    # but will be created when running wine.  Nonexisting directories
    # are not a problem anyway.
    windirs+=";$(winepath -w "$i")"
  done
}

# create_wine_directory DIR
# -------------------------
create_wine_directory ()
{
  local dir=$1
  verbose "creating $dir"
  rm -rf "$dir"
  mkdir -p "$dir"
  local winehome=$HOME/.wine
  cp -a $winehome/dosdevices $winehome/*.reg $winehome/config "$dir"
  # Symlink everything from the .wine directory just in case.
  local f
  for f in $winehome/*
  do
    test -a "$dir/$(basename $f)" ||
      ln -s $f "$dir"
  done

  # Add all the added path to the Path variable in user.reg config
  # file.  That idiotic file may have multiple "Path=".  Only one
  # will have ';'.

  # A safe means to pass information to Perl: envvars.
  export windirs
  perl -pi                                                              \
    -e 'BEGIN { $dirs=$ENV{windirs}; $dirs =~ s/\\/\\\\/g; }'           \
    -e 's/"Path"="(.*;.*)"/"Path"="\1$dirs"/' "$dir/user.reg"
}

# Be lazy.
force=false
# By default, installed this here (--to).
where=$(pwd)
# List of directories to add to the Path, in Windows format.
# Each directory is preceded by a ';'.
windirs=

get_options $WINE_LD_LIBRARY_PATH_ARGS "$@"
verbose "host: $host"

# We used to check that we are not running Cygwin underneath.  Does
# not seem to make any sense here, and MN does not remember why he
# checked that.  So this check is removed.
wineprefix=$where/wine
if $force || test ! -d "$wineprefix"; then
  create_wine_directory "$wineprefix"
fi
verbose "WINEPREFIX='$wineprefix'"
echo "WINEPREFIX='$wineprefix'"

exit 0
]
