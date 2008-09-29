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
# You can set QSCINTILLA2_ROOT_DIR to a custom path to search in.
#
# This module set two variables:
#
#   QSCINTILLA2_LIBRARY: the path to the libraries found.
#   QSCINTILLA2_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

set(QSCINTILLA2_FOUND FALSE)

# Search for the library.
if(NOT QSCINTILLA2_LIBRARY)
  find_library(QSCINTILLA2_LIBRARY
    NAMES qscintilla2
    PATHS ${QT_LIBRARY_DIR})
  if(QSCINTILLA2_LIBRARY)
    message(STATUS "Found qscintilla2 library: ${QSCINTILLA2_LIBRARY}")
    set(QSCINTILLA2_FOUND TRUE)
  else(QSCINTILLA2_LIBRARY)
    message(SEND_ERROR "no qscintilla2 library found "
      "(setting -DQSCINTILLA2_ROOT_DIR=/path/to/qscintilla2 may solve this "
      "problem).")
  endif(QSCINTILLA2_LIBRARY)
endif(NOT QSCINTILLA2_LIBRARY)

if(QSCINTILLA2_FOUND)
  # Search for the include directory.
  if(NOT QSCINTILLA2_INCLUDE_DIR)
    find_path(QSCINTILLA2_INCLUDE_DIR
      NAMES Qsci/qsciapis.h
      PATHS ${QT_INCLUDE_DIR})
    if(QSCINTILLA2_INCLUDE_DIR)
      message(STATUS "Found qscintilla2 headers directory: "
	"${QSCINTILLA2_INCLUDE_DIR}")
    else(QSCINTILLA2_INCLUDE_DIR)
      message(SEND_ERROR "no qscintilla2 headers found "
	"(setting -DQSCINTILLA2_ROOT_DIR=/path/to/qscintilla2 may solve this "
	"problem).")
      set(QSCINTILLA2_FOUND FALSE)
    endif(QSCINTILLA2_INCLUDE_DIR)
  endif(NOT QSCINTILLA2_INCLUDE_DIR)
endif(QSCINTILLA2_FOUND)

if(QSCINTILLA2_FOUND AND QSCINTILLA2_INCLUDE_DIR)
  # Check whether scintilla header are installed (needed because we write our
  # own lexer).
  find_path(QSCINTILLA2_SCINTILLA_INCLUDE_DIR
    NAMES Qsci/scintilla/include/Scintilla.h
    PATHS ${QSCINTILLA2_INCLUDE_DIR})
  if(QSCINTILLA2_SCINTILLA_INCLUDE_DIR)
    message(STATUS "Found scintilla header directory: "
      "${QSCINTILLA2_SCINTILLA_INCLUDE_DIR}")
  else()
    message(SEND_ERROR "no scintilla headers found "
      "(check that your apply our patch before to install QScintilla).")
    set(QSCINTILLA2_FOUND FALSE)
  endif()
endif()
