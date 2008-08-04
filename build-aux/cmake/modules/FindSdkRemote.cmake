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
##-----------------------------------------------------------------
## CMake PACKAGE - URBI SDK Remote
## Find URBI Sdk Remote libraries and paths.
##-----------------------------------------------------------------

set(PACKAGE_FULLNAME "URBI SDK Remote")

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
# Here is the XXX_YY list:
# SDK_REMOTE_URBI_LIBRARY
# SDK_REMOTE_JPEG_LIBRARY
# SDK_REMOTE_INCLUDE
# SDK_REMOTE_UMAIN_DIR
#
##-----------------------------------------------------------------

# Retrieve PACKAGE_NAME and PACKAGE_FILENAME
include(Package-toolbox)
set_package_name(${CMAKE_CURRENT_LIST_FILE})

if(NOT ${PACKAGE_NAME}_FOUND)

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

    # Search for the include directory.
    search(PATH SDK_REMOTE_INCLUDE uobject.h
           FULLNAME "SDK Remote INCLUDE"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${${PACKAGE_NAME}_ROOT_DIR}
	         ${${PACKAGE_NAME}_ROOT_DIR}/include
           ${SEARCH_OPTIONS})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${SDK_REMOTE_INCLUDE})

    # Search for URBI library
    search(LIBRARY SDK_REMOTE_URBI_LIBRARY urbi
           FULLNAME "SDK Remote"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${${PACKAGE_NAME}_ROOT_DIR}
	         ${${PACKAGE_NAME}_ROOT_DIR}/lib
		 ${SDK_REMOTE_INCLUDE}/../lib
           ${SEARCH_OPTIONS})
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${SDK_REMOTE_URBI_LIBRARY})

    # Search for the optionnal main.cc added to link libraries
    search(PATH SDK_REMOTE_UMAIN_DIR umain.cc
           FULLNAME "SDK Remote UMAIN"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${SDK_REMOTE_ROOT_DIR}/share/umain
                 ${SDK_REMOTE_INCLUDE}/../share/umain
           ${SEARCH_OPTIONS})
    set(SDK_REMOTE_UMAIN_FILE ${SDK_REMOTE_UMAIN_DIR})
    list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${SDK_REMOTE_UMAIN_DIR})

    # Search for the associated jpeg library.
    search(LIBRARY SDK_REMOTE_JPEG_LIBRARY jpeg
           FULLNAME "SDK Remote's JPEG"
           PACKAGE ${PACKAGE_FULLNAME}
           PATHS ${${PACKAGE_NAME}_ROOT_DIR}
	         ${SDK_REMOTE_URBI_LIBRARY}
           ${SEARCH_OPTIONS})
    list(APPEND ${PACKAGE_NAME}_LIBRARIES ${SDK_REMOTE_JPEG_LIBRARY})

    if(SDK_REMOTE_URBI_LIBRARY AND SDK_REMOTE_INCLUDE AND SDK_REMOTE_JPEG_LIBRARY)
        set(${PACKAGE_NAME}_FOUND TRUE)
    else(SDK_REMOTE_URBI_LIBRARY AND SDK_REMOTE_INCLUDE AND SDK_REMOTE_JPEG_LIBRARY)
        if(${PACKAGE_FILENAME}_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find the whole package. Use -D"
                                ${PACKAGE_NAME}_ROOT_DIR "to specify paths to help"
                                "CMake find this package.")
        endif(${PACKAGE_FILENAME}_FIND_REQUIRED)
    endif(SDK_REMOTE_URBI_LIBRARY AND SDK_REMOTE_INCLUDE AND SDK_REMOTE_JPEG_LIBRARY)

    # Add platform specific libraries.
    if(WIN32)
        list(APPEND ${PACKAGE_NAME}_LIBRARIES ws2_32)
    elseif(UNIX)
        list(APPEND ${PACKAGE_NAME}_LIBRARIES pthread)
    else(UNIX)
        if(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
            message( STATUS "Warning! No dependencies with SDK Remote has been "
                        "pre-configured for your platform into CMake. There "
                        "may be problem during the link.." )
        endif(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
    endif(WIN32)

endif(NOT ${PACKAGE_NAME}_FOUND)
