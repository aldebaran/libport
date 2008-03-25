##
## CopyDependentLibraries.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND COPY_DEPENDENT_LIBRARIES)

include(CheckCMakeVarsExists)

# Copy dependent libraries of _target_ in target-dldep and schedule them for
# installation. It uses dldep.
function(COPY_DEPENDENT_LIBRARIES target)
  check_cmake_vars_exists(CMAKE_AUX_DIR)
  set(out "${target}-dldep")
  set(generator ${CMAKE_AUX_DIR}/dldep)
  add_custom_target(
    ${out} ALL
    COMMAND ${generator} ${target} ${out}
    DEPENDS ${target} ${generator}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "${CMAKE_COMMAND} -E cmake_echo_color "
    "--switch=${CMAKE_MAKEFILE_COLOR} --cyan "
    "Copying dependent libraries of '${target}'."
    VERBATIM
    )
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${out})
  # The trailing backslash is need to avoid to have the last component of the
  # path installed too.
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${out}/ DESTINATION lib)
endfunction(COPY_DEPENDENT_LIBRARIES)

endif(NOT COMMAND COPY_DEPENDENT_LIBRARIES)
