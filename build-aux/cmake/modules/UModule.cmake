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
include(Tools)

#------------------------------------------------------------------------------
#                                 Default parameters
#------------------------------------------------------------------------------
# Install dirs
set(UMODULE_INSTALL_TYPES REMOTE ENGINE TEST LIBRARY)
set(UMODULE_REMOTE_INSTALL_SUBDIR bin/remote)
set(UMODULE_ENGINE_INSTALL_SUBDIR bin/engine)
set(UMODULE_TEST_INSTALL_SUBDIR bin/test)
set(UMODULE_LIBRARY_INSTALL_SUBDIR lib)

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

# Force release/debug mode only
# FIXME: Waiting for closed ticked
# Do not force release mode actually. This is a known bug
# in CMake still pending in bug reports list : ID 0006788.
# The variable has to be set before the parse of any CMakeLists
# (so at the call of CMake with -DCMAKE_CONFIGURATION_TYPES="Debug;Release")
if(MSVC_VERSION)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
else(MSVC_VERSION)
    if(SDK_DEBUG_MODE)
        set(CMAKE_BUILD_TYPE Debug)
    else(SDK_DEBUG_MODE)
        set(CMAKE_BUILD_TYPE Release)
    endif(SDK_DEBUG_MODE)
endif(MSVC_VERSION)

#------------------------------------------------------------------------------
#                                 Add UModule
#------------------------------------------------------------------------------
# Call this function in the root CMakeLists.txt to add a UModule. If it's a
# standalone project, it will be equivalent to add_subdirectory command.
# However, if the project is linked to others just notice and all will be
# managed.
# Shared libraries can be grabbed from other umodules. Nevertheless, to do so,
# you have to include those umodules with add_modules BEFORE using it as a
# dependence. Moreover, you must have created all targets from dependent
# umodules (with for example 'add_uobject' command) to grab all dependencies.
# Indeed, ${UMODULE_DIR}_${TYPE}_SHARED_LIBRARIES variables are set during
# those steps.
# e.g. add_umodule(umodule_folder [umodule_binary_dir]
#                  [DEPENDS umodule_folder1 umodule_folder2..])

MACRO(add_umodule)

    parse_arguments(UMODULE "DEPENDS" "" ${ARGN})
    list(LENGTH UMODULE_DEFAULT_ARGS args_length)
    list(GET UMODULE_DEFAULT_ARGS 0 UMODULE_DIR)
    if(args_length GREATER 1)
      list(GET UMODULE_DEFAULT_ARGS 1 UMODULE_BINARY_DIR)
    else()
      set(UMODULE_BINARY_DIR)
    endif()
    set(UMODULE_LINKS)

    foreach(i ${UMODULE_INSTALL_TYPES})
        set(${UMODULE_DIR}_${i}_SHARED_LIBRARIES)
    endforeach(i ${UMODULE_INSTALL_TYPES})

    if(UMODULE_DEPENDS)
        foreach(UMODULE_DEPENDS_DIR ${UMODULE_DEPENDS})
            if(IS_DIRECTORY ${UMODULE_DEPENDS_DIR})
                include_directories(${${PROJECT_NAME}_SOURCE_DIR}/${UMODULE_DEPENDS_DIR})
                link_directories(${${PROJECT_NAME}_SOURCE_DIR}/${UMODULE_DEPENDS_DIR})
                list(APPEND UMODULE_LINKS ${${UMODULE_DEPENDS_DIR}_TARGET_NAME}${UMODULE_LIBRARY_TARGET_SUFFIX})
            else(IS_DIRECTORY ${UMODULE_DEPENDS_DIR})
                list(APPEND UMODULE_LINKS ${UMODULE_DEPENDS})

                # Create a fake structure to make the list_shared_libraries call
                # easier.
                foreach(i ${UMODULE_INSTALL_TYPES})
                    set(${UMODULE_DEPENDS}_${i}_SHARED_LIBRARIES ${UMODULE_DEPENDS})
                endforeach(i ${UMODULE_INSTALL_TYPES})
            endif(IS_DIRECTORY ${UMODULE_DEPENDS_DIR})

            # Grab from dependencies shared libraries needed.
            foreach(i ${UMODULE_INSTALL_TYPES})
                list_shared_libraries(${UMODULE_DEPENDS_DIR}_${i}_SHARED_LIBRARIES
                                      ${UMODULE_DIR}_${i}_SHARED_LIBRARIES)
            endforeach(i ${UMODULE_INSTALL_TYPES})
        endforeach(UMODULE_DEPENDS_DIR)
    endif(UMODULE_DEPENDS)

    add_subdirectory(${UMODULE_DIR} ${UMODULE_BINARY_DIR})

