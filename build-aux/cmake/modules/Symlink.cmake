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

## Create a symlink of OLD in NEW.
## For Windows, copy instead of symlinking.

if(NOT COMMAND SYMLINK)

  function(SYMLINK OLD NEW)
    ## Prepare
    ## -------
    ## cmake create_symlink does not support syntax "à la" 
    ## `ln [-fs] source_file ... target_dir',
    ## so we emulate it.

    if(IS_DIRECTORY ${OLD})
      string(REGEX REPLACE "/+$" "" OLD ${OLD})
      set(OLD "${OLD}/")
    endif(IS_DIRECTORY ${OLD})
    if(NEW MATCHES "/$")
      if(IS_DIRECTORY ${OLD})
        string(REGEX MATCH "/[^/]+/$" FILENAME ${OLD})
      else(IS_DIRECTORY ${OLD})
        string(REGEX MATCH "/[^/]+$" FILENAME ${OLD})
      endif(IS_DIRECTORY ${OLD})
      if(FILENAME STREQUAL "")
        message(SEND_ERROR "Failed to extract filename from ${OLD}")
      endif(FILENAME STREQUAL "")
      string(REPLACE "/" "" FILENAME ${FILENAME})
      set(NEW "${NEW}/${FILENAME}")
    endif(NEW MATCHES "/$")
    if(EXISTS ${NEW})
      message("-- Skipping symlink `${OLD}' to `${NEW}': ${NEW} already exists")
      return()
    endif(EXISTS ${NEW})
    message("-- *** Symlinking `${OLD}' to `${NEW}'")

    ## Do
    if(UNIX)
      execute_process(COMMAND ${CMAKE_COMMAND}
        -E create_symlink
        "${OLD}" "${NEW}"
        )
    else(UNIX)
      if(IS_DIRECTORY ${OLD})
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy_directory
          "${OLD}" "${NEW}"
          )
      else(IS_DIRECTORY ${OLD})
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy
          "${OLD}" "${NEW}"
          )
      endif(IS_DIRECTORY ${OLD})
    endif(UNIX)

    ## Check
    if(NOT EXISTS ${NEW})
      message(SEND_ERROR "!!! Symlink creating failed!")
    endif(NOT EXISTS ${NEW})

  endfunction(SYMLINK)

endif(NOT COMMAND SYMLINK)
