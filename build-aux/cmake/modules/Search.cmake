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
## Call find_library/find_path command. If the library/path still cannot be
## found, it prints a message you can customize by adding FULLNAME and PACKAGE
## info. You can ask for one library or one path. Not both.
## You can influence the search by telling the macro if you prefer STATIC or
## SHARED library.
## The QUIET option allows you to disable any output message from this macro.
## The macro try to respect CMake guidelines.
## e.g. search(LIBRARY URBI_LIBRARY urbi
##             FULLNAME "Urbi remote"
##             PACKAGE "SDK Remote"
##             PATHS /urbi/root/directory
##             [STATIC|SHARED]
##             [QUIET])
##------------------------------------------------------------------------------

MACRO(search)

    # Check parameters and look for the element(library/path)
    parse_arguments(ELEMENT
                    "LIBRARY;PATH;FULLNAME;PACKAGE;PATHS"
                    "STATIC;SHARED;QUIET"
                    ${ARGN})

    if(ELEMENT_LIBRARY AND ELEMENT_PATH)
        message(FATAL_ERROR "Invalid call of search macro. You cannot ask for"
                            " both library and path finding.")
    endif(ELEMENT_LIBRARY AND ELEMENT_PATH)

    # ARGV1 is the variable where the find result is stored (second argument)
    if(NOT ${ARGV1})

        if(ELEMENT_LIBRARY)
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
            set(ELEMENT_TYPE directory)
            find_path(${ELEMENT_PATH} PATHS ${ELEMENT_PATHS})

        else(ELEMENT_PATH)
            message(FATAL_ERROR "Invalid syntax. Specify LIBRARY or PATH section.")
        endif(ELEMENT_LIBRARY)

        # If we don't find the element
        if(${ARGV1})
            if(NOT ELEMENT_QUIET)
                message(STATUS "Found '" ${ARGV2} "' " ${ELEMENT_TYPE} " : "
                        ${${ARGV1}})
            endif(NOT ELEMENT_QUIET)
            set(${ARGV1}_FOUND TRUE)
        else(${ARGV1})
            # Replace undefined parameters by generic ones
            if(NOT ELEMENT_FULLNAME)
                set(ELEMENT_FULLNAME ${ARGV2})
            else(NOT ELEMENT_FULLNAME)
                set(ELEMENT_FULLNAME ${ELEMENT_FULLNAME} "(" ${ARGV2} ")")
            endif(NOT ELEMENT_FULLNAME)

            if(NOT ELEMENT_PACKAGE)
                set(ELEMENT_PACKAGE "it")
            endif(NOT ELEMENT_PACKAGE)

           # Custom message
           if(NOT ELEMENT_QUIET)
               message(STATUS ${ELEMENT_FULLNAME} " " ${ELEMENT_TYPE}
                  " has not been found. Make sure you've installed "
                  ${ELEMENT_PACKAGE} " on your computer or please specify the "
                  "library path manually(-D" ${ARGV1} ").")
           endif(NOT ELEMENT_QUIET)

           set(${ARGV1}_FOUND TRUE)
           set(${${ARGV1}} ${ARGV1}-NOTFOUND)
        endif(${ARGV1})

    endif(NOT ${ARGV1})

ENDMACRO(search)
