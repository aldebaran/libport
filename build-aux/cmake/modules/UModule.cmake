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
## ---------------------------------------------------------------
##

if(NOT MODULE_UMODULE_LOADED)
set(MODULE_UMODULE_LOADED TRUE)

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
#                                  Win32 Manifest
#------------------------------------------------------------------------------
# Gostai policy for windows manifest : embedded

MACRO(add_manifest TARGET_NAME)
  if(MSVC_VERSION)
    # FIXME: Waiting for closed ticked
    # If you set /MANIFEST:NO, CMake will remove automatic manifest declaration.
    # But if you specify then your own manifest, it will not disable the CMake's
    # generated one. Seems to be a bug which will be fixed soon (since bug report
    # ID0007222 has been fixed).
    # Since the automatic manifest is exactly what we want (embedded) there's
    # no trouble. Nevertheless, I added the same command (so no confliting)
    # in additionnal options in case the automatic manifest is changed
    # by CMake in the future : Then there will be an error at the link.
    #
    set_target_properties(${TARGET_NAME} PROPERTIES CONFIGURATION_TYPES "Release")
    set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "/MANIFEST /MANIFESTFILE:${TARGET_NAME}.dir\\Release\\${TARGET_NAME}.exe.intermediate.manifest")

  endif(MSVC_VERSION)
ENDMACRO(add_manifest)

# Force release mode only
# FIXME: Waiting for closed ticked
# Do not force release mode actually. This is a known bug
# in CMake still pending in bug reports list : ID 0006788.
# The variable has to be set before the parse of any CMakeLists
# (so at the call of CMake with -DCMAKE_CONFIGURATION_TYPES=Release)
if(WIN32)
  set(CMAKE_BUILD_TYPES "Release" FORCE)
  set(CMAKE_CONFIGURATION_TYPES "Release" FORCE)
endif(WIN32)


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
#
# For some reasons, you may need to prevent CMake from creating some targets.
# That's why you can specify into brackets the following OPTIONS :
#     - NO_REMOTE
#     - NO_ENGINE
#     - NO_TEST
#     - NO_LIBRARY
# e.g. If you've not SDK Engine installed, type add_uobject(NO_ENGINE)

MACRO(add_uobject)

    # Each project is identified by its directory name and can have A different
    # target name. If you want to change the output name of your uobjects
    # you should use :
    # set_target_properties(target_name PROPERTIES OUTPUT_NAME "foo")
    # where for target name is for the engine :
    # {PROJECT_NAME}${UOBJECT_ENGINE_TARGET_SUFFIX}
    #     Else, if you change target name or add executables yourself
    # you cannot ask for an automatic link in 'add_engine'
    # and 'add_umodule' with other umodules.
    # You will have to specify fullname of your custom targets
    # in the dependencies.

    set(${UMODULE_DIR}_TARGET_NAME ${PROJECT_NAME} PARENT_SCOPE)
    message(STATUS "Add workspace for project named: " ${PROJECT_NAME})
    parse_arguments(ADD_UOBJECT_WITH ""
                    "NO_ENGINE;NO_REMOTE;NO_TEST;NO_LIBRARY" ${ARGN})

    if(NOT ADD_UOBJECT_WITH_NO_REMOTE)
        add_remote(
            ${PROJECT_NAME}${UOBJECT_REMOTE_TARGET_SUFFIX}
            SOURCES ${${UMODULE_DIR}_UOBJECT_SOURCES}
            DEPENDS ${UMODULE_LINKS}
            ${ADD_UOBJECT_WITH_DEFAULT_ARGS})
    endif(NOT ADD_UOBJECT_WITH_NO_REMOTE)

    if(NOT ADD_UOBJECT_WITH_NO_LIBRARY)
        add_umodule_library(
            ${PROJECT_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX}
            SOURCES ${${UMODULE_DIR}_UOBJECT_SOURCES}
                    ${${UMODULE_DIR}_WRAPPER_SOURCES}
            DEPENDS ${UMODULE_LINKS}
            ${ADD_UOBJECT_WITH_DEFAULT_ARGS})
    endif(NOT ADD_UOBJECT_WITH_NO_LIBRARY)

    if(NOT ADD_UOBJECT_WITH_NO_ENGINE)
        add_engine(
            ${PROJECT_NAME}${UOBJECT_ENGINE_TARGET_SUFFIX}
            SOURCES ${${UMODULE_DIR}_UOBJECT_SOURCES}
            DEPENDS ${UMODULE_LINKS}
            ${ADD_UOBJECT_WITH_DEFAULT_ARGS})
    endif(NOT ADD_UOBJECT_WITH_NO_ENGINE)

    if(${UMODULE_DIR}_TEST_SOURCES)
        if(NOT ADD_UOBJECT_WITH_NO_TEST)
            add_wrapper_test(
                ${PROJECT_NAME}${UMODULE_TEST_TARGET_SUFFIX}
                SOURCES ${${UMODULE_DIR}_TEST_SOURCES}
                        ${${UMODULE_DIR}_WRAPPER_SOURCES}
                DEPENDS ${UMODULE_LINKS}
                        ${ADD_UOBJECT_WITH_DEFAULT_ARGS}
            )
        endif(NOT ADD_UOBJECT_WITH_NO_TEST)
    endif(${UMODULE_DIR}_TEST_SOURCES)

ENDMACRO(add_uobject)


#------------------------------------------------------------------------------
#                                Add Remote UObject
#------------------------------------------------------------------------------
# Create a remote UObject
# e.g. add_remote(name_of_binary SOURCES cpp1 cpp2..
#                  [DEPENDS target1 target2..] [NO_DEFAULT_MAIN])

