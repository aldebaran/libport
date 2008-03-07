# - Try to find Boost
#
# FIXME: This macro does not work properly. We still use it because we don't
#  require too much yet. We are looking forward to use FindBoostV2 that requires
#  cmake 2.5.
#
# Once done this will define
#
#  BOOST_FOUND - System has Boost
#  BOOST_INCLUDE_DIRS - Boost include directory
#  BOOST_LIBRARIES - Link these to use Boost
#  BOOST_LIBRARY_DIRS - The path to where the Boost library files are.
#  BOOST_DEFINITIONS - Compiler switches required for using Boost
#  BOOST_LIBRARIES_SUFFIX - Boost libraries name suffix (e.g -vc71-mt-gd-1_34,
#                                                         -gcc41-mt...)
#
#  Use these variables to control which libraries will be used for linking.
#  BOOST_USE_DATE_TIME - Link against the date_time Library
#  BOOST_USE_FILESYSTEM - Link against the filesystem Library
#  BOOST_USE_IOSTREAMS - Link against the iostreams Library
#  BOOST_USE_PRO_EXEC_MONITOR - Link against the prg_exec_monitor Library
#  BOOST_USE_PROGRAM_OPTIONS - Link against the program_options Library
#  BOOST_USE_PYTHON - Link against the python Library
#  BOOST_USE_REGEX - Link against the regex Library
#  BOOST_USE_SERIALIZATION - Link against the serialization Library
#  BOOST_USE_SIGNALS - Link against the signals Library
#  BOOST_USE_TEST_EXEC_MONITOR - Link against the test_exec_monitor Library
#  BOOST_USE_THREAD - Link against the thread Library
#  BOOST_USE_UNIT_TEST_FRAMEWORK - Link against the unit_test_framework Library
#  BOOST_USE_WSERIALIZATION - Link against the wserialization Library
#
#  BOOST_DATE_TIME_FOUND               True IF Boost Date Time was found.
#  BOOST_FILESYSTEM_FOUND              True IF Boost Filesystem was found.
#  BOOST_IOSTREAMS_FOUND               True IF Boost Iostream was found.
#  BOOST_PRG_EXEC_MONITOR_FOUND        True IF Boost Program Exec Monitor was
#                                      found.
#  BOOST_PROGRAM_OPTIONS_FOUND         True IF Boost Program Options was found.
#  BOOST_PYTHON_FOUND                  True IF Boost Python was found.
#  BOOST_REGEX_FOUND                   True IF Boost Regex was found.
#  BOOST_SERIALIZATION_FOUND           True IF Boost Serialization was found.
#  BOOST_SIGNALS_FOUND                 True IF Boost Signals was found.
#  BOOST_TEST_EXEC_MONITOR_FOUND       True IF Boost Test Exec Monitor was found.
#  BOOST_THREAD-MT_FOUND               True IF Boost Thread was found.
#  BOOST_UNIT_TEST_FRAMEWORK_FOUND     True IF Boost Unit Test Framework was found.
#  BOOST_WSERIALIZATION_FOUND          True IF Boost WSerialization was found.
#
#  BOOST_DATE_TIME_LIBRARY     The Boost date_time libraries
#  BOOST_DATE_TIME_LIBRARY_DEBUG     The Boost date_time debug library
#  BOOST_DATE_TIME_LIBRARY_RELEASE   The Boost date_time release library
#
#  BOOST_FILESYSTEM_LIBRARY     The Boost filesystem libraries
#  BOOST_FILESYSTEM_LIBRARY_DEBUG     The Boost filesystem debug library
#  BOOST_FILESYSTEM_LIBRARY_RELEASE   The Boost filesystem release library
#
#  BOOST_IOSTREAMS_LIBRARY     The Boost iostreams libraries
#  BOOST_IOSTREAMS_LIBRARY_DEBUG     The Boost iostreams debug library
#  BOOST_IOSTREAMS_LIBRARY_RELEASE   The Boost iostreams release library
#
#  BOOST_PRO_EXEC_MONITOR_LIBRARY     The Boost prg_exec_monitor libraries
#  BOOST_PRO_EXEC_MONITOR_LIBRARY_DEBUG     The Boost prg_exec_monitor debug library
#  BOOST_PRO_EXEC_MONITOR_LIBRARY_RELEASE   The Boost prg_exec_monitor release library
#
#  BOOST_PROGRAM_OPTIONS_LIBRARY     The Boost program_options libraries
#  BOOST_PROGRAM_OPTIONS_LIBRARY_DEBUG     The Boost program_options debug library
#  BOOST_PROGRAM_OPTIONS_LIBRARY_RELEASE   The Boost program_options release library
#
#  BOOST_PYTHON_LIBRARY     The Boost python libraries
#  BOOST_PYTHON_LIBRARY_DEBUG     The Boost python debug library
#  BOOST_PYTHON_LIBRARY_RELEASE   The Boost python release library
#
#  BOOST_REGEX_LIBRARY     The Boost regex libraries
#  BOOST_REGEX_LIBRARY_DEBUG     The Boost regex debug library
#  BOOST_REGEX_LIBRARY_RELEASE   The Boost regex release library
#
#  BOOST_SERIALIZATION_LIBRARY     The Boost serialization libraries
#  BOOST_SERIALIZATION_LIBRARY_DEBUG     The Boost serialization debug library
#  BOOST_SERIALIZATION_LIBRARY_RELEASE   The Boost serialization release library
#
#  BOOST_SIGNALS_LIBRARY     The Boost signals libraries
#  BOOST_SIGNALS_LIBRARY_DEBUG     The Boost signals debug library
#  BOOST_SIGNALS_LIBRARY_RELEASE   The Boost signals release library
#
#  BOOST_TEST_EXEC_MONITOR_LIBRARY     The Boost test_exec_monitor libraries
#  BOOST_TEST_EXEC_MONITOR_LIBRARY_DEBUG     The Boost test_exec_monitor debug library
#  BOOST_TEST_EXEC_MONITOR_LIBRARY_RELEASE   The Boost test_exec_monitor release library
#
#  BOOST_THREAD_LIBRARY     The Boost thread libraries
#  BOOST_THREAD_LIBRARY_DEBUG     The Boost thread debug library
#  BOOST_THREAD_LIBRARY_RELEASE   The Boost thread release library
#
#  BOOST_UNIT_TEST_FRAMEWORK_LIBRARY     The Boost unit_test_framework libraries
#  BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG     The Boost unit_test_framework debug library
#  BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE   The Boost unit_test_framework release library
#
#  BOOST_WSERIALIZATION_LIBRARY     The Boost wserialization libraries
#  BOOST_WSERIALIZATION_LIBRARY_DEBUG     The Boost wserialization debug library
#  BOOST_WSERIALIZATION_LIBRARY_RELEASE   The Boost wserialization release library
#
#  Copyright (c) 2006 Andreas Schneider <mail at cynapses.org>
#  Copyright (c) 2007 Wengo
#
#  Redistribution AND use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