ENDMACRO(add_umodule)


#------------------------------------------------------------------------------
#                              Add UModule library
#------------------------------------------------------------------------------
# Creates a library ready to be linked to an engine kernel.
# If you want to add this file to the INSTALL target, add INSTALL option.
# e.g. add_umodule_library(name_of_binary SOURCES cpp1 cpp2..
#                         [DEPENDS target1 target2..]
#                         [NO_DEFAULT_MAIN]
#                         [INSTALL])

MACRO(add_umodule_library)

    parse_arguments(UMODULE_LIBRARY "SOURCES;DEPENDS"
                    "NO_DEFAULT_MAIN;INSTALL" ${ARGN})
    set(UMODULE_LIBRARY_NAME ${UMODULE_LIBRARY_DEFAULT_ARGS})
    add_library(${UMODULE_LIBRARY_NAME} ${UMODULE_LIBRARY_SOURCES})
    target_link_libraries(${UMODULE_LIBRARY_NAME} ${UMODULE_LIBRARY_DEPENDS})

    if(UMODULE_LIBRARY_INSTALL)
        # Update INSTALL target
        add_install(LIBRARY ${UMODULE_LIBRARY_NAME})
    endif(UMODULE_LIBRARY_INSTALL)

ENDMACRO(add_umodule_library)

#------------------------------------------------------------------------------
#                                 Add UObject
#------------------------------------------------------------------------------
# This macro groups the creation of build rules for both remote and engine
# UObject.
# You can specify options, more dependencies/sources into add_uobject brackets
# that will be applied to both uobject.
# e.g. : add_uobject(DEPENDS ${FFTW_LIBRARY})
#
# You must have set first :
#
# - PROJECT_NAME                    The generic name of your target. You must
#                                   call command 'project(your_target_name)'.
#                                   It will set for you the variable.
#                                   Suffixes for remote/engine/library will be
#                                   added automatically. See the header.
# - ${UMODULE_DIR}_UOBJECT_SOURCES  List of .cXX files of you uobject.
# - ${UMODULE_DIR}_WRAPPER_SOURCES  Optional. List of .cXX of your library
#                                   wrapper used by the uobject.
# - ${UMODULE_DIR}_TEST_SOURCES     Optional. Your .cXX files used to test
#                                   your wrapper. This will create an
#                                   executable with no uobject links.
#
# For some reasons, you may need to prevent CMake from creating some targets.
# That's why you can specify into brackets the following OPTIONS :
#     - NO_REMOTE
#     - NO_ENGINE
#     - NO_TEST
#     - NO_LIBRARY
# e.g. If you've not SDK Engine installed, type add_uobject(NO_ENGINE)
#
# The install option is automatically added. This will install
# engine/remote targets in install directory with all shared libraries
# needed. Generic paths can be customized in the header of this file.
# If you want a specific folder tree for your UModule, you've to set
# manually in your CMakeLists.txt the following variables :
# ${UMODULE_DIR}_REMOTE_INSTALL_SUBDIR
# ${UMODULE_DIR}_ENGINE_INSTALL_SUBDIR
# ${UMODULE_DIR}_TEST_INSTALL_SUBDIR
# ${UMODULE_DIR}_LIBRARY_INSTALL_SUBDIR
#
# If you just want to specify a different prefix for your UModule
# (and keep the default folder tree), just add into brackets
# INSTALL my/custom/path. You path can be even relative (this
# will append default install prefix) or absolute.
# You can also set yourself the ${UMODULE_DIR}_INSTALL_PREFIX variable.
#     Add NO_INSTALL to disable the default install option.


