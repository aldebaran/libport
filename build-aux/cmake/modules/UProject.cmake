## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
## ---------------------------------------------------------------
## CMake - UObject project
## User-friendly macros for UObject project.
## ------------------------------- -------------------------------
##

include (Macro-toolbox)
# parse_arguments macro

#------------------------------------------------------------------------------
#                                Add Remote UObject
#------------------------------------------------------------------------------
# Create a remote UObject
# e.g. add_remote (name_of_binary SOURCES cpp1 cpp2..
#                  [DEPENDS target1 target2..] [NO_DEFAULT_MAIN])

MACRO (add_remote)

    parse_arguments (REMOTE_UOBJECT "SOURCES;DEPENDS" "NO_DEFAULT_MAIN" ${ARGN})
    set (REMOTE_UOBJECT_NAME ${REMOTE_UOBJECT_DEFAULT_ARGS})
    add_executable (${REMOTE_UOBJECT_NAME} ${REMOTE_UOBJECT_SOURCES})
    link_uobject_libraries (${REMOTE_UOBJECT_NAME})
    target_link_libraries (${REMOTE_UOBJECT_NAME} ${REMOTE_UOBJECT_DEPENDS})

ENDMACRO (add_remote)

#------------------------------------------------------------------------------
#                              Add Engine library
#------------------------------------------------------------------------------
# Creates a library ready to be linked to an engine kernel.
# e.g. add_engine_library (name_of_binary SOURCES cpp1 cpp2..
#                         [DEPENDS target1 target2..] [NO_DEFAULT_MAIN])

MACRO (add_engine_library)

    parse_arguments (ENGINE_UOBJECT "SOURCES;DEPENDS" "NO_DEFAULT_MAIN" ${ARGN})
    set (ENGINE_UOBJECT_NAME ${ENGINE_UOBJECT_DEFAULT_ARGS})
    add_library (${ENGINE_UOBJECT_NAME} ${ENGINE_UOBJECT_SOURCES})
    target_link_libraries (${ENGINE_UOBJECT_NAME} ${ENGINE_UOBJECT_DEPENDS})

ENDMACRO (add_engine_library)

#------------------------------------------------------------------------------
#                                 Add UObject
#------------------------------------------------------------------------------
# This macro groups the creation of build rules for both remote and engine
# UObject. It MUST be called in a project loaded by 'add_project' command to
# work properly.
# You can specify options into add_uobject brackets that will be applied to
# both uobject.

MACRO (add_uobject)

    add_remote(
        ${PROJECT_NAME}${UOBJECT_REMOTE_SUFFIX}
        SOURCES ${${PROJECT_NAME}_UOBJECT_SOURCES}
        DEPENDS ${PROJECT_LINKS}
        ${ARGN}
   )

    add_engine_library(
        ${PROJECT_NAME}${UOBJECT_ENGINE_LIBRARY_SUFFIX}
        SOURCES ${${PROJECT_NAME}_UOBJECT_SOURCES} ${${PROJECT_NAME}_WRAPPER_SOURCES}
        DEPENDS ${PROJECT_LINKS}
        ${ARGN}
   )

ENDMACRO (add_uobject)


#------------------------------------------------------------------------------
#                                  Add engine
#------------------------------------------------------------------------------
# FIXME: Implement me :)



#------------------------------------------------------------------------------
#                                 Add project
#------------------------------------------------------------------------------
# Call this function in the root CMakeLists.txt to add a project. If it's a
# standalone project, it will be equivalent to add_subdirectory command.
# However, if the project is linked to others just notice and all will be managed.
# e.g. add_project (project_folder [DEPENDS project_folder1 project_folder2..])

MACRO (add_project)

    parse_arguments (PROJECT "DEPENDS" "" ${ARGN})
    set (PROJECT_DIR ${PROJECT_DEFAULT_ARGS})
    set (PROJECT_LINKS)

    if (PROJECT_DEPENDS)
        foreach (PROJECT_DEPENDS_DIR ${PROJECT_DEPENDS})
            include_directories (${MODULES_SOURCE_DIR}/${PROJECT_DEPENDS_DIR})
            link_directories (${MODULES_SOURCE_DIR}/${PROJECT_DEPENDS_DIR})
            set (PROJECT_LINKS ${PROJECT_LINKS} ${PROJECT_DEPENDS}${UOBJECT_ENGINE_LIBRARY_SUFFIX})
        endforeach (PROJECT_DEPENDS_DIR)
    endif (PROJECT_DEPENDS)

    add_subdirectory (${PROJECT_DIR})

