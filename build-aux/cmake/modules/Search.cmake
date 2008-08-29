## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
## --------------------------------------------------------------
## CMake MODULE - Search library/path

if(NOT MODULE_SEARCH_LOADED)
set(MODULE_SEARCH_LOADED TRUE)

#------------------------------------------------------------------------------
#                                 Win32 Addon
#------------------------------------------------------------------------------
# Because some libraries comes from UNIX programmers, there are sometimes wrong
# prefix/suffix on library name. Allow also the search of shared lib (.dll)
if(WIN32)

    # Avoid the automatic add of a big set of not needed libraries by CMake
    # under visual projects.
    set(CMAKE_CXX_STANDARD_LIBRARIES)
    set(CMAKE_C_STANDARD_LIBRARIES)

endif(WIN32)


#------------------------------------------------------------------------------
#                                 Search macro
#------------------------------------------------------------------------------
# Call find_library/find_path command. If the library/path still cannot be
# found, it prints a message you can customize by adding FULLNAME and INSTALL
# info. You can ask for one library or one path. Not both.
# You can influence the search by telling the macro if you prefer STATIC or
# SHARED library.
# The QUIET option allows you to disable any output message from this macro.
# The macro try to respect CMake guidelines.
#      Add the PACKAGE_MODE option if you are using search macro from a package
# module. Note that an alias for this mode is defined in <Package-toolbox.cmake>
# It will use your package information and update ${PACKAGE_NAME}_INCLUDE_DIRS,
# ${PACKAGE_NAME}_LIBRARIES and ${PACKAGE_NAME}_LIBRARY_DIRS variables.
# If you add RELEASE option, the library will be tagged and only added
# if the user choose to build his project in Release mode.
# Same logic for DEBUG option. If you want the library to be added
# in all build configuration, do not use those two options.
#
# syntax : search(LIBRARY|PATH RESULT_VAR file
#                 [FULLNAME "Full name of your file"]
#                 [INSTALL "What to install to get it"]
#                 [PATHS /some/additional/paths/to/find/the/file]
#                 [STATIC|SHARED]
#                 [QUIET]
#                 [ [PACKAGE_MODE] [RELEASE|DEBUG] ])
#
# e.g. : search(LIBRARY URBI_LIBRARY urbi
#               FULLNAME "Urbi remote"
#               INSTALL "SDK Remote"
#               PATHS /urbi/root/directory
#               STATIC
#               QUIET)

# For code convenience. Do not use this macro.
MACRO(_search_update_package_vars)

    if(ELEMENT_PACKAGE_MODE)
        if(ELEMENT_LIBRARY)
            # Look if the library has to be linked in a
            # special build configuration only.
            if(ELEMENT_RELEASE)
                list(APPEND ${PACKAGE_NAME}_LIBRARIES "optimized")
            endif(ELEMENT_RELEASE)
            if(ELEMENT_DEBUG)
                list(APPEND ${PACKAGE_NAME}_LIBRARIES "debug")
            endif(ELEMENT_DEBUG)

            # Add the result
            list(APPEND ${PACKAGE_NAME}_LIBRARIES ${${ELEMENT_RESULT}})
            get_filename_component(ELEMENT_TEMP0 ${${ELEMENT_RESULT}} PATH)
            list(APPEND ${PACKAGE_NAME}_LIBRARY_DIRS ${ELEMENT_TEMP0})
        else(ELEMENT_LIBRARY)
            list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${${ELEMENT_RESULT}})
        endif(ELEMENT_LIBRARY)
        list(APPEND ${PACKAGE_NAME}_PARTS_LIST ${ELEMENT_RESULT})
    endif(ELEMENT_PACKAGE_MODE)

ENDMACRO(_search_update_package_vars)

