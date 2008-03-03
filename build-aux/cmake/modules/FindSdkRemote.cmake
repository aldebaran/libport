##
## FindSdkRemote.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Find sdk-remote library.
#
# You can set SDK_REMOTE_PATH to a custom path to search in.
#
# This module set two variables:
#
#   SDK_REMOTE_LIBRARY: the path to the libraries found.
#   SDK_REMOTE_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.

# Search for the library urbi.
if(NOT SDK_REMOTE_LIBRARY)
  find_library(SDK_REMOTE_LIBRARY
    NAMES urbi
    PATHS ${SDK_REMOTE_PATH} ${SDK_REMOTE_PATH}/lib)
  if(SDK_REMOTE_LIBRARY)
    message(STATUS "Found sdk-remote library: ${SDK_REMOTE_LIBRARY}")
  else(SDK_REMOTE_LIBRARY)
    message(FATAL_ERROR "no sdk-remote library found "
      "(setting -DSDK_REMOTE_PATH=/path/to/sdk-remote may solve this problem).")
  endif(SDK_REMOTE_LIBRARY)
endif(NOT SDK_REMOTE_LIBRARY)

# Search for the include directory.
if(NOT SDK_REMOTE_INCLUDE_DIR)
  find_path(SDK_REMOTE_INCLUDE_DIR
    NAMES uobject.h
    PATHS ${SDK_REMOTE_PATH} ${SDK_REMOTE_PATH}/include)
  if(SDK_REMOTE_INCLUDE_DIR)
    message(STATUS "Found sdk-remote include directory: "
      "${SDK_REMOTE_INCLUDE_DIR}")
  else(SDK_REMOTE_INCLUDE_DIR)
    message(FATAL_ERROR "no sdk-remote headers found "
      "(setting -DSDK_REMOTE_PATH=/path/to/sdk-remote may solve this problem).")
  endif(SDK_REMOTE_INCLUDE_DIR)
endif(NOT SDK_REMOTE_INCLUDE_DIR)