MACRO(add_uobject)

    # Each project is identified by its directory name and can have A different
    # target name. If you want to change the output name of your uobjects
    # you should use :
    # set_target_properties(target_name PROPERTIES OUTPUT_NAME "foo")
    # where <target_name> is for the engine :
    # {PROJECT_NAME}${UOBJECT_ENGINE_TARGET_SUFFIX}
    #     Else, if you change target name or add executables yourself
    # you cannot ask for an automatic link in 'add_engine'
    # and 'add_umodule' with other umodules.
    # You will have to specify fullname of your custom targets
    # in the dependencies.

    set(${UMODULE_DIR}_TARGET_NAME ${PROJECT_NAME} PARENT_SCOPE)
    message(STATUS "Add workspace for project named: " ${PROJECT_NAME})
    parse_arguments(ADD_UOBJECT_WITH
                    "INSTALL"
                    "NO_ENGINE;NO_REMOTE;NO_TEST;NO_LIBRARY;NO_INSTALL"
                    ${ARGN})

    if(ADD_UOBJECT_WITH_INSTALL)
        set(${UMODULE_DIR}_INSTALL_OLD_PREFIX ${${UMODULE_DIR}_INSTALL_PREFIX})
        set(${UMODULE_DIR}_INSTALL_PREFIX ${ADD_UOBJECT_WITH_INSTALL})
    endif(ADD_UOBJECT_WITH_INSTALL)

    # Automatic INSTALL option
    if(NOT ADD_UOBJECT_WITH_NO_INSTALL)
        list(APPEND ADD_UOBJECT_WITH_DEFAULT_ARGS "INSTALL")
    endif(NOT ADD_UOBJECT_WITH_NO_INSTALL)

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

    # Reset install prefix.
    if(ADD_UOBJECT_WITH_INSTALL)
        set(${UMODULE_DIR}_INSTALL_PREFIX ${${UMODULE_DIR}_INSTALL_OLD_PREFIX})
    endif(ADD_UOBJECT_WITH_INSTALL)

ENDMACRO(add_uobject)


#------------------------------------------------------------------------------
#                                Add Remote UObject
#------------------------------------------------------------------------------
# Create a remote UObject.
# If you have your own main in sources, add NO_DEFAULT_MAIN option.
# If you want to add this file to the INSTALL target, add INSTALL option.
# e.g. add_remote(name_of_binary SOURCES cpp1 cpp2..
#                  [DEPENDS target1 target2..] [NO_DEFAULT_MAIN][INSTALL])

MACRO(add_remote)

    parse_arguments(UOBJECT_REMOTE "SOURCES;DEPENDS"
                    "NO_DEFAULT_MAIN;INSTALL" ${ARGN})
    set(UOBJECT_REMOTE_NAME ${UOBJECT_REMOTE_DEFAULT_ARGS})

    if(NOT NO_DEFAULT_MAIN)
        find_package(Umain REQUIRED)
        include_directories(${UMAIN_INCLUDE_DIRS})
        list(APPEND UOBJECT_REMOTE_SOURCES ${UMAIN_CC_DIR}/umain.cc)
        list(APPEND ${UMODULE_DIR}_REMOTE_SHARED_LIBRARIES ${UMAIN_SHARED_LIBRARIES})
    endif(NOT NO_DEFAULT_MAIN)

    # This policy command will be unnecessary after CMake 2.6
    cmake_policy(SET CMP0003 NEW)
    add_executable(${UOBJECT_REMOTE_NAME} ${UOBJECT_REMOTE_SOURCES})
    add_manifest(${UOBJECT_REMOTE_NAME})
    set_target_properties(${UOBJECT_REMOTE_NAME}
                          PROPERTIES COMPILE_DEFINITIONS URBI_ENV_REMOTE)

    # Link with urbi remote and dependencies
    link_remote_libraries(${UOBJECT_REMOTE_NAME})
    target_link_libraries(${UOBJECT_REMOTE_NAME} ${UOBJECT_REMOTE_DEPENDS})
    list_shared_libraries(UOBJECT_REMOTE_DEPENDS
                          ${UMODULE_DIR}_REMOTE_SHARED_LIBRARIES)

    if(UOBJECT_REMOTE_INSTALL)
        # Update INSTALL target
        add_install(REMOTE ${UOBJECT_REMOTE_NAME})
    endif(UOBJECT_REMOTE_INSTALL)

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
# If you have your own main in sources, add NO_DEFAULT_MAIN option.
# If you want to add this file to the INSTALL target, add INSTALL option.
# e.g. add_engine(executable_name SOURCES cpp1 cpp2...
#                                 UMODULES module1 module2..
#                                 DEPENDS lib1 lib2..
#                                 [NO_DEFAULT_MAIN])