MACRO(add_remote)

    parse_arguments(UOBJECT_REMOTE "SOURCES;DEPENDS" "NO_DEFAULT_MAIN" ${ARGN})
    set(UOBJECT_REMOTE_NAME ${UOBJECT_REMOTE_DEFAULT_ARGS})

    if(NOT UOBJECT_REMOTE_SOURCES OR NOT NO_DEFAULT_MAIN)
        find_package(Umain REQUIRED)
        include_directories(${UMAIN_INCLUDE_DIRS})
        list(APPEND UOBJECT_REMOTE_SOURCES ${UMAIN_CC_DIR}/umain.cc)
    endif(NOT UOBJECT_REMOTE_SOURCES OR NOT NO_DEFAULT_MAIN)

    # This policy command will be unnecessary after CMake 2.6
    cmake_policy(SET CMP0003 NEW)
    add_executable(${UOBJECT_REMOTE_NAME} ${UOBJECT_REMOTE_SOURCES})
    add_manifest(${UOBJECT_REMOTE_NAME})
    set_target_properties(${UOBJECT_REMOTE_NAME}
                          PROPERTIES COMPILE_DEFINITIONS URBI_ENV_REMOTE)
    link_remote_libraries(${UOBJECT_REMOTE_NAME})
    target_link_libraries(${UOBJECT_REMOTE_NAME} ${UOBJECT_REMOTE_DEPENDS})

ENDMACRO(add_remote)


#------------------------------------------------------------------------------
#                                Add Engine UObject
#------------------------------------------------------------------------------
# Allows you to create custom engine. Put in the UMODULES section the name
# of each umodule you want to put in (directory name ). You can also add
# custom library in the DEPENDS section.
# Fill the SOURCES section if you want to create directly the engine from
# sources.
# You can just fill UMODULES and DEPENDS sections. In such case, a .cc file
# will be generated to link your libs.
# e.g. add_engine(executable_name SOURCES cpp1 cpp2...
#                                 UMODULES module1 module2..
#                                 DEPENDS lib1 lib2..
#                                 [NO_DEFAULT_MAIN])

MACRO(add_engine)

    parse_arguments(UOBJECT_ENGINE "SOURCES;UMODULES;DEPENDS"
                    "NO_DEFAULT_MAIN" ${ARGN})
    set(UOBJECT_ENGINE_NAME ${UOBJECT_ENGINE_DEFAULT_ARGS})

    if(NOT UOBJECT_ENGINE_SOURCES OR NOT NO_DEFAULT_MAIN)
        find_package(Umain REQUIRED)
        include_directories(${UMAIN_INCLUDE_DIRS})
        list(APPEND UOBJECT_ENGINE_SOURCES ${UMAIN_CC_DIR}/umain.cc)
    endif(NOT UOBJECT_ENGINE_SOURCES OR NOT NO_DEFAULT_MAIN)

    # This policy command will be unnecessary after CMake 2.6
    cmake_policy(SET CMP0003 NEW)
    add_executable(${UOBJECT_ENGINE_NAME} ${UOBJECT_ENGINE_SOURCES})
    add_manifest(${UOBJECT_ENGINE_NAME})
    set_target_properties(${UOBJECT_REMOTE_NAME}
                          PROPERTIES COMPILE_DEFINITIONS URBI_ENV_REMOTE)

    # Link with needed umodules
    foreach(UOBJECT_ENGINE_UMODULE ${UOBJECT_ENGINE_UMODULES})
        target_link_libraries(${UOBJECT_ENGINE_NAME}
                              ${${UOBJECT_ENGINE_UMODULE}_TARGET_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX})
    endforeach(UOBJECT_ENGINE_UMODULE)

    # Link with custom libraries
    foreach(UOBJECT_ENGINE_DEPEND ${UOBJECT_ENGINE_DEPENDS})
        target_link_libraries(${UOBJECT_ENGINE_NAME} ${UOBJECT_ENGINE_DEPEND})
    endforeach(UOBJECT_ENGINE_DEPEND)

    # Link with urbi engine and dependencies
    link_engine_libraries(${UOBJECT_ENGINE_NAME})

ENDMACRO(add_engine)


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
#                           Link Remote libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# remote UObject
# e.g. link_uobject_libraries(uobject_name)

MACRO(link_remote_libraries UOBJECT_NAME)
# e.g. "link_remote_libraries(NAME_OF_EXECUTABLE)"

    find_package(SdkRemote REQUIRED)

    # Keeps only required libs
    if(WIN32)
      set(CMAKE_CXX_STANDARD_LIBRARIES)
      set(CMAKE_C_STANDARD_LIBRARIES)
    endif(WIN32)

    # Link
    include_directories(${SDK_REMOTE_INCLUDE_DIRS})
    target_link_libraries(${UOBJECT_NAME} ${SDK_REMOTE_LIBRARIES})

ENDMACRO(link_remote_libraries)


#------------------------------------------------------------------------------
#                           Link Engine libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# engine UObject.
# e.g. link_uobject_libraries(uobject_name)

MACRO(link_engine_libraries UOBJECT_NAME)
# e.g. "link_engine_libraries(NAME_OF_EXECUTABLE)"

    find_package(SdkEngine REQUIRED)
    include_directories(${SDK_ENGINE_INCLUDE_DIRS})
    target_link_libraries(${UOBJECT_NAME} ${SDK_ENGINE_LIBRARIES})

ENDMACRO(link_engine_libraries)

endif(NOT MODULE_UMODULE_LOADED)
