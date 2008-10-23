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

include(Dirs)

if(UNIX AND NOT WIN32 AND NOT STGZ_SHORTCUTS)
macro(STGZ_SHORTCUTS)
  parse_arguments(
    "STGZ_SHORTCUTS"
    "DATA_DIR;PATCH_FILENAME;FILE_EXTENSION"
    "DESKTOP;MENU"
    ${ARGN})

  set(STGZ_XDG_FILENAME
      "${GOSTAI}-${PROJECT_NAME}"
      )
  set(PATCH_PATH
      "${STGZ_SHORTCUTS_DATA_DIR}/${STGZ_SHORTCUTS_PATCH_FILENAME}"
      )

  configure_file(
    ${CMAKE_MODULE_PATH}/create_shortcut.sh.in
    ${CMAKE_BINARY_DIR}/create_shortcut.sh
    @ONLY
    )

  add_custom_target(package_patch ALL
    COMMAND sh -c
      'grep
         '"'"'patch -i .*${STGZ_SHORTCUTS_PATCH_FILENAME}$$'"'"' Makefile
       || \(sed -i -e 
         '"'"'s=^.PHONY : package$$=\\tpatch -i ${PATCH_PATH}\\n&='"'"'
         Makefile
       && echo \"Please, relaunch me\"\; exit 42\)'
    COMMENT "Patching package rule"
    )
  install(PROGRAMS ${CMAKE_BINARY_DIR}/create_shortcut.sh
    DESTINATION ${DATA_DIR}
    )
  install(FILES "${STGZ_SHORTCUTS_DATA_DIR}/${STGZ_XDG_FILENAME}.png"
    DESTINATION ${DATA_DIR}
    )

endmacro(STGZ_SHORTCUTS)
endif(UNIX AND NOT WIN32 AND NOT STGZ_SHORTCUTS)
