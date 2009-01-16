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

# Search for the library qt_ext.

if(NOT QT_EXT_LIBRARY)

  #Release
  if(NOT QT_EXT_LIBRARY_RELEASE)
    find_library(QT_EXT_LIBRARY_RELEASE qt_ext)
    if(QT_EXT_LIBRARY_RELEASE)
      message(STATUS "Found qt-ext release library: ${QT_EXT_LIBRARY_RELEASE}")
      set(QT_EXT_FOUND TRUE)
    else(QT_EXT_LIBRARY_RELEASE)
      message(SEND_ERROR "no qt-ext release library found "
        "(setting -DCMAKE_INCLUDE_PATH may solve this problem).")
    endif(QT_EXT_LIBRARY_RELEASE)
  endif(NOT QT_EXT_LIBRARY_RELEASE)

  #Debug
  if(NOT QT_EXT_LIBRARY_DEBUG)
    find_library(QT_EXT_LIBRARY_DEBUG qt_extd)
    if(QT_EXT_LIBRARY_DEBUG)
      message(STATUS "Found qt-extd debug library: ${QT_EXT_LIBRARY_DEBUG}")
      set(QT_EXT_FOUND TRUE)
    else(QT_EXT_LIBRARY_DEBUG)
      message(SEND_ERROR "no qt-ext debug library found "
        "(setting -DCMAKE_INCLUDE_PATH may solve this problem).")
    endif(QT_EXT_LIBRARY_DEBUG)
  endif(NOT QT_EXT_LIBRARY_DEBUG)

  #Set QT_EXT_LIBRARY to the right value if needed
  if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    if(QT_EXT_LIBRARY_RELEASE OR QT_EXT_LIBRARY_DEBUG)
      set(QT_EXT_LIBRARY
	        optimized ${QT_EXT_LIBRARY_RELEASE}
	        debug     ${QT_EXT_LIBRARY_DEBUG}
      )
    endif(QT_EXT_LIBRARY_RELEASE OR QT_EXT_LIBRARY_DEBUG)
  else(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    set(QT_EXT_LIBRARY ${QT_EXT_LIBRARY_RELEASE})
  endif(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)

endif(NOT QT_EXT_LIBRARY)

if(QT_EXT_FOUND)
  # Search for the include directory.
  if(NOT QT_EXT_INCLUDE_DIR)
    find_path(QT_EXT_INCLUDE_DIR qt-ext/_qt-ext.hh)
    if(QT_EXT_INCLUDE_DIR)
      message(STATUS "Found qt-ext include directory: "
	"${QT_EXT_INCLUDE_DIR}")
    else(QT_EXT_INCLUDE_DIR)
      message(SEND_ERROR "no qt-ext headers found "
	"(setting -DCMAKE_INCLUDE_PATH may solve this problem).")
      set(QT_EXT_FOUND FALSE)
    endif(QT_EXT_INCLUDE_DIR)
  endif(NOT QT_EXT_INCLUDE_DIR)
endif(QT_EXT_FOUND)

if(QT_EXT_FOUND)
  # Use this variables iff you use all those libraries otherwise you will add
  # unused dependencies. Unfortunately cmake is unable to track dependent
  # library that are not built by itself (e.g. sdk-remote and qscintilla2).
  set(QT_EXT_LIBRARIES
    ${QT_EXT_LIBRARY}
    ${URBI_SDK_REMOTE_LIBRARY}
    ${QSCINTILLA2_LIBRARY}
    )

  set(QT_EXT_INCLUDES
    ${QT_EXT_INCLUDE_DIR}
    ${URBI_SDK_REMOTE_INCLUDE_DIR}
    ${QSCINTILLA2_INCLUDE_DIR}
    )
endif(QT_EXT_FOUND)

