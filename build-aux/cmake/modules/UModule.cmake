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
## User-friendly macros for UObject projects.
## ------------------------------- -------------------------------
##

include(Macro-toolbox)
# parse_arguments macro

#------------------------------------------------------------------------------
#                                 Default parameters
#------------------------------------------------------------------------------
# Output default naming
set(UOBJECT_REMOTE_TARGET_SUFFIX -remote)
set(UOBJECT_ENGINE_TARGET_SUFFIX -engine)
set(UMODULE_TEST_TARGET_SUFFIX -test)
set(UMODULE_LIBRARY_TARGET_SUFFIX)

# Input default naming
set(UMODULE_UOBJECT_SOURCE_PREFIX)
set(UMODULE_WRAPPER_SOURCE_PREFIX)
set(UMODULE_TEST_SOURCE_PREFIX)

#------------------------------------------------------------------------------
#                                Add Remote UObject
#------------------------------------------------------------------------------
# Create a remote UObject
# e.g. add_remote(name_of_binary SOURCES cpp1 cpp2..
#                  [DEPENDS target1 target2..] [NO_DEFAULT_MAIN])

MACRO(add_remote)

    parse_arguments(REMOTE_UOBJECT "SOURCES;DEPENDS" "NO_DEFAULT_MAIN" ${ARGN})
    set(REMOTE_UOBJECT_NAME ${REMOTE_UOBJECT_DEFAULT_ARGS})
    add_executable(${REMOTE_UOBJECT_NAME} ${REMOTE_UOBJECT_SOURCES})
    link_uobject_libraries(${REMOTE_UOBJECT_NAME})
    target_link_libraries(${REMOTE_UOBJECT_NAME} ${REMOTE_UOBJECT_DEPENDS})

ENDMACRO(add_remote)

#------------------------------------------------------------------------------
#                              Add UModule library
#------------------------------------------------------------------------------
# Creates a library ready to be linked to an engine kernel.
# e.g. add_umodule_library(name_of_binary SOURCES cpp1 cpp2..
#                         [DEPENDS target1 target2..] [NO_DEFAULT_MAIN])

MACRO(add_umodule_library)

    parse_arguments(UMODULE_LIBRARY "SOURCES;DEPENDS" "NO_DEFAULT_MAIN" ${ARGN})
    set(UMODULE_LIBRARY_NAME ${UMODULE_LIBRARY_DEFAULT_ARGS})
    add_library(${UMODULE_LIBRARY_NAME} ${UMODULE_LIBRARY_SOURCES})
    target_link_libraries(${UMODULE_LIBRARY_NAME} ${UMODULE_LIBRARY_DEPENDS})

ENDMACRO(add_umodule_library)

#------------------------------------------------------------------------------
#                                 Add UObject
#------------------------------------------------------------------------------
# This macro groups the creation of build rules for both remote and engine
# UObject.
# You can specify options, more dependencies/sources into add_uobject brackets
# that will be applied to both uobject.
# You must have set first :
#
# - PROJECT_NAME                    The generic name of your target. You must
#                                   call command 'project(your_target_name)'.
#                                   It will set for you the variable.
#                                   Suffixes for remote/engine/library will be
#                                   added automatically. See the header.
# - ${UMODULE_DIR}_UOBJECT_SOURCES  List of .cXX files of you uobject.
# - ${UMODULE_DIR}_WRAPPER_SOURCES  Optional. List of .cXX of you library
#                                   wrapper used by the uobject.
# - ${UMODULE_DIR}_TEST_SOURCES     Optional. Your .cXX files used to test
#                                   you wrapper. This will create an
#                                   executable with no uobject links.

MACRO(add_uobject)

    # Each project is identified by its directory name and can have A different
    # target name. Else you cannot ask for an automatic link in 'add_engine'
    # and 'add_umodule' with other umodules.
    set(${UMODULE_DIR}_TARGET_NAME ${PROJECT_NAME} PARENT_SCOPE)
    message(STATUS "Add workspace for project named: " ${PROJECT_NAME})

    add_remote(
        ${PROJECT_NAME}${UOBJECT_REMOTE_TARGET_SUFFIX}
        SOURCES ${${UMODULE_DIR}_UOBJECT_SOURCES}
        DEPENDS ${UMODULE_LINKS}
        ${ARGN}
    )

    add_umodule_library(
        ${PROJECT_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX}
        SOURCES ${${UMODULE_DIR}_UOBJECT_SOURCES} ${${UMODULE_DIR}_WRAPPER_SOURCES}
        DEPENDS ${UMODULE_LINKS}
        ${ARGN}
    )

#   add_engine

    if(${UMODULE_DIR}_TEST_SOURCES)
        add_wrapper_test(
            ${PROJECT_NAME}${UMODULE_TEST_TARGET_SUFFIX}
            SOURCES ${${UMODULE_DIR}_TEST_SOURCES}
                    ${${UMODULE_DIR}_WRAPPER_SOURCES}
            DEPENDS ${UMODULE_LINKS}
                    ${ARGN}
        )
    endif(${UMODULE_DIR}_TEST_SOURCES)

