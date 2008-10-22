##
## FindUmain.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##
##-----------------------------------------------------------------
## CMake PACKAGE - UMain.cc
## This find the umain.cc file and his dependencies. This file is
## added if no default main is found in your project, which the
## most common case.
##-----------------------------------------------------------------

# Dependencies
find_package(Boost 1.34.1 REQUIRED)
# Check if boost was found, because, FindBoost don't display
# a FATAL_ERROR message.
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found")
endif()

set(PACKAGE_FULLNAME "URBI SDK Umain.cc directory")

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
# UMAIN_CC_DIR
#
##-----------------------------------------------------------------

# Retrieve PACKAGE_NAME and PACKAGE_FILENAME
include(Package-toolbox)
package_header(${CMAKE_CURRENT_LIST_FILE})

# Add dependencies to the UMain package
list(APPEND ${PACKAGE_NAME}_LIBRARIES ${Boost_LIBRARIES})
list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND ${PACKAGE_NAME}_LIBRARY_DIRS ${Boost_LIBRARY_DIRS})

if(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
    message(STATUS "Found boost lib: " ${Boost_LIBRARIES})
    message(STATUS "Found boost include: " ${Boost_INCLUDE_DIRS})
endif(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)

# Search for umain.cc file
package_search(PATH ${PACKAGE_NAME}_CC_DIR umain.cc
               FULLNAME "SDK Umain"
               PATHS ${SDK_ENGINE_ROOT_DIR}/Release/share/umain
                     ${SDK_ENGINE_ROOT_DIR}/Debug/share/umain
                     ${SDK_REMOTE_ROOT_DIR}/Release/share/umain
                     ${SDK_REMOTE_ROOT_DIR}/Debug/share/umain
                     ${SDK_ENGINE_ROOT_DIR}/share/umain
                     ${SDK_REMOTE_ROOT_DIR}/share/umain
                     ${SDK_ENGINE_INCLUDE}/../share/umain
                     ${SDK_REMOTE_INCLUDE}/../share/umain)

package_foot(${${PACKAGE_NAME}_PARTS_LIST})