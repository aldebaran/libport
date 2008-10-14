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

# Generate a wrapper script, for _binary_, that sets
# LD_LIBRARY_PATH/DYLD_LIBRARY_PATH/PATH to the prefix/lib directory according
# to the platform. Extra directories maybe given as extra arguments. The
# generated wrapper script is schedule for installation.
# NOTE: _binary_ my be a target.
function(GEN_LIB_LOADER binary)

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
  if(TARGET ${binary})
    get_target_property(binary_loc ${binary} LOCATION_${CMAKE_BUILD_TYPE})
    if(WIN32)
      set(binary_name ${binary}.exe)
    else(WIN32)
      set(binary_name ${binary})
    endif(WIN32)
  else(TARGET ${binary})
    set(binary_loc ${CMAKE_CURRENT_BINARY_DIR})
    get_filename_component(binary_name ${binary} NAME)
  endif(TARGET ${binary})
  get_filename_component(binary_path ${binary_loc} PATH)
  string(REGEX REPLACE "^(.*)\\.[^.]+$" "\\1" output ${binary_name})
  set(output "${binary_path}/${output}${ext}")
  # Configure wrapper script.
  message(STATUS
    "Generate library loader script from '${input}' to '${output}'")
  configure_file(${input} ${output} ESCAPE_QUOTES @ONLY)
  # Let 'clean' target remove configured file.
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${output})
  # Install the script
  INSTALL(PROGRAMS ${output} DESTINATION ${BINARIES_DIR})
endfunction(GEN_LIB_LOADER binary)

endif(NOT COMMAND GEN_LIB_LOADER)