MACRO(add_engine)

    parse_arguments(UOBJECT_ENGINE "SOURCES;UMODULES;DEPENDS"
                    "NO_DEFAULT_MAIN;INSTALL" ${ARGN})
    set(UOBJECT_ENGINE_NAME ${UOBJECT_ENGINE_DEFAULT_ARGS})

    if(NOT NO_DEFAULT_MAIN)
        find_package(Umain REQUIRED)
        include_directories(${UMAIN_INCLUDE_DIRS})
        list(APPEND UOBJECT_ENGINE_SOURCES ${UMAIN_CC_DIR}/umain.cc)
        list(APPEND ${UMODULE_DIR}_ENGINE_SHARED_LIBRARIES
                    ${UMAIN_SHARED_LIBRARIES})
    endif(NOT NO_DEFAULT_MAIN)

    # This policy command will be unnecessary after CMake 2.6
    cmake_policy(SET CMP0003 NEW)
    add_executable(${UOBJECT_ENGINE_NAME} ${UOBJECT_ENGINE_SOURCES})
    add_manifest(${UOBJECT_ENGINE_NAME})
    set_target_properties(${UOBJECT_ENGINE_NAME}
                          PROPERTIES COMPILE_DEFINITIONS URBI_ENV_ENGINE)

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
    list_shared_libraries(UOBJECT_ENGINE_DEPENDS
                          ${UMODULE_DIR}_ENGINE_SHARED_LIBRARIES)

    if(UOBJECT_ENGINE_INSTALL)
        # Update INSTALL target
        add_install(ENGINE ${UOBJECT_ENGINE_NAME})
    endif(UOBJECT_ENGINE_INSTALL)

ENDMACRO(add_engine)


#------------------------------------------------------------------------------
#                                   Add test
#------------------------------------------------------------------------------
# Add a binary test for your eventual library wrapper linked to your uobject.
# If you want to add this file to the INSTALL target, add INSTALL option.
# syntax: add_wrapper_test(target_name SOURCES cpp1 cpp2..
#                                      [DEPENDS lib/target1 lib/target2]
#                                      [INSTALL])
# e.g.: add_wrapper_test(fft-test SOURCES fft.cc ufft-test.cc DEPENDS fftw3)

MACRO(add_wrapper_test)

    parse_arguments(WRAPPER_TEST "SOURCES;DEPENDS" "INSTALL" ${ARGN})
    add_executable(${WRAPPER_TEST_DEFAULT_ARGS} ${WRAPPER_TEST_SOURCES})
    target_link_libraries(${WRAPPER_TEST_DEFAULT_ARGS} ${WRAPPER_TEST_DEPENDS})
    list_shared_libraries(WRAPPER_TEST_DEPENDS
                          ${UMODULE_DIR}_TEST_SHARED_LIBRARIES)

    if(WRAPPER_TEST_INSTALL)
        # Update INSTALL target
        add_install(TEST ${WRAPPER_TEST_DEFAULT_ARGS})
    endif(WRAPPER_TEST_INSTALL)

ENDMACRO(add_wrapper_test)


#------------------------------------------------------------------------------
#                           Link Remote libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# remote UObject
# e.g. link_uobject_libraries(target_name)

MACRO(link_remote_libraries UOBJECT_NAME)

    find_package(SdkRemote REQUIRED)

    # Keeps only required libs
    if(WIN32)
      set(CMAKE_CXX_STANDARD_LIBRARIES)
      set(CMAKE_C_STANDARD_LIBRARIES)
    endif(WIN32)

    # Link
    include_directories(${SDK_REMOTE_INCLUDE_DIRS})
    target_link_libraries(${UOBJECT_NAME} ${SDK_REMOTE_LIBRARIES})
    list(APPEND ${UMODULE_DIR}_REMOTE_SHARED_LIBRARIES
                ${SDK_REMOTE_SHARED_LIBRARIES})

ENDMACRO(link_remote_libraries)