MESSAGE(STATUS "Finding Boost libraries.... ")
IF (NOT DEFINED BOOST_CURRENT_VERSION)
  SET ( BOOST_CURRENT_VERSION "1_34_1")
ENDIF (NOT DEFINED BOOST_CURRENT_VERSION)
SET (BOOST_LIBRARIES "")
SET (BOOST_INCLUDE_DIRS "")
IF (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
  # in cache already
  SET(BOOST_FOUND TRUE)
ELSE (BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)

  # Add in some path suffixes. These will have to be updated whenever
  # a new Boost version comes out.
  SET(BOOST_PATH_SUFFIX
    boost-${BOOST_CURRENT_VERSION}
    )

  SET(BOOST_INCLUDE_SEARCH_DIRS
    $ENV{BOOSTINCLUDEDIR}
    $ENV{BOOST_ROOT}/include
    C:/boost/include
    "C:/Program Files/boost/boost_${BOOST_CURRENT_VERSION}"
    # D: is very often the cdrom drive, IF you don't have a
    # cdrom inserted it will popup a very annoying dialog
    #D:/boost/include
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
    )

  SET(BOOST_LIBRARIES_SEARCH_DIRS
    $ENV{BOOSTLIBDIR}
    $ENV{BOOST_ROOT}/lib
    C:/boost/lib
    "C:/Program Files/boost/boost_${BOOST_CURRENT_VERSION}/lib"
    /usr/lib
    /usr/local/lib
    /opt/local/lib
    /sw/lib
    )
  # MESSAGE (STATUS "BOOST_LIBRARIES_SEARCH_DIRS: ${BOOST_LIBRARIES_SEARCH_DIRS}")
  IF (WIN32)
    # In windows, automatic linking is performed, so you do not have to
    # specify the libraries. If you are linking to a dynamic runtime, then you
    # can choose to link to either a static or a dynamic Boost library, the
    # default is to do a static link. You can alter this for a specific
    # library "whatever" by defining BOOST_WHATEVER_DYN_LINK to force Boost
    # library "whatever" to be linked dynamically.  Alternatively you can
    # force all Boost libraries to dynamic link by defining
    # BOOST_ALL_DYN_LINK.

    # This feature can be disabled for Boost library "whatever" by defining
    # BOOST_WHATEVER_NO_LIB, or for all of Boost by defining BOOST_ALL_NO_LIB.

    # If you want to observe which libraries are being linked against then
    # defining BOOST_LIB_DIAGNOSTIC will cause the auto-linking code to emit a
    # pragma MESSAGE each time a library is selected for linking.
    SET(BOOST_LIB_DIAGNOSTIC_DEFINITIONS "-DBOOST_LIB_DIAGNOSTIC" "-
DBOOST_ALL_NO_LIB")
ENDIF(WIN32)

FIND_PATH(BOOST_INCLUDE_DIR
  NAMES
  boost/config.hpp
  PATHS
  ${BOOST_INCLUDE_SEARCH_DIRS}
  PATH_SUFFIXES
  ${BOOST_PATH_SUFFIX}
  )

############################################
#
# Check the existence of the libraries.
#
############################################
# This macro was taken directly from the FindQt4.cmake file that is included
# with the CMake distribution. This is NOT my work. All work was done by the
# original authors of the FindQt4.cmake file. Only minor modifications were
# made to remove references to Qt and make this file more generally applicable
########################################################################
#

MACRO (_BOOST_ADJUST_LIB_VARS basename)

  IF (BOOST_INCLUDE_DIR )
    #MESSAGE(STATUS "Adjusting ${basename} ")
    # if only the release version was found, set the debug variable also to the release version
    IF (BOOST_${basename}_LIBRARY_RELEASE AND NOT BOOST_${basename}_LIBRARY_DEBUG)
      SET(BOOST_${basename}_LIBRARY_DEBUG ${BOOST_${basename}_LIBRARY_RELEASE})
      SET(BOOST_${basename}_LIBRARY       ${BOOST_${basename}_LIBRARY_RELEASE})
      SET(BOOST_${basename}_LIBRARIES     ${BOOST_${basename}_LIBRARY_RELEASE})
    ENDIF (BOOST_${basename}_LIBRARY_RELEASE AND NOT BOOST_${basename}_LIBRARY_DEBUG)

    # if only the debug version was found, set the release variable also to the debug version
    IF (BOOST_${basename}_LIBRARY_DEBUG AND NOT BOOST_${basename}_LIBRARY_RELEASE)
      SET(BOOST_${basename}_LIBRARY_RELEASE ${BOOST_${basename}_LIBRARY_DEBUG})
      SET(BOOST_${basename}_LIBRARY         ${BOOST_${basename}_LIBRARY_DEBUG})
      SET(BOOST_${basename}_LIBRARIES       ${BOOST_${basename}_LIBRARY_DEBUG})
    ENDIF (BOOST_${basename}_LIBRARY_DEBUG AND NOT BOOST_${basename}_LIBRARY_RELEASE)
    IF (BOOST_${basename}_LIBRARY_DEBUG AND BOOST_${basename}_LIBRARY_RELEASE)
      # if the generator supports configuration types then set
      # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
      IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(BOOST_${basename}_LIBRARY       optimized ${BOOST_${basename}_LIBRARY_RELEASE} debug ${BOOST_${basename}_LIBRARY_DEBUG})
      ELSE(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        # if there are no configuration types and CMAKE_BUILD_TYPE has no value
        # then just use the release libraries
        SET(BOOST_${basename}_LIBRARY       ${BOOST_${basename}_LIBRARY_RELEASE} )
      ENDIF(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      SET(BOOST_${basename}_LIBRARIES       optimized ${BOOST_${basename}_LIBRARY_RELEASE} debug ${BOOST_${basename}_LIBRARY_DEBUG})
    ENDIF (BOOST_${basename}_LIBRARY_DEBUG AND BOOST_${basename}_LIBRARY_RELEASE)

    SET(BOOST_${basename}_LIBRARY ${BOOST_${basename}_LIBRARY} CACHE FILEPATH "The Boost ${basename} library")

    IF (BOOST_${basename}_LIBRARY)
      SET(BOOST_${basename}_FOUND 1)
      IF (BOOST_USE_${basename})
        SET (BOOST_LIBRARIES ${BOOST_LIBRARIES} ${BOOST_${basename}_LIBRARY} )
        # MESSAGE(STATUS "BOOST_LIBRARIES: ${BOOST_LIBRARIES}")
      ENDIF(BOOST_USE_${basename})
    ENDIF (BOOST_${basename}_LIBRARY)

  ENDIF (BOOST_INCLUDE_DIR )

  # Make variables changeble to the advanced user
  MARK_AS_ADVANCED(BOOST_${basename}_LIBRARY
    BOOST_${basename}_LIBRARY_RELEASE
    BOOST_${basename}_LIBRARY_DEBUG
    BOOST_INCLUDE_DIR
    )
ENDMACRO (_BOOST_ADJUST_LIB_VARS)

#-------------------------------------------------------------------------------
SET (BOOST_LIB_PREFIX "")
IF ( WIN32 )
  SET (BOOST_LIB_PREFIX "lib")
ENDIF ( WIN32 )
SET (BOOST_COMPILER "-gcc")
IF (MSVC71)
  SET (BOOST_COMPILER "-vc71")
ENDIF(MSVC71)
IF (MSVC80)
  SET (BOOST_COMPILER "-vc80")
ENDIF(MSVC80)
IF (MINGW)
  SET (BOOST_COMPILER "-mgw")
ENDIF(MINGW)
IF (CYGWIN)
  SET (BOOST_COMPILER "-gcc")
ENDIF (CYGWIN)
IF (UNIX)

  IF (APPLE)
    SET (BOOST_COMPILER "")
  ELSE (APPLE)
    IF (NOT CMAKE_COMPILER_IS_GNUCC)
      # This is for the intel compiler
      SET (BOOST_COMPILER "-il")
    ELSE (NOT CMAKE_COMPILER_IS_GNUCC)
      SET (BOOST_COMPILER "-gcc34")
    ENDIF (NOT CMAKE_COMPILER_IS_GNUCC)
  ENDIF (APPLE)
ENDIF(UNIX)

SET (BOOST_MULTITHREADED "-mt")

IF (WIN32)
  SET (BOOST_ABI_TAG "g")
ENDIF(WIN32)
SET (BOOST_ABI_TAG "-${BOOST_ABI_TAG}d")

SET (BOOST_EXTENSION "a")
IF (WIN32)
  SET (BOOST_EXTENSION "lib")
ENDIF (WIN32)

#MESSAGE (STATUS "BOOST_LIBRARIES_SEARCH_DIRS: ${BOOST_LIBRARIES_SEARCH_DIRS}")

# ------------------------------------------------------------------------
#  Begin finding boost libraries
# ------------------------------------------------------------------------
# -------- Find the date_time Library -------------
SET (BOOST_LIB date_time)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
# -- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_DATE_TIME_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_DATE_TIME_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_DATE_TIME_LIBRARY_DEBUG} )
ENDIF (BOOST_DATE_TIME_LIBRARY_DEBUG)
# -- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_DATE_TIME_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_DATE_TIME_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_DATE_TIME_LIBRARY_RELEASE} )
ENDIF (BOOST_DATE_TIME_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(DATE_TIME)

# -------- Find the filesystem Library -------------
SET (BOOST_LIB filesystem)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})

#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_FILESYSTEM_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )

IF (BOOST_FILESYSTEM_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_FILESYSTEM_LIBRARY_DEBUG} )
ENDIF (BOOST_FILESYSTEM_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_FILESYSTEM_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_FILESYSTEM_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_FILESYSTEM_LIBRARY_RELEASE} )
ENDIF (BOOST_FILESYSTEM_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(FILESYSTEM)

# -------- Find the iostreams Library -------------
SET (BOOST_LIB iostreams)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_IOSTREAMS_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_IOSTREAMS_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_IOSTREAMS_LIBRARY_DEBUG} )
ENDIF (BOOST_IOSTREAMS_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_IOSTREAMS_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_IOSTREAMS_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_IOSTREAMS_LIBRARY_RELEASE} )
ENDIF (BOOST_IOSTREAMS_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(IOSTREAMS)

# -------- Find the prg_exec_monitor Library -------------
SET (BOOST_LIB prg_exec_monitor)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_PRO_EXEC_MONITOR_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PRO_EXEC_MONITOR_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PRO_EXEC_MONITOR_LIBRARY_DEBUG} )
ENDIF (BOOST_PRO_EXEC_MONITOR_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_PRO_EXEC_MONITOR_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PRO_EXEC_MONITOR_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PRO_EXEC_MONITOR_LIBRARY_RELEASE} )
ENDIF (BOOST_PRO_EXEC_MONITOR_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(PRO_EXEC_MONITOR)

# -------- Find the program_options Library -------------
SET (BOOST_LIB program_options)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_PROGRAM_OPTIONS_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PROGRAM_OPTIONS_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PROGRAM_OPTIONS_LIBRARY_DEBUG} )
ENDIF (BOOST_PROGRAM_OPTIONS_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_PROGRAM_OPTIONS_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PROGRAM_OPTIONS_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PROGRAM_OPTIONS_LIBRARY_RELEASE} )
ENDIF (BOOST_PROGRAM_OPTIONS_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(PROGRAM_OPTIONS)

# -------- Find the python Library -------------
SET (BOOST_LIB python)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_PYTHON_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PYTHON_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PYTHON_LIBRARY_DEBUG} )
ENDIF (BOOST_PYTHON_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_PYTHON_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_PYTHON_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_PYTHON_LIBRARY_RELEASE} )
ENDIF (BOOST_PYTHON_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(PYTHON)

# -------- Find the regex Library -------------
SET (BOOST_LIB regex)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_REGEX_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_REGEX_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_REGEX_LIBRARY_DEBUG} )
ENDIF (BOOST_REGEX_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_REGEX_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_REGEX_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_REGEX_LIBRARY_RELEASE} )
ENDIF (BOOST_REGEX_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(REGEX)

# -------- Find the serialization Library -------------
SET (BOOST_LIB serialization)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_SERIALIZATION_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_SERIALIZATION_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_SERIALIZATION_LIBRARY_DEBUG} )
ENDIF (BOOST_SERIALIZATION_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_SERIALIZATION_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_SERIALIZATION_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_SERIALIZATION_LIBRARY_RELEASE} )
ENDIF (BOOST_SERIALIZATION_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(SERIALIZATION)

# -------- Find the signals Library -------------
SET (BOOST_LIB signals)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_SIGNALS_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_SIGNALS_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_SIGNALS_LIBRARY_DEBUG} )
ENDIF (BOOST_SIGNALS_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_SIGNALS_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_SIGNALS_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_SIGNALS_LIBRARY_RELEASE} )
ENDIF (BOOST_SIGNALS_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(SIGNALS)

# -------- Find the test_exec_monitor Library -------------
SET (BOOST_LIB test_exec_monitor)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_TEST_EXEC_MONITOR_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_TEST_EXEC_MONITOR_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_TEST_EXEC_MONITOR_LIBRARY_DEBUG} )
ENDIF (BOOST_TEST_EXEC_MONITOR_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_TEST_EXEC_MONITOR_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_TEST_EXEC_MONITOR_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_TEST_EXEC_MONITOR_LIBRARY_RELEASE} )
ENDIF (BOOST_TEST_EXEC_MONITOR_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(TEST_EXEC_MONITOR)

# -------- Find the thread Library -------------
SET (BOOST_LIB thread)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_THREAD_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_THREAD_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_THREAD_LIBRARY_DEBUG} )
ENDIF (BOOST_THREAD_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_THREAD_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_THREAD_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_THREAD_LIBRARY_RELEASE} )
ENDIF (BOOST_THREAD_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(THREAD)

# -------- Find the unit_test_framework Library -------------
SET (BOOST_LIB unit_test_framework)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} $
    {BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} )
