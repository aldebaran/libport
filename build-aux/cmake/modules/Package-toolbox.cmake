##
## Package-toolbox.cmake: This file is part of cmake-aux.
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
## CMake PACKAGE - Toolbox
## Usefull macro to use into package file
##-----------------------------------------------------------------

if(NOT MODULE_PACKAGE_TOOLBOX_LOADED)
SET(MODULE_PACKAGE_TOOLBOX_LOADED TRUE)

include(Search)
# Used by package_search macro.

include(Macro-toolbox)
# Used by package_foot (list_shared_libraries)

#-----------------------------------------------------------------
#                           Set PACKAGE_NAME
#-----------------------------------------------------------------
# You must give the macro the CMAKE_CURRENT_LIST_FILE.
# Then it deduces from the filename the most appropriate package name.
# Set it in PACKAGE_NAME variable. It also set you PACKAGE_FILENAME
# which is the XXX part of FindXXX.cmake with the file case if you
# want to retrieve parameters like XXX_FIND_QUIET and
# XXX_FIND_REQUIRED.
#
# e.g. "foo/bar/FindYourCoolPACKAGEwithNEWfeature.cmake"
#      will generate : "YOUR_COOL_PACKAGE_WITH_NEW_FEATURE"
#
# Algorithm: 1. Extract the XXX in /current/dir/FindXXX.cmake string
#            2. Replace '.' and '-' with '_'
#            3. Put a '_' between lowcase/upcase
#            4. Put a '_' between upcase(2 or more)/lowcase
#            5. Upper case the output string
#
# Note : If you just want the PACKAGE_FILENAME variable, you
#        would prefer use the 'get_filename_component' command.

MACRO(set_package_name CURRENT_LIST_FILE)

    string(REGEX MATCH "Find(.+)\\.cmake$" DUMMY ${CURRENT_LIST_FILE})
    set(PACKAGE_FILENAME ${CMAKE_MATCH_1})
    string(REGEX REPLACE "[-.]" "_" CMAKE_MATCH_1 ${CMAKE_MATCH_1})
    string(REGEX REPLACE "([^A-Z_])([A-Z])" "\\1_\\2" CMAKE_MATCH_1 ${CMAKE_MATCH_1})
    string(REGEX REPLACE "([A-Z][A-Z]+)([^A-Z_])" "\\1_\\2" CMAKE_MATCH_1 ${CMAKE_MATCH_1})
    string(TOUPPER ${CMAKE_MATCH_1} PACKAGE_NAME)

ENDMACRO(set_package_name)

#-----------------------------------------------------------------
#                         Package is complete ?
#-----------------------------------------------------------------
# This macro checks if the package is complete.
# The result is store in ${PACKAGE_NAME}_FOUND like it's
# recommended in CMake guidelines.
# You must have called 'set_package_name' before.
# Just specify which variables must be set for each platform.
# If it's for both, then type it without delimiters.
# e.g. package_isComplete(var_both1 var_both2..
#                         UNIX var1 var2...
#                         WIN32 var1 var2...)

MACRO(package_isComplete)

    set(${PACKAGE_NAME}_FOUND TRUE)
    parse_arguments(PACKAGE "UNIX;WIN32" "" ${ARGN})

    if(WIN32)
      set(PACKAGE_REQUIRED ${PACKAGE_WIN32})
    endif(WIN32)

    if(UNIX)
      set(PACKAGE_REQUIRED ${PACKAGE_UNIX})
    endif(UNIX)

    list(APPEND PACKAGE_REQUIRED ${PACKAGE_DEFAULT_ARGS})

    foreach(package_var ${PACKAGE_REQUIRED})
        if(NOT EXISTS ${${package_var}})
          if(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
              message(STATUS "MISSING : " ${package_var})
          endif(NOT ${PACKAGE_FILENAME}_FIND_QUIETLY)
          set(${PACKAGE_NAME}_FOUND FALSE)
        endif(NOT EXISTS ${${package_var}})
    endforeach(package_var)

ENDMACRO(package_isComplete)


#-----------------------------------------------------------------
#                           Package search
#-----------------------------------------------------------------
# Quick alias to the search macro in package mode.

MACRO(package_search)

    search(${ARGN} PACKAGE_MODE)

ENDMACRO(package_search)

#-----------------------------------------------------------------
#                           Package header
#-----------------------------------------------------------------
# Header of package. Convenient to factorize find packages.
# Just specify the path to the package file.
# e.g. package_header(${CMAKE_CURRENT_LIST_FILE})

MACRO(package_header PACKAGE_PATH)

    # Retrieve PACKAGE_NAME and PACKAGE_FILENAME
    set_package_name(${PACKAGE_PATH})

    # Look if the package has already been found
    if(DEFINED ${PACKAGE_NAME}_FOUND)
        package_foot()
        return()
    endif(DEFINED ${PACKAGE_NAME}_FOUND)

    # Load search macro and set additional options
    include(Search)
    set(SEARCH_OPTIONS)

    if(${PACKAGE_FILENAME}_FIND_QUIETLY)
        list(APPEND SEARCH_OPTIONS QUIET)
    else(${PACKAGE_FILENAME}_FIND_QUIETLY)
        message(STATUS "Loading package " ${PACKAGE_FULLNAME} " ("
                       ${PACKAGE_NAME} ")")
    endif(${PACKAGE_FILENAME}_FIND_QUIETLY)

    set(${PACKAGE_NAME}_FOUND)
    set(${PACKAGE_NAME}_LIBRARY_DIRS)
    set(${PACKAGE_NAME}_SHARED_LIBRARIES)
    set(${PACKAGE_NAME}_LIBRARIES)

ENDMACRO(package_header)

#-----------------------------------------------------------------
#                           Package foot
#-----------------------------------------------------------------
# Foot of package. Convenient to factorize find packages.
# Check if the whole package has been retrieved
# You must give 'package_isComplete' arguments.
# You must have called 'set_package_name' or 'package_header'
# before.
# e.g. package_foot(UNIX var1 var2... WIN32 var1 var2...)

MACRO(package_foot)

    # Check if the whole package has been retrieved
    package_isComplete(${ARGN})

    # If the REQUIRED option is set, it throws a fatal error to the
    # user to prevent him from going any further in the build process.
    if(NOT ${PACKAGE_NAME}_FOUND AND ${PACKAGE_FILENAME}_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find the whole package. Use -D"
                            ${PACKAGE_NAME}_ROOT_DIR " to specify paths to help"
                            " CMake find this package.")
    endif(NOT ${PACKAGE_NAME}_FOUND AND ${PACKAGE_FILENAME}_FIND_REQUIRED)

    list_shared_libraries(${PACKAGE_NAME}_LIBRARIES
                          ${PACKAGE_NAME}_SHARED_LIBRARIES)

ENDMACRO(package_foot)

endif(NOT MODULE_PACKAGE_TOOLBOX_LOADED)