ENDMACRO (add_project)

#------------------------------------------------------------------------------
#                              Search library/path
#------------------------------------------------------------------------------
# Call find_library/find_path command. If the library/path still cannot be found,
# it prints a message you can customize by adding FULLNAME and PACKAGE info.
# You can ask for one library or one path. Not both.
# e.g. search(LIBRARY URBI_LIBRARY urbi FULLNAME "Urbi remote" PACKAGE "SDK Remote")

MACRO (search)

    # Check parameters and look for the element (library/path)
    parse_arguments (ELEMENT "LIBRARY;PATH;FULLNAME;PACKAGE" "" ${ARGN})
    if (ELEMENT_LIBRARY AND ELEMENT_PATH)
        message (FATAL_ERROR "Invalid call of search macro. You cannot ask for"
                             " both library and path finding.")
    endif (ELEMENT_LIBRARY AND ELEMENT_PATH)
    
    if (ELEMENT_LIBRARY)
        find_library (${ELEMENT_LIBRARY})
        set(ELEMENT_TYPE library)
    elseif (ELEMENT_PATH)
        find_path (${ELEMENT_PATH})
        set(ELEMENT_TYPE directory)
    else (ELEMENT_PATH)
        message (FATAL_ERROR "Invalid syntax. Specify LIBRARY or PATH section.")
    endif (ELEMENT_LIBRARY)
   
    # If we don't find the element
    if (NOT ${ARGV1})

        # Replace undefined parameters by generic ones
        if (NOT ELEMENT_FULLNAME)
            set (ELEMENT_FULLNAME ${ARGV2})
        else (NOT ELEMENT_FULLNAME)
            set (ELEMENT_FULLNAME ${ELEMENT_FULLNAME} " (" ${ARGV2} ")")
        endif (NOT ELEMENT_FULLNAME)

        if (NOT ELEMENT_PACKAGE)
            set (ELEMENT_PACKAGE it)
        endif (NOT ELEMENT_PACKAGE)

       # Custom message
       message ( STATUS ${ELEMENT_FULLNAME} " " ${ELEMENT_TYPE}
                 " has not been found. Make sure you've installed "
                 ${ELEMENT_PACKAGE} " on your computer or please specify the "
                 "library path manually (-D" ${ARGV1} ").")
    endif (NOT ${ARGV1})

ENDMACRO (search)

#------------------------------------------------------------------------------
#                           Link UObject libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# remote UObject
# e.g. link_uobject_libraries (uobject_name)

MACRO (link_uobject_libraries UOBJECT_NAME)
# e.g. "link_uobject_libraries (NAME_OF_EXECUTABLE CPP1 CPP2)"

    # Autofind libraries
    if (WIN32)
      set (CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "" "lib")
    endif (WIN32)

    search (LIBRARY SDK_REMOTE_LIBRARY urbi PACKAGE "URBI SDK remote")
    search (LIBRARY JPEG_LIBRARY jpeg)
    search (PATH SDK_REMOTE_INCLUDE_DIR urbi 
            FULLNAME "SDK Remote INCLUDE"
            PACKAGE  "URBI SDK Remote")

    # Keeps only required libs
    if (WIN32)
      set (CMAKE_CXX_STANDARD_LIBRARIES "gdi32.lib")
      set (CMAKE_C_STANDARD_LIBRARIES "gdi32.lib")
    endif (WIN32)

    # FIXME // Do not force release mode actually
    # Force release mode only
    if (WIN32)
      set (CMAKE_BUILD_TYPE "Release")
    endif (WIN32)

    # Link
    include_directories (${SDK_REMOTE_INCLUDE_DIR})
    target_link_libraries (${UOBJECT_NAME} ${SDK_REMOTE_LIBRARY} ${JPEG_LIBRARY})
    if (WIN32)
      target_link_libraries (${UOBJECT_NAME} ws2_32)
    elseif (UNIX)
      target_link_libraries (${UOBJECT_NAME} pthread)
      else (UNIX)
        message ("[UOBJECT] No auto-link has been pre-configured for your "
                 "platform into CMake.")
    endif (WIN32)

ENDMACRO (link_uobject_libraries)
