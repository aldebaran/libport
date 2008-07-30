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

    find_library (SDK_REMOTE_LIBRARY urbi)
    find_library (JPEG_LIBRARY jpeg)
    find_path (SDK_REMOTE_INCLUDE_DIR urbi)

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

    # Default libraries, in case they're not found
    if (NOT SDK_REMOTE_INCLUDE_DIR)
          message ("SDK Remote INCLUDE directory has not been found. Make sure"
                   " you've installed URBI SDK remote on your computer or "
                   "please specify the library path manually "
                   "(-DSDK_REMOTE_INCLUDE_DIR).")
    endif (NOT SDK_REMOTE_INCLUDE_DIR)

    if (NOT JPEG_LIBRARY)
          message ("The JPEG library has not been found. Make sure you've "
                   "installed URBI SDK remote on your computer or please "
                   "specify the library path manually (-DJPEG_LIBRARY).")
    endif (NOT JPEG_LIBRARY)

    if (NOT SDK_REMOTE_LIBRARY)
          message ("The URBI library has not been found. Make sure you've "
                   "installed URBI SDK remote on your computer or please "
                   "specify the library path manually (-DSDK_REMOTE_LIBRARY).")
    endif (NOT SDK_REMOTE_LIBRARY)

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
