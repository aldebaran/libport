##
## FindUrbiSdkRemote.cmake: This file is part of cmake-aux.
## Copyright (C) 2006-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##
##-----------------------------------------------------------------
## CMake PACKAGE - Urbi SDK Remote
## Find Urbi Sdk Remote libraries and paths.
##-----------------------------------------------------------------

set(PACKAGE_FULLNAME "Urbi SDK Remote")

##-----------------------------------------------------------------
# This package tries to respect CMake guidelines to be easily
# reusable.
# This module handles the following variables. Please note XXX is
# the name specified in the PACKAGE_NAME variable (just above).
#
# XXX_INCLUDE_DIRS     The final set of include directories listed in
#                      one variable for use by client code.
#
# XXX_LIBRARY_DIRS     Optionally, the final set of library
#                      directories listed in one variable for use
#                      by client code.
#
# XXX_LIBRARIES        The libraries to link against to use XXX. These
#                      should include full paths.
#
# XXX_SHARED_LIBRARIES The list of all shared libraries found in the
#                      package. This can be usefull if you're looking
#                      for .dll or .so to export during the install.
#
# XXX_YY_FOUND         FALSE/Unidefined if YY part of XXX package has
#                      not been found.
#
# XXX_FOUND            FALSE/Undefined if the whole package has not
#                      been found.
#                      Set it manually to FALSE don't want to use XXX.
#
# XXX_ROOT_DIR         Paths you want to add in the path list in order
#                      to help cmake find your package on your computer.
#
# QUIET option         You can ask the package not to complain if there
#                      is library/path not found. This means no output.
#                      If you specify both QUIET and REQUIRED option,
#                      the QUIET option has a lower priority and
#                      critical messages are displayed.
#
# REQUIRED option      You can ask the package to throw a FATAL_ERROR
#                      if the whole package has not been found.
#

# Here is the XXX_YY list:
# URBI_SDK_REMOTE_URBI_LIBRARY
# URBI_SDK_REMOTE_JPEG_LIBRARY
# URBI_SDK_REMOTE_INCLUDE
# URBI_SDK_REMOTE_UMAIN_DIR
#
##-----------------------------------------------------------------

# Retrieve PACKAGE_NAME and PACKAGE_FILENAME
include(Package-toolbox)
package_header(${CMAKE_CURRENT_LIST_FILE})

# Search for the include directory.
package_search(PATH ${PACKAGE_NAME}_INCLUDE uobject.h
               FULLNAME "SDK Remote INCLUDE"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Debug/include
                     ${${PACKAGE_NAME}_ROOT_DIR}/Release/include
                     ${${PACKAGE_NAME}_ROOT_DIR}/include)

# Search for URBI debug library
package_search(DEBUG LIBRARY ${PACKAGE_NAME}_URBI_DEBUG_LIBRARY urbi liburbi
               FULLNAME "SDK Remote for debug"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/core/i686-pc-cygwin/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/core/i686-pc-linux-gnu/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/core/mingw32/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Debug/lib
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/i686-pc-cygwin/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/mingw32/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/lib)

# Search for the associated jpeg debug library.
package_search(DEBUG LIBRARY ${PACKAGE_NAME}_JPEG_DEBUG_LIBRARY jpeg libjpeg
               FULLNAME "${PACKAGE_FULLNAME}'s JPEG"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Debug/lib
                     ${${PACKAGE_NAME}_ROOT_DIR}/lib)

# Search for URBI release library
package_search(RELEASE LIBRARY ${PACKAGE_NAME}_URBI_RELEASE_LIBRARY urbi liburbi
               FULLNAME "${PACKAGE_FULLNAME} for release"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/core/i686-pc-cygwin/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/core/i686-pc-linux-gnu/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/core/mingw32/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/Release/lib
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/i686-pc-cygwin/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/mingw32/remote
                     ${${PACKAGE_NAME}_ROOT_DIR}/lib)

# Search for the associated jpeg release library.
package_search(RELEASE LIBRARY ${PACKAGE_NAME}_JPEG_RELEASE_LIBRARY jpeg libjpeg
               FULLNAME "${PACKAGE_FULLNAME}'s JPEG"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Release/lib
                     ${${PACKAGE_NAME}_ROOT_DIR}/lib)

# Add platform specific libraries.
if(WIN32)
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ws2_32 gdi32)
elseif(UNIX)
    list(APPEND ${PACKAGE_NAME}_LIBRARIES pthread)
else(UNIX)
    if(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
        message( STATUS "Warning! No dependencies with SDK Remote has been "
                    "pre-configured for your platform into CMake. There "
                    "may be problem during the link.." )
    endif(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
endif(WIN32)

package_foot(${${PACKAGE_NAME}_PARTS_LIST})
