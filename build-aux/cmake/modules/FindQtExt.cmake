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
# You can set QT_EXT_PATH to a custom path to search in.
#
# This module set two variables:
#
#   QT_EXT_LIBRARY: the path to the libraries found.
#   QT_EXT_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

# Search for the library urbi.
if(NOT QT_EXT_LIBRARY)
  find_library(QT_EXT_LIBRARY
    NAMES qt_ext
    PATHS ${QT_EXT_PATH} ${QT_EXT_PATH}/lib)
  if(QT_EXT_LIBRARY)
    message(STATUS "Found qt-ext library: ${QT_EXT_LIBRARY}")
  else(QT_EXT_LIBRARY)
    message(FATAL_ERROR "no qt-ext library found "
      "(setting -DQT_EXT_PATH=/path/to/qt-ext may solve this problem).")
  endif(QT_EXT_LIBRARY)
endif(NOT QT_EXT_LIBRARY)

# Search for the include directory.
if(NOT QT_EXT_INCLUDE_DIR)
  find_path(QT_EXT_INCLUDE_DIR
    NAMES qt-ext/qt-info.h
    PATHS ${QT_EXT_PATH} ${QT_EXT_PATH}/include)
  if(QT_EXT_INCLUDE_DIR)
    message(STATUS "Found qt-ext include directory: "
      "${QT_EXT_INCLUDE_DIR}")
  else(QT_EXT_INCLUDE_DIR)
    message(FATAL_ERROR "no qt-ext headers found "
      "(setting -DQT_EXT_PATH=/path/to/qt-ext may solve this problem).")
  endif(QT_EXT_INCLUDE_DIR)
endif(NOT QT_EXT_INCLUDE_DIR)
