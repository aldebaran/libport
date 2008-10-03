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

include(Tools)

# Generate a wrapper script, for _target_, that sets
# LD_LIBRARY_PATH/DYLD_LIBRARY_PATH/PATH to the prefix/lib directory according
# to the platform. Extra directories maybe given as extra arguments. The
# generated wrapper script is schedule for installation.
function(GEN_LIB_LOADER target)

  check_cmake_vars_exists(BINARIES_DIR EXTRA_LIBRARIES_DIR)

  # Extension of the script.
  if(UNIX)
    set(input "${CMAKE_MODULE_PATH}/ld-wrapper-linux.sh.in")
    set(ext ".sh")
  else(UNIX)
    set(input "${CMAKE_MODULE_PATH}/ld-wrapper-win32.bat.in")
    set(ext ".bat")
  endif(UNIX)
  # Output script file name.
  get_target_property(target_loc ${target} LOCATION_${CMAKE_BUILD_TYPE})
  get_filename_component(target_path ${target_loc} PATH)
  string(REGEX REPLACE "^(.*)\\.[^.]+$" "\\1" output ${target})
  set(output "${target_path}/${output}${ext}")
  # Add target
  message(STATUS
    "Generate library loader script from '${input}' to '${output}'")
  configure_file(${input} ${output} ESCAPE_QUOTES @ONLY)
  # Let clean target remove it.
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${output})
  # Install the script
  INSTALL(PROGRAMS ${output} DESTINATION ${BINARIES_DIR})
endfunction(GEN_LIB_LOADER target)

endif(NOT COMMAND GEN_LIB_LOADER)
