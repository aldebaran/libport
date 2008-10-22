##
## Flex.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Provide macro to build flex sources.
# Examples:
#  ADD_FLEX_FILES(my_project_SRCS myFlexFile.ll myFlexFile.l)


# Find flex on the system.
# Set FLEX_EXECUTABLE to the executable name.
function(FIND_FLEX)
  if(NOT FLEX_EXECUTABLE)
    find_program(FLEX_EXECUTABLE flex)
    if (NOT FLEX_EXECUTABLE)
      message(FATAL_ERROR "flex not found - aborting")
    endif (NOT FLEX_EXECUTABLE)
  endif(NOT FLEX_EXECUTABLE)
endfunction(FIND_FLEX)

# Add flex files to the source list.
function(ADD_FLEX_FILES _sources)
  find_flex()
  foreach(_current_FILE ${ARGN})
    get_filename_component(_in ${_current_FILE} ABSOLUTE)
    get_filename_component(_basename ${_current_FILE} NAME_WE)

    set(intermediate lex.yy.c)
    set(_out ${CMAKE_CURRENT_BINARY_DIR}/flex_${_basename}.cpp)
    # Invoke flex.
    add_custom_command(
      OUTPUT ${intermediate}
      COMMAND ${FLEX_EXECUTABLE}
      ARGS
      ${_in}
      DEPENDS ${_in}
      )
    # Copy intermediate file to output file.
    add_custom_command(
      OUTPUT ${_out}
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy ${intermediate} ${_out}
      DEPENDS ${intermediate}
      )
    # Add the generated source file to the source list.
    set(${_sources} ${${_sources}} ${_out} PARENT_SCOPE)
  endforeach(_current_FILE)
endfunction(ADD_FLEX_FILES)
