##
## GostaiHintDirs.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2009.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT DEFINED GOSTAI_HINT_DIRS_CMAKE)
  set(GOSTAI_INIT_HINT_DIRS_CMAKE TRUE)

# Initialize file containing the list of hint directories used to set up PATH
# environment variable before to execute a program or to run a test on
# platform without
# RPATH support. It is also useful when searching for dependent dynamic
# libraries to ship.
macro(gostai_init_hint_dirs)
  set(GOSTAI_HINT_DIRS_FILENAME "${CMAKE_BINARY_DIR}/gostai-hint-dirs.txt")
  file(REMOVE ${GOSTAI_HINT_DIRS_FILENAME})
endmacro(gostai_init_hint_dirs)

function(gostai_add_hint_dir dir)
  file(APPEND ${GOSTAI_HINT_DIRS_FILENAME} "${dir}\n")
endfunction(gostai_add_hint_dir)

function(gostai_read_hint_dirs dirs)
  file(STRINGS ${GOSTAI_HINT_DIRS_FILENAME} l)
  list(REMOVE_DUPLICATES l)
  set(${dirs} ${l} PARENT_SCOPE)
endfunction(gostai_read_hint_dirs)

endif(NOT DEFINED GOSTAI_HINT_DIRS_CMAKE)