ENDIF (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} )
ENDIF (BOOST_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(UNIT_TEST_FRAMEWORK)

# -------- Find the wserialization Library -------------
SET (BOOST_LIB wserialization)
SET (BOOST_DEBUG_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}${BOOST_MULTITHREADED}${BOOST_ABI_TAG}-${BOOST_CURRENT_VERSION})
SET (BOOST_RELEASE_LIB_NAME boost_${BOOST_LIB}${BOOST_COMPILER}$
  {BOOST_MULTITHREADED}-${BOOST_CURRENT_VERSION})
#-- Find a Debug Library ---------------------------------------------
FIND_LIBRARY(BOOST_WSERIALIZATION_LIBRARY_DEBUG
  NAMES ${BOOST_LIB_PREFIX}${BOOST_DEBUG_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_WSERIALIZATION_LIBRARY_DEBUG)
  SET(BOOST_DEBUG_LIBRARIES ${BOOST_DEBUG_LIBRARIES} ${BOOST_WSERIALIZATION_LIBRARY_DEBUG} )
ENDIF (BOOST_WSERIALIZATION_LIBRARY_DEBUG)
#-- Find a Release Library -------------------------------------------
FIND_LIBRARY(BOOST_WSERIALIZATION_LIBRARY_RELEASE
  NAMES ${BOOST_LIB_PREFIX}${BOOST_RELEASE_LIB_NAME}
  PATHS ${BOOST_LIBRARIES_SEARCH_DIRS}
  NO_DEFAULT_PATH
  )
IF (BOOST_WSERIALIZATION_LIBRARY_RELEASE)
  SET(BOOST_RELEASE_LIBRARIES ${BOOST_DEBUG_LIBRARIES} ${BOOST_WSERIALIZATION_LIBRARY_RELEASE} )
ENDIF (BOOST_WSERIALIZATION_LIBRARY_RELEASE)
# -- Adjust all the library variables --------------------------------
_BOOST_ADJUST_LIB_VARS(WSERIALIZATION)

# ------------------------------------------------------------------------
#  End finding boost libraries
# ------------------------------------------------------------------------

SET(BOOST_INCLUDE_DIRS
  ${BOOST_INCLUDE_DIR}
  )

# MESSAGE(STATUS "BOOST_INCLUDE_DIR: ${BOOST_INCLUDE_DIR}")
# MESSAGE(STATUS "BOOST_LIBRARIES: ${BOOST_LIBRARIES}")

IF (BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)
  SET(BOOST_FOUND TRUE)
ENDIF(BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES)

IF (BOOST_FOUND)
  IF (NOT Boost_FIND_QUIETLY)
    MESSAGE(STATUS "Found The Following Boost Libraries:")
    FOREACH (TMP_LIB  ${BOOST_LIBRARIES} )
      MESSAGE (STATUS "  ${TMP_LIB}")
    ENDFOREACH(TMP_LIB)
  ENDIF(NOT Boost_FIND_QUIETLY)
ELSE (BOOST_FOUND)
  IF (Boost_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Please install the Boost libraries AND
development packages")
ENDIF(Boost_FIND_REQUIRED)
ENDIF(BOOST_FOUND)

FOREACH (BOOST_LIBDIR ${BOOST_LIBRARIES})
  GET_FILENAME_COMPONENT(BOOST_LIBRARY_DIRS ${BOOST_LIBDIR} PATH)
ENDFOREACH (BOOST_LIBDIR ${BOOST_LIBRARIES})

# Under Windows, automatic linking is performed, so no need to specify the libraries.
IF (WIN32)
  SET(BOOST_LIBRARIES "")
ENDIF(WIN32)

# show the BOOST_INCLUDE_DIRS AND BOOST_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED(BOOST_INCLUDE_DIRS
  BOOST_LIBRARIES
  BOOST_LIBRARY_DIRS
  BOOST_DEFINITIONS
  BOOST_LIBRARIES_SUFFIX
  )

ENDIF(BOOST_LIBRARIES AND BOOST_INCLUDE_DIRS)
