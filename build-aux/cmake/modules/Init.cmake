##
## Init.cmake: This file is part of libport.
## Copyright (C) 2006-2008, Gostai S.A.S.
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
set(INIT_CMAKE TRUE)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
include(CTest)
include(Tools)
include(ProjectInfo)
hostname(BUILDER_HOSTNAME)
set(BUILDFARM $ENV{BUILDFARM})
include(GenLibLoader)
include(GostaiInfo)
include(Gostai)
include(GostaiHintDirs)
gostai_init_hint_dirs()

# Add clean-install target
configure_file(
  ${CMAKE_MODULE_PATH}/clean-install-target.cmake.in
  ${CMAKE_BINARY_DIR}/clean-install-target.cmake
  @ONLY
  )
add_custom_target(clean-install
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/clean-install-target.cmake
  COMMENT "Clean installation directory."
  )

# Add re-install target
add_custom_target(re-install
  COMMAND ${CMAKE_BUILD_TOOL} install
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Re-install")
add_dependencies(re-install clean-install)

# Add check target
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Checking the project..."
  )

# Add bf-test target
add_custom_target(bf-test
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process -VV
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Checking the project on the buildfarm slave..."
  )

endif(NOT DEFINED INIT_CMAKE)
