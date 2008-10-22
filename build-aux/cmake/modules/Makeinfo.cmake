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
  set(ext "")
else()
  set(ext ".html")
endif()
set(MAKEINFO_OUTEXT ${ext} CACHE INTERNAL "Extension of texinfo output file.")
set(ext)

# Compile a texinfo file. Typically, readme, news, knownbugs, etc...
# Expect input file as first arguement and output as second. Finally, a list
# of path to add to the include path list.
function(MAKEINFO_FILE input output includepaths)
  # Set options depending on the system.
  set(html_opts --html --no-split --no-headers)
  set(txt_opts --plaintext)
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
    #HACK: On cygwin/win32 with mingw make, makeinfo.exe 4.8 do understand
    #      input/output file c:/like/that but not path to include
    #      directories. So, we convert c: to /cygdrive/c
    if(WIN32)
      string(REGEX REPLACE "^[cC]:" "/cygdrive/c" dir ${i})
    else(WIN32)
      set(dir ${i})
    endif(WIN32)
    set(opts ${opts} -I ${dir})
  endforeach(i)
  # Add command
  # FIXME: We do not use add_custom_command because CPack.cmake needs the
  # output file at configuration time.
  # (see http://public.kitware.com/Bug/view.php?id=6674)
#   add_custom_command(
#     OUTPUT ${output}
#     COMMAND ${MAKEINFO_EXECUTABLE}
#     ARGS ${opts} ${input} -o ${output}
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     DEPENDS ${input} ${MAKEINFO_EXECUTABLE}
#     COMMENT "${CMAKE_COMMAND} -E cmake_echo_color "
#     "--switch=${CMAKE_MAKEFILE_COLOR} --cyan "
#     "Building '${output}'"
#     VERBATIM
#     )
  execute_process(
    COMMAND ${MAKEINFO_EXECUTABLE} ${opts} ${input} -o ${output}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    TIMEOUT 30
    RESULT_VARIABLE ret
    ERROR_VARIABLE err
    )
  if(ret)
    message(SEND_ERROR
      "'${MAKEINFO_EXECUTABLE}' failed to build '${input}' to '${output}' "
      "(with options '${opts}' from '${CMAKE_CURRENT_SOURCE_DIR}')")
    message(STATUS "Makeinfo error begins:\n${err}\nMakeinfo error ends.")
  else(ret)
    message(STATUS "Makeinfo '${opts}' '${input}' to '${output}'")
  endif(ret)
endfunction(MAKEINFO_FILE)

endif(NOT COMMAND MAKEINFO_FILE)