MACRO(search)
    # Add options given by packages
    list(APPEND ARGN ${SEARCH_OPTIONS})

    # Check parameters and look for the element(library/path)
    parse_arguments(ELEMENT
                    "LIBRARY;PATH;FULLNAME;INSTALL;PATHS"
                    "STATIC;SHARED;QUIET;PACKAGE_MODE;RELEASE;DEBUG"
                    ${ARGN})
    if(ELEMENT_LIBRARY AND ELEMENT_PATH)
        message(FATAL_ERROR "Invalid call of search macro. You cannot ask for"
                            " both library and path finding.")
    endif(ELEMENT_LIBRARY AND ELEMENT_PATH)

    # Get the result variable
    list(GET ELEMENT_LIBRARY 0 ELEMENT_TEMP0)
    list(GET ELEMENT_PATH 0 ELEMENT_TEMP1)
    if(NOT ELEMENT_TEMP0)
      set(ELEMENT_RESULT ${ELEMENT_TEMP1})
    else(NOT ELEMENT_TEMP0)
      set(ELEMENT_RESULT ${ELEMENT_TEMP0})
    endif(NOT ELEMENT_TEMP0)

    # Check if the path has not been already found
    # or manually given by the user
    if(NOT EXISTS ${${ELEMENT_RESULT}})

        # Add ROOT_DIR for package
        if(ELEMENT_PACKAGE_MODE)
            list(APPEND ELEMENT_PATHS ${${PACKAGE_NAME}_ROOT_DIR})
        endif(ELEMENT_PACKAGE_MODE)

        # Switch library/path mode
        if(ELEMENT_LIBRARY)
            list(GET ELEMENT_LIBRARY 1 ELEMENT_NAME)
            set(ELEMENT_TYPE library)

            # Look if the user specified a library preference
            if(ELEMENT_STATIC)
                set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_STATIC_LIBRARY_PREFIX})
                set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
                list(APPEND CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_SHARED_LIBRARY_PREFIX})
                list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
            endif(ELEMENT_STATIC)

            if(ELEMENT_SHARED)
                set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_SHARED_LIBRARY_PREFIX})
                set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
                list(APPEND CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_STATIC_LIBRARY_PREFIX})
                list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
            endif(ELEMENT_SHARED)

            find_library(${ELEMENT_LIBRARY} PATHS ${ELEMENT_PATHS})

        elseif(ELEMENT_PATH)
            list(GET ELEMENT_PATH 1 ELEMENT_NAME)
            set(ELEMENT_TYPE directory)
            find_path(${ELEMENT_PATH} PATHS ${ELEMENT_PATHS})

        else(ELEMENT_PATH)
            message(FATAL_ERROR "Invalid syntax. Specify LIBRARY or PATH section.")
        endif(ELEMENT_LIBRARY)

        # If we find the element
        if(EXISTS ${${ELEMENT_RESULT}})
            if(NOT ELEMENT_QUIET)
                message(STATUS "Found '" ${ELEMENT_NAME} "' " ${ELEMENT_TYPE} " : "
                        ${${ELEMENT_RESULT}})
            endif(NOT ELEMENT_QUIET)
            set(${ELEMENT_RESULT}_FOUND TRUE)

            # Update guidelines variables for package
            _search_update_package_vars()
        else(EXISTS $${{ELEMENT_RESULT}})
            # Replace undefined parameters by generic ones
            # or by package info if PACKAGE option is defined
            if(NOT ELEMENT_FULLNAME)
                set(ELEMENT_FULLNAME ${ELEMENT_NAME})
            else(NOT ELEMENT_FULLNAME)
                set(ELEMENT_FULLNAME ${ELEMENT_FULLNAME} "(" ${ELEMENT_NAME} ")")
            endif(NOT ELEMENT_FULLNAME)

            if(NOT ELEMENT_INSTALL)
                if(ELEMENT_PACKAGE_MODE)
                    set(ELEMENT_INSTALL ${PACKAGE_FULLNAME})
                else(ELEMENT_PACKAGE_MODE)
                    set(ELEMENT_INSTALL "it")
                endif(ELEMENT_PACKAGE_MODE)
            endif(NOT ELEMENT_INSTALL)

           # Custom message
           if(NOT ELEMENT_QUIET)
               message(STATUS ${ELEMENT_FULLNAME} " " ${ELEMENT_TYPE}
                  " has not been found. Make sure you've installed "
                  ${ELEMENT_INSTALL} " on your computer or please specify the "
                  "library path manually(-D" ${ELEMENT_RESULT} ").")
           endif(NOT ELEMENT_QUIET)

           set(${ELEMENT_RESULT} ${ELEMENT_RESULT}-NOTFOUND)
        endif(EXISTS ${${ELEMENT_RESULT}})

    else(NOT EXISTS ${${ELEMENT_RESULT}})
        # Just add previously found elements
        _search_update_package_vars()
    endif(NOT EXISTS ${${ELEMENT_RESULT}})

ENDMACRO(search)

endif(NOT MODULE_SEARCH_LOADED)