#------------------------------------------------------------------------------
#                           Link Engine libraries
#------------------------------------------------------------------------------
# Link your current executable with all libraries required for a standard
# engine UObject.
# e.g. link_uobject_libraries(target_name)

MACRO(link_engine_libraries UOBJECT_NAME)

    find_package(SdkEngine REQUIRED)

    # Link
    include_directories(${SDK_ENGINE_INCLUDE_DIRS})
    target_link_libraries(${UOBJECT_NAME} ${SDK_ENGINE_LIBRARIES})
    list(APPEND ${UMODULE_DIR}_ENGINE_SHARED_LIBRARIES
                ${SDK_ENGINE_SHARED_LIBRARIES})

ENDMACRO(link_engine_libraries)

endif(NOT MODULE_UMODULE_LOADED)


#------------------------------------------------------------------------------
#                                   Install
#------------------------------------------------------------------------------
# Enable the install of the specified target. This will also add dependent
# shared libraries. If there's no given subdirectory, the macro will use
# the umodule default one (UMODULE_type_INSTALL_SUBDIR).
# syntax: add_install(ENGINE|REMOTE|TEST|LIBRARY target_name
#                     [DESTINATION subdirectory])
# e.g.  : add_install(ENGINE fft-engine)


MACRO(add_install)

    # We cannot set the default values somewhere else without
    # the doubt it has not been set before add_install call.
    foreach(i ${UMODULE_INSTALL_TYPES})
        if(NOT ${UMODULE_DIR}_${i}_INSTALL_SUBDIR)
            set(${UMODULE_DIR}_${i}_INSTALL_SUBDIR
                ${UMODULE_${i}_INSTALL_SUBDIR})
        endif(NOT ${UMODULE_DIR}_${i}_INSTALL_SUBDIR)
    endforeach(i ${UMODULE_INSTALL_TYPES})

    parse_arguments(ADD_INSTALL "DESTINATION" "" ${ARGN})
    list(GET ADD_INSTALL_DEFAULT_ARGS 0 TARGET_TYPE)
    list(GET ADD_INSTALL_DEFAULT_ARGS 1 TARGET_NAME)

    # Add a slash to the prefix if needed
    set(prefix ${${UMODULE_DIR}_INSTALL_PREFIX})
    if(prefix)
        string(REGEX MATCH /$ TEMP ${prefix})
        if(NOT TEMP)
          set(prefix ${prefix}/)
        endif(NOT TEMP)
    endif(prefix)
    set(subdir ${${UMODULE_DIR}_${TARGET_TYPE}_INSTALL_SUBDIR})

    if(NOT ADD_INSTALL_DESTINATION)
        set(${TARGET_NAME}_SUBDIR ${prefix}${subdir})
        install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${${TARGET_NAME}_SUBDIR}
        LIBRARY DESTINATION ${${TARGET_NAME}_SUBDIR}
        ARCHIVE DESTINATION ${${TARGET_NAME}_SUBDIR})
    else(NOT ADD_INSTALL_DESTINATION)
        # Inferred: Call with no custom install dirs or calls from add_uobject
        if(${TARGET_TYPE} STREQUAL "ENGINE")
            set(${TARGET_NAME}_SUBDIR ${${UMODULE_DIR}_ENGINE_INSTALL_SUBDIR})
        else(${TARGET_TYPE} STREQUAL "ENGINE")
            set(${TARGET_NAME}_SUBDIR ${${UMODULE_DIR}_REMOTE_INSTALL_SUBDIR})
        endif(${TARGET_TYPE} STREQUAL "ENGINE")

        install(TARGETS ${TARGET_NAME}
                RUNTIME DESTINATION ${prefix}${${TARGET_NAME}_SUBDIR}
                LIBRARY DESTINATION ${prefix}${${UMODULE_DIR}_LIBRARY_INSTALL_SUBDIR}
                ARCHIVE DESTINATION ${prefix}${${TARGET_NAME}_LIBRARY_INSTALL_SUBDIR})
    endif(NOT ADD_INSTALL_DESTINATION)

    # Install shared libraries
    install(FILES ${${UMODULE_DIR}_${TARGET_TYPE}_SHARED_LIBRARIES}
                      DESTINATION ${prefix}${UMODULE_${TARGET_TYPE}_INSTALL_SUBDIR})


ENDMACRO(add_install)

