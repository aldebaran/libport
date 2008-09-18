##
## Symlink.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

## Create a symlink of TARGET in DIRECTORY.
## For Windows, copy instead of symlinking.

if(NOT COMMAND SYMLINK)

  function(SYMLINK TARGET DIRECTORY)
    if(IS_DIRECTORY ${TARGET})
      set(TARGET "${TARGET}/")
    endif(IS_DIRECTORY ${TARGET})
    string(REGEX REPLACE "/+$" "" DIRECTORY ${DIRECTORY})
    if(UNIX)
      execute_process(COMMAND ${CMAKE_COMMAND}
        -E create_symlink
        "${TARGET}" "${DIRECTORY}"
        )
    else(UNIX)
      if(IS_DIRECTORY ${TARGET})
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy_directory
          "${TARGET}" "${DIRECTORY}"
          )
      else(IS_DIRECTORY)
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy
          "${TARGET}" "${DIRECTORY}"
          )
      endif(IS_DIRECTORY)
    endif(UNIX)
  endfunction(SYMLINK)

endif(NOT COMMAND SYMLINK)