ENDMACRO(add_uobject)


#------------------------------------------------------------------------------
#                                   Add test
#------------------------------------------------------------------------------
# Add a binary test for your eventual library wrapper linked to your uobject.

MACRO(add_wrapper_test)

    parse_arguments(WRAPPER_TEST "SOURCES;DEPENDS" "" ${ARGN})
    add_executable(${WRAPPER_TEST_DEFAULT_ARGS} ${WRAPPER_TEST_SOURCES})
    target_link_libraries(${WRAPPER_TEST_DEFAULT_ARGS} ${WRAPPER_TEST_DEPENDS})

ENDMACRO(add_wrapper_test)


#------------------------------------------------------------------------------
#                                  Add engine
#------------------------------------------------------------------------------
# Allows you to create custom engine. Put in the UMODULES section the name
# of each umodule you want to put in (directory name ). You can also add
# custom library in the DEPENDS section.
# You can just fill UMODULES and DEPENDS sections. In such case, a .cc file
# will be generated to link your libs.
# e.g. add_engine(executable_name UMODULES module1 module2.. DEPENDS lib1 lib2..)

MACRO(add_engine)

    parse_arguments(ENGINE "SOURCES;UMODULES;DEPENDS" "" ${ARGN})

    if(NOT ENGINE_SOURCES)
        message(STATUS "Add a generated .cc file")
        set(ENGINE_SOURCES /home/petit/engine-1.5.1/share/umain/umain.cc)
    endif(NOT ENGINE_SOURCES)

    set(UOBJECT_NAME ${ENGINE_DEFAULT_ARGS}${UOBJECT_ENGINE_TARGET_SUFFIX})

    include_directories(/home/petit/engine-1.5.1/include)
    add_executable(${UOBJECT_NAME} ${ENGINE_SOURCES})

    # Link with needed umodules
    foreach(ENGINE_UMODULE ${ENGINE_UMODULES})
        target_link_libraries(${UOBJECT_NAME}
                              ${${ENGINE_UMODULE}_TARGET_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX})
    endforeach(ENGINE_UMODULE)

    # Link with custom libraries
    foreach(ENGINE_DEPEND ${ENGINE_DEPENDS})
        target_link_libraries(${UOBJECT_NAME} ${ENGINE_DEPEND})
    endforeach(ENGINE_DEPEND)

    # Link with urbi engine and dependencies

    # FIXME: To be continued :)

ENDMACRO(add_engine)


#------------------------------------------------------------------------------
#                                 Add UModule
#------------------------------------------------------------------------------
# Call this function in the root CMakeLists.txt to add a UModule. If it's a
# standalone project, it will be equivalent to add_subdirectory command.
# However, if the project is linked to others just notice and all will be
# managed.
# e.g. add_umodule(umodule_folder [DEPENDS umodule_folder1 umodule_folder2..])

MACRO(add_umodule)

    parse_arguments(UMODULE "DEPENDS" "" ${ARGN})
    set(UMODULE_DIR ${UMODULE_DEFAULT_ARGS})
    set(UMODULE_LINKS)

    if(UMODULE_DEPENDS)
        foreach(UMODULE_DEPENDS_DIR ${UMODULE_DEPENDS})
            include_directories(${MODULES_SOURCE_DIR}/${UMODULE_DEPENDS_DIR})
            link_directories(${MODULES_SOURCE_DIR}/${UMODULE_DEPENDS_DIR})
            list(APPEND UMODULE_LINKS ${${UMODULE_DEPENDS}_TARGET_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX})
        endforeach(UMODULE_DEPENDS_DIR)
    endif(UMODULE_DEPENDS)

    add_subdirectory(${UMODULE_DIR})

ENDMACRO(add_umodule)


#------------------------------------------------------------------------------
#                           Link UObject libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# remote UObject
# e.g. link_uobject_libraries(uobject_name)

MACRO(link_uobject_libraries UOBJECT_NAME)
# e.g. "link_uobject_libraries(NAME_OF_EXECUTABLE CPP1 CPP2)"

    find_package(SdkRemoteNew)

    # Keeps only required libs
    if(WIN32)
      set(CMAKE_CXX_STANDARD_LIBRARIES "gdi32.lib")
      set(CMAKE_C_STANDARD_LIBRARIES "gdi32.lib")
    endif(WIN32)

    # FIXME // Do not force release mode actually
    # Force release mode only
    if(WIN32)
      set(CMAKE_BUILD_TYPE "Release")
    endif(WIN32)

    # Link
    include_directories(${SDK_REMOTE_NEW_INCLUDE_DIRS})
    target_link_libraries(${UOBJECT_NAME} ${SDK_REMOTE_NEW_LIBRARIES})

ENDMACRO(link_uobject_libraries)
