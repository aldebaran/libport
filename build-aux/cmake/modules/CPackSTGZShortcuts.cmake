##
## CPackSTGZShortcuts.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

##
## ***PLEASE*** update your application README if you need this macro.
##
## * How to package this application
##
## To package this application on Linux, run ``make package_linux'' in your
## build directory.
##
## For any other OS, simply run ``make package''.
##

if(NOT COMMAND STGZ_SHORTCUTS)

include(Dirs)

function(STGZ_SHORTCUTS)
  parse_arguments(
    "STGZ_SHORTCUTS"
    "DATA_DIR;FILE_EXTENSION;MIME;CMD;CMD_ARGS;DESCRIPTION;DOCTYPE"
    "DESKTOP;MENU"
    ${ARGN})

  if(UNIX AND NOT WIN32)
    set(STGZ_XDG_FILENAME
      "${GOSTAI}-${PROJECT_NAME}"
      )
    if(NOT STGZ_SHORTCUTS_CMD_ARGS)
      set(STGZ_SHORTCUTS_CMD_ARGS "%f")
    endif(NOT STGZ_SHORTCUTS_CMD_ARGS)

    configure_file(
      ${CMAKE_MODULE_PATH}/create_shortcut.sh.in
      ${CMAKE_BINARY_DIR}/create_shortcut.sh
      @ONLY
      )
    configure_file(
      ${CMAKE_MODULE_PATH}/CPackSTGZShortcuts.patch.in
      ${CMAKE_BINARY_DIR}/CPackSTGZShortcuts.patch
      @ONLY
      )

    add_custom_target(package_linux
      COMMAND $(MAKE) package
      COMMAND patch -i CPackSTGZShortcuts.patch
      )
    install(PROGRAMS ${CMAKE_BINARY_DIR}/create_shortcut.sh
      DESTINATION ${DATA_DIR}
      )
    install(FILES "${STGZ_SHORTCUTS_DATA_DIR}/${STGZ_XDG_FILENAME}.png"
      DESTINATION ${DATA_DIR}
      )
  endif(UNIX AND NOT WIN32)

endfunction(STGZ_SHORTCUTS)

endif(NOT COMMAND STGZ_SHORTCUTS)
