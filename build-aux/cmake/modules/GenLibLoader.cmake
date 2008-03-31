##
## GenLibLoader.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND GEN_LIB_LOADER)

include(CheckCMakeVarsExists)

# Generate a wrapper script, for _target_, that sets
# LD_LIBRARY_PATH/DYLD_LIBRARY_PATH/PATH to the prefix/lib directory according
# to the platform. Extra directories maybe given as extra arguments. The
# generated wrapper script is schedule for installation.
function(GEN_LIB_LOADER target)

  check_cmake_vars_exists(CMAKE_AUX_DIR)

  # Extension of the script.
  if(UNIX)
    set(input "${CMAKE_MODULE_PATH}/ld-wrapper-linux.sh.in")
    set(ext ".sh")
  else(UNIX)
    set(ext ".bat")
  endif(UNIX)
  # Output script file name.
  string(REGEX REPLACE "^(.*)\\.[^.]+$" "\\1" output ${target})
  set(output "${CMAKE_CURRENT_BINARY_DIR}/${output}${ext}")
  # Add target
  configure_file(${input} ${output} ESCAPE_QUOTES @ONLY)
  message(STATUS "Configure '${input}' to '${output}'")
  # Let clean target remove it.
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${output})
  # Install the script
  INSTALL(PROGRAMS ${output} DESTINATION bin)
endfunction(GEN_LIB_LOADER target)

endif(NOT COMMAND GEN_LIB_LOADER)
