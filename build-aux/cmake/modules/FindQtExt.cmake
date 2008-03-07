##
## FindQtExt.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Find qt-ext library.
#
# You can set QT_EXT_ROOT_DIR to a custom path to search in.
#
# This module set two variables:
#
#   QT_EXT_LIBRARY: the path to the libraries found.
#   QT_EXT_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

include(QtExt)

set(QT_EXT_FOUND FALSE)

# Search for the library urbi.
if(NOT QT_EXT_LIBRARY)
  find_library(QT_EXT_LIBRARY qt_ext)
  if(QT_EXT_LIBRARY)
    message(STATUS "Found qt-ext library: ${QT_EXT_LIBRARY}")
    set(QT_EXT_FOUND TRUE)
  else(QT_EXT_LIBRARY)
    message(FATAL_ERROR "no qt-ext library found "
      "(setting -DQT_EXT_ROOT_DIR=/path/to/qt-ext may solve this problem).")
  endif(QT_EXT_LIBRARY)
endif(NOT QT_EXT_LIBRARY)

if(QT_EXT_FOUND)
  # Search for the include directory.
  if(NOT QT_EXT_INCLUDE_DIR)
    find_path(QT_EXT_INCLUDE_DIR qt-ext/qt-info.h)
    if(QT_EXT_INCLUDE_DIR)
      message(STATUS "Found qt-ext include directory: "
	"${QT_EXT_INCLUDE_DIR}")
    else(QT_EXT_INCLUDE_DIR)
      message(FATAL_ERROR "no qt-ext headers found "
	"(setting -DQT_EXT_ROOT_DIR=/path/to/qt-ext may solve this problem).")
      set(QT_EXT_FOUND FALSE)
    endif(QT_EXT_INCLUDE_DIR)
  endif(NOT QT_EXT_INCLUDE_DIR)
endif(QT_EXT_FOUND)

if(QT_EXT_FOUND)
  # Use this variables iff you use all those libraries otherwise you will add
  # unused dependencies.
  set(QT_EXT_LIBRARIES
    ${QT_EXT_LIBRARY}
    ${SDK_REMOTE_LIBRARY}
    ${QSCINTILLA2_LIBRARY}
    ${QT_LIBRARIES}
    )

  set(QT_EXT_INCLUDES
    ${QT_EXT_INCLUDE_DIR}
    ${SDK_REMOTE_INCLUDE_DIR}
    ${QSCINTILLA2_INCLUDE_DIR}
    ${QT_INCLUDES}
    )
endif(QT_EXT_FOUND)

