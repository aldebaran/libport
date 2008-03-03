##
## FindQScintilla2.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Find QScintilla2 library.
#
# You can set QSCINTILLA2_PATH to a custom path to search in.
#
# This module set two variables:
#
#   QSCINTILLA2_LIBRARY: the path to the libraries found.
#   QSCINTILLA2_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

# Search for the library.
if(NOT QSCINTILLA2_LIBRARY)
  find_library(QSCINTILLA2_LIBRARY
    NAMES qscintilla2
    PATHS ${QT_LIBRARY_DIR} ${QSCINTILLA2_PATH}/lib)
  if(QSCINTILLA2_LIBRARY)
    message(STATUS "Found qscintilla2 library: ${QSCINTILLA2_LIBRARY}")
  else(QSCINTILLA2_LIBRARY)
    message(FATAL_ERROR "no qscintilla2 library found "
      "(setting -DQSCINTILLA2_PATH=/path/to/qscintilla2 may solve this "
      "problem).")
  endif(QSCINTILLA2_LIBRARY)
endif(NOT QSCINTILLA2_LIBRARY)

# Search for the include directory.
if(NOT QSCINTILLA2_INCLUDE_DIR)
  find_path(QSCINTILLA2_INCLUDE_DIR
    NAMES Qsci
    PATHS ${QT_INCLUDE_DIR} ${QSCINTILLA2_PATH}/include)
  if(QSCINTILLA2_INCLUDE_DIR)
    message(STATUS "Found qscintilla2 include directory: "
      "${QSCINTILLA2_INCLUDE_DIR}")
  else(QSCINTILLA2_INCLUDE_DIR)
    message(FATAL_ERROR "no qscintilla2 headers found "
      "(setting -DQSCINTILLA2_PATH=/path/to/qscintilla2 may solve this "
      "problem).")
  endif(QSCINTILLA2_INCLUDE_DIR)
endif(NOT QSCINTILLA2_INCLUDE_DIR)
