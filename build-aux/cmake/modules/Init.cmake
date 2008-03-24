##
## Init.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Set common variables used in all Gostai project using cmake.

if(NOT DEFINED INIT_CMAKE)
set(INIT_CMAKE)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
enable_testing()
include(Hostname)
hostname(BUILDER_HOSTNAME)
include(PrintVar)
set(BUILDFARM $ENV{BUILDFARM})

endif(NOT DEFINED INIT_CMAKE)
