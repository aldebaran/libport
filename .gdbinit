# These lines are tricks to work with the source files of the gdb extensions
# instead of the installed files.  This gives opportunity to develop new
# extensions for the debugger without making new installs.
python libport_source_dir = ''
python sched_source_dir = ''
source share/gdb/python/libport.so-gdb.py
source share/gdb/python/libsched.so-gdb.py
