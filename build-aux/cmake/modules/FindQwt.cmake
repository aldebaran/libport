##
## FindQwt.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Find qwt library.
#
# You can set QWT_ROOT_DIR to a custom path to search in.
#
# This module set two variables:
#
#   QWT_LIBRARY: the path to the libraries found.
#   QWT_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

# Search for the library urbi.
if(NOT QWT_LIBRARY)
  find_library(QWT_LIBRARY
    NAMES qwt
    PATHS ${QWT_ROOT_DIR} ${QWT_ROOT_DIR}/lib)
  if(QWT_LIBRARY)
    message(STATUS "Found qwt library: ${QWT_LIBRARY}")
  else(QWT_LIBRARY)
    message(FATAL_ERROR "no qwt library found "
      "(setting -DQWT_ROOT_DIR=/path/to/qwt may solve this problem).")
  endif(QWT_LIBRARY)
endif(NOT QWT_LIBRARY)

# Search for the include directory.
if(NOT QWT_INCLUDE_DIR)
  find_path(QWT_INCLUDE_DIR
    NAMES qwt.h
    PATHS ${QWT_ROOT_DIR} ${QWT_ROOT_DIR}/include)
  if(QWT_INCLUDE_DIR)
    message(STATUS "Found qwt include directory: "
      "${QWT_INCLUDE_DIR}")
  else(QWT_INCLUDE_DIR)
    message(FATAL_ERROR "no qwt headers found "
      "(setting -DQWT_ROOT_DIR=/path/to/qwt may solve this problem).")
  endif(QWT_INCLUDE_DIR)
endif(NOT QWT_INCLUDE_DIR)
