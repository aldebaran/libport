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
include(Tools)
hostname(BUILDER_HOSTNAME)
set(BUILDFARM $ENV{BUILDFARM})
include(GenLibLoader)
include(GostaiInfo)

# Add clean-install target
configure_file(
  ${CMAKE_MODULE_PATH}/clean-install-target.cmake.in
  ${CMAKE_BINARY_DIR}/clean-install-target.cmake
  )
add_custom_target(clean-install
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/clean-install-target.cmake
  COMMENT "Clean installation directory."
  )

endif(NOT DEFINED INIT_CMAKE)
