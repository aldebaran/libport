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
# You can set SDK_REMOTE_ROOT_DIR to a custom path to search in.
#
# This module set two variables:
#
#   SDK_REMOTE_LIBRARY: the path to the libraries found.
#   SDK_REMOTE_LIBRARIES: the set of libraries of the remote sdk. Add this
#                         variables to target_link_libraries.
#   SDK_REMOTE_INCLUDE_DIR: the include path.
#
# These variables are set to <var-name>-NOTFOUND if an error occurs.


set(SDK_REMOTE_FOUND FALSE)

# Search for the library urbi.
if(NOT SDK_REMOTE_LIBRARY)
  # Search for sdk-remote library.
  find_library(SDK_REMOTE_LIBRARY urbi)
  if(SDK_REMOTE_LIBRARY)
    message(STATUS "Found sdk-remote library: ${SDK_REMOTE_LIBRARY}")
    set(SDK_REMOTE_FOUND TRUE)
  else(SDK_REMOTE_LIBRARY)
    message(SEND_ERROR "no sdk-remote library found.")
  endif(SDK_REMOTE_LIBRARY)
  # Search for the associated jpeg library.
  find_library(SDK_REMOTE_JPEG_LIBRARY jpeg)
  if(SDK_REMOTE_JPEG_LIBRARY)
    message(STATUS
      "Found sdk-remote's jpeg library: ${SDK_REMOTE_JPEG_LIBRARY}")
    set(SDK_REMOTE_JPEG_FOUND TRUE)
  else(SDK_REMOTE_LIBRARY)
    message(SEND_ERROR "no sdk-remote's jpeg library found.")
  endif(SDK_REMOTE_JPEG_LIBRARY)
endif(NOT SDK_REMOTE_LIBRARY)

if(SDK_REMOTE_FOUND)
  # Search for the include directory.
  if(NOT SDK_REMOTE_INCLUDE_DIR)
    find_path(SDK_REMOTE_INCLUDE_DIR uobject.h)
    if(SDK_REMOTE_INCLUDE_DIR)
      message(STATUS "Found sdk-remote include directory: "
	"${SDK_REMOTE_INCLUDE_DIR}")
    else(SDK_REMOTE_INCLUDE_DIR)
      message(SEND_ERROR "no sdk-remote headers found "
	"(setting -DSDK_REMOTE_ROOT_DIR=/path/to/sdk-remote may solve this "
	"problem).")
      set(SDK_REMOTE_FOUND FALSE)
    endif(SDK_REMOTE_INCLUDE_DIR)
  endif(NOT SDK_REMOTE_INCLUDE_DIR)

  # Add dependent libraries.
  set(SDK_REMOTE_DEPLIBS ${SDK_REMOTE_LIBRARY})
  if(WIN32)
    list(APPEND SDK_REMOTE_DEPLIBS ${SDK_REMOTE_JPEG_LIBRARY} ws2_32)
  else()
    list(APPEND SDK_REMOTE_DEPLIBS ${SDK_REMOTE_JPEG_LIBRARY})
  endif()
  set(SDK_REMOTE_LIBRARIES ${SDK_REMOTE_DEPLIBS}
    CACHE INTERNAL "SdkRemote's dependent libraries")
endif(SDK_REMOTE_FOUND)

