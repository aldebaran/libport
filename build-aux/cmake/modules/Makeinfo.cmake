##
## Makeinfo.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# This file provides function to make texinfo documents.
#
# Output functions:
#   MAKEINFO_FILE
#
# Output variables:
#  MAKEINFO_OUTEXT - Extension used for output files.


if(NOT COMMAND MAKEINFO_FILE)

find_package(Makeinfo REQUIRED)

# Set MAKEINFO_OUTEXT
if(UNIX AND NOT CYGWIN AND NOT APPLE)
  set(MAKEINFO_OUTEXT "")
else()
  set(MAKEINFO_OUTEXT ".html")
endif()

# Compile a texinfo file. Typically, readme, news, knownbugs, etc...
# Expect input file as first arguement and output as second. Finally, a list
# of path to add to the include path list.
function(MAKEINFO_FILE input output includepaths)
  # Set options depending on the system.
  set(html_opts "--html --no-split --no-headers")
  set(txt_opts "--plaintext")
  if(UNIX)
    if(CYGWIN)
      set(opts ${html_opts})
    else(CYGWIN)
      if(APPLE)
	set(opts ${html_opts})
      else(APPLE)
	set(opts ${txt_opts})
      endif(APPLE)
    endif(CYGWIN)
  else(UNIX)
    set(opts ${html_opts})
  endif(UNIX)
  # Compute include paths
  foreach(i ${includepaths})
    set(opts ${opts} -I ${i} )
  endforeach(i)
  # Add command
  add_custom_command(
    OUTPUT ${output}
    COMMAND ${MAKEINFO_EXECUTABLE}
    ARGS ${opts} ${input} -o ${output}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${input} ${MAKEINFO_EXECUTABLE}
    COMMENT "${CMAKE_COMMAND} -E cmake_echo_color "
    "--switch=${CMAKE_MAKEFILE_COLOR} --cyan "
    "Building '${output}'"
    VERBATIM
    )
  if(ret)
    message(SEND_ERROR
      "'${MAKEINFO_EXECUTABLE}' failed to build '${input}' to '${output}'")
    message(STATUS "Makeinfo error begins:\n${err}\nMakeinfo error ends.")
  endif(ret)
endfunction(MAKEINFO_FILE)

endif(NOT COMMAND MAKEINFO_FILE)
