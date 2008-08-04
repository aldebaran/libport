##
## FindSdkEngine.cmake: This file is part of cmake-aux.
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
## CMake PACKAGE - URBI SDK Engine
## Find URBI Sdk Engine libraries and paths.
#
# Current FindOpenSSL module provided by CMake is laking:
#    * support for choosing between static/shared library to search for
#    * search of the libcrypto
# that's why we provide this alternative package waiting for better solution
# coming from upstream.
#
##-----------------------------------------------------------------

# Dependencies
find_package(OpenSSL)
find_package(BoostV2 1.34.1)

set(PACKAGE_FULLNAME "URBI SDK Engine")

##-----------------------------------------------------------------
# This package tries to respect CMake guidelines to be easily
# reusable.
# This module handles the following variables. Please note XXX is
# the name specified in the PACKAGE_NAME variable (just above).
#
# XXX_INCLUDE_DIRS   The final set of include directories listed in
#                    one variable for use by client code.
#
# XXX_LIBRARY_DIRS   Optionally, the final set of library
#                    directories listed in one variable for use
#                    by client code.
#
# XXX_LIBRARIES      The libraries to link against to use XXX. These
#                    should include full paths.
#
# XXX_YY_FOUND       FALSE/Unidefined if YY part of XXX package has
#                    not been found.
#
# XXX_FOUND          FALSE/Undefined if the whole package has not
#                    been found.
#                    Set it manually to FALSE don't want to use XXX.
#
# XXX_ROOT_DIR       Paths you want to add in the path list in order
#                    to help cmake find your package on your computer.
#
# QUIET option       You can ask the package not to complain if there
#                    is library/path not found. This means no output.
#                    If you specify both QUIET and REQUIRED option,
#                    the QUIET option has a lower priority and
#                    critical messages are displayed.
#
# REQUIRED option    You can ask the package to throw a FATAL_ERROR
#                    if the whole package has not been found.
#
# Here is the current XXX_YY list :
# SDK_ENGINE_URBI_LIBRARY
# SDK_ENGINE_INCLUDE
# SDK_ENGINE_UMAIN_DIR
# SDK_ENGINE_THREAD_BOOST_LIBRARY
#
##-----------------------------------------------------------------

# Retrieve PACKAGE_NAME and PACKAGE_FILENAME
include(Package-toolbox)
set_package_name(${CMAKE_CURRENT_LIST_FILE})

if(NOT ${PACKAGE_NAME}_FOUND)

    # Add dependencies to the SDK engine
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${OPEN_SSL_LIBRARIES})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${OPEN_SSL_INCLUDE_DIRS})
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${Boost_LIBRARIES})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    if(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
        message(STATUS "Found boost lib: " ${Boost_LIBRARIES})
        message(STATUS "Found boost include: " ${Boost_INCLUDE_DIRS})
    endif(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)

    # Load search macro and set additional options
    include(Search)
    set(SEARCH_OPTIONS)

    if(${PACKAGE_FILENAME}_FIND_QUIETLY)
        list(APPEND SEARCH_OPTIONS QUIET)
    else(${PACKAGE_FILENAME}_FIND_QUIETLY)
        message(STATUS "Loading package " ${PACKAGE_FULLNAME} " ("
                       ${PACKAGE_NAME} ")")
    endif(${PACKAGE_FILENAME}_FIND_QUIETLY)

    set(${PACKAGE_NAME}_FOUND FALSE)
    set(${PACKAGE_NAME}_LIBRARY_DIRS ${CMAKE_LIBRARY_PATH})

    # Some libraries names are not WIN32 standards
    if(WIN32)
        list(APPEND CMAKE_STATIC_PREFIX "" "lib")
    endif(WIN32)

    # Search for URBI library
    search(LIBRARY SDK_ENGINE_URBI_LIBRARY urbicore
           FULLNAME "SDK Engine"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${${PACKAGE_NAME}_ROOT_DIR}
                 ${${PACKAGE_NAME}_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/engine
           ${SEARCH_OPTIONS})
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${SDK_ENGINE_URBI_LIBRARY})

    # Search for the include directory.
    search(PATH SDK_ENGINE_INCLUDE uobject.h
           FULLNAME "SDK Engine INCLUDE"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${${PACKAGE_NAME}_ROOT_DIR}
                 ${${PACKAGE_NAME}_ROOT_DIR}/include
           ${SEARCH_OPTIONS})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${SDK_ENGINE_INCLUDE})

    # Search for the optionnal main.cc added to link libraries
    search(PATH SDK_ENGINE_UMAIN_DIR umain.cc
           FULLNAME "SDK Engine UMAIN"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${SDK_ENGINE_ROOT_DIR}/share/umain
                 ${SDK_ENGINE_INCLUDE}/../share/umain
           ${SEARCH_OPTIONS})
    set(SDK_ENGINE_UMAIN_FILE ${SDK_ENGINE_UMAIN_DIR})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${SDK_ENGINE_UMAIN_DIR})

    # Search for the thread part of the boost library
    search(LIBRARY SDK_ENGINE_THREAD_BOOST_LIBRARY boost_thread-gcc41-mt-1_34_1
           PACKAGE "Boost version 1.34.1 (thread part)"
           PATHS ${${PACKAGE_NAME}_ROOT_DIR})
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${SDK_ENGINE_THREAD_BOOST_LIBRARY})


    if(SDK_ENGINE_URBI_LIBRARY AND SDK_ENGINE_INCLUDE)
        set(${PACKAGE_NAME}_FOUND TRUE)
    else(SDK_ENGINE_URBI_LIBRARY AND SDK_ENGINE_INCLUDE)
        if(${PACKAGE_FILENAME}_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find the whole package. Use -D"
                                ${PACKAGE_NAME}_ROOT_DIR "to specify paths to help"
                                "CMake find this package.")
        endif(${PACKAGE_FILENAME}_FIND_REQUIRED)
    endif(SDK_ENGINE_URBI_LIBRARY AND SDK_ENGINE_INCLUDE)

endif(NOT ${PACKAGE_NAME}_FOUND)

