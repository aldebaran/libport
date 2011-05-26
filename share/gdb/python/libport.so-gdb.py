# Copyright (C) 2011, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.


# This file should be installed inside the same directory of the library in
# order to be relocatable.
#
# To disable the autoload, just add "" -xe 'set auto-load-scripts no' "" on
# gdb command line.

# This file is supposed to be installed inside /lib
libport_install_dir = '../'
if 'libport_source_dir' in globals():
    libport_prefix = libport_source_dir
else:
    libport_prefix = libport_install_dir

sys.path.insert(0, libport_prefix + 'share/gdb/python')
import libport
