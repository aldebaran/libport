##
## FindUrbiKernel.cmake: This file is part of cmake-aux.
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
## CMake PACKAGE - URBI Kernel
## Find URBI Kernel libraries and paths.
##-----------------------------------------------------------------

# Dependencies
#find_package(UrbiOpenSSL REQUIRED)

set(PACKAGE_NAME "URBI_KERNEL")
set(PACKAGE_FULLNAME "URBI Kernel")

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

# Here is the current XXX_YY list :
# URBI_KERNEL_DEBUG_LIBRARY
# URBI_KERNEL_RELEASE_LIBRARY
# URBI_KERNEL_INCLUDE
#
##-----------------------------------------------------------------

include(Package-toolbox)
package_header(${CMAKE_CURRENT_LIST_FILE})

# Places to look for the library/dll
list(APPEND ${PACKAGE_NAME}_ADDITIONAL_DEBUG_PATHS
            ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/kernel/i686-pc-cygwin/engine/.libs
            ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/kernel/i686-pc-linux-gnu/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/kernel/mingw32/engine/.libs
            ${${PACKAGE_NAME}_ROOT_DIR}/Debug/lib
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/i686-pc-cygwin/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/i686-pc-linux-gnu/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/mingw32/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/lib)

list(APPEND ${PACKAGE_NAME}_ADDITIONAL_RELEASE_PATHS
            ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/kernel/i686-pc-cygwin/engine/.libs
            ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/kernel/i686-pc-linux-gnu/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/kernel/mingw32/engine/.libs
            ${${PACKAGE_NAME}_ROOT_DIR}/Release/lib
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/i686-pc-cygwin/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/i686-pc-linux-gnu/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/gostai/kernel/mingw32/engine
            ${${PACKAGE_NAME}_ROOT_DIR}/lib)

# Search for the include directory.
package_search(PATH ${PACKAGE_NAME}_INCLUDE kernel/userver.hh
               FULLNAME "Urbi Kernel INCLUDE"
               PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Release/gostai/kernel/include
                     ${${PACKAGE_NAME}_ROOT_DIR}/Debug/gostai/kernel/include
                     ${${PACKAGE_NAME}_ROOT_DIR}/include)

# Search for URBI debug library
package_search(DEBUG LIBRARY ${PACKAGE_NAME}_DEBUG_LIBRARY libkernel kernel
               FULLNAME "Urbi Kernel for debug"
               PATHS ${${PACKAGE_NAME}_ADDITIONAL_DEBUG_PATHS})

# Search for URBI release library
package_search(RELEASE LIBRARY ${PACKAGE_NAME}_RELEASE_LIBRARY libkernel kernel
               FULLNAME "Urbi Kernel for release"
               PATHS ${${PACKAGE_NAME}_ADDITIONAL_RELEASE_PATHS})

# Search for the thread part of the boost library
# Boost
find_package(Boost 1.34.1 COMPONENTS thread EXACT REQUIRED)

package_foot(${${PACKAGE_NAME}_PARTS_LIST})
