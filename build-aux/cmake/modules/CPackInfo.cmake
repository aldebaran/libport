##
## CPackInfo.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# This module setup the variables used by cpack. It uses many variables already
# set for package information. Here the list:
#
#  PROJECT_LONGNAME
#  PROJECT_VERSION_MAJOR
#  PROJECT_VERSION_MINOR
#  PROJECT_VERSION
#  PROJECT_COPYRIGHT_HOLDER
#  PROJECT_NAME
#  PROJECT_HOMEPAGE
#  PROJECT_SUPPORT
#  PROJECT_HELP_LINK
#
# You can overwrite all the variables set by this module after including it.
# Then, include the CPack module.

if(NOT DEFINED CPACK_INFO_CMAKE)
set(CPACK_INFO_CMAKE TRUE)

  #FIXME: Some variables are not set yet. Fix it.

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_STRING}")
set(CPACK_PACKAGE_VENDOR ${PROJECT_COPYRIGHT_HOLDER})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
if(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${GOSTAI}\\\\${PROJECT_NAME} ${PROJECT_VERSION}")
else(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${PROJECT_VERSION}")
endif(WIN32)
if(WIN32 AND NOT UNIX)
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  set(CPACK_NSIS_HELP_LINK ${PROJECT_HELP_LINK})
  set(CPACK_NSIS_URL_INFO_ABOUT ${PROJECT_HOMEPAGE})
  set(CPACK_NSIS_CONTACT ${PROJECT_SUPPORT})
  set(CPACK_NSIS_MODIFY_PATH OFF)
else(WIN32 AND NOT UNIX)
  # Compute list of programs to strip.
  foreach(i ${INSTALLED_PROGRAMS})
    set(CPACK_STRIP_FILES "${CPACK_STRIP_FILES};bin/${i}")
  endforeach(i)
  #FIXME: Generate wrappers scripts
#  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
# List of pair of executable and label used by NSIS to create start menu items.
set(CPACK_PACKAGE_EXECUTABLES @PROJECT_NAME@ @PROJECT_STRING@)

# Set generators
IF(NOT CPACK_GENERATOR)
  IF(UNIX)
    IF(APPLE)
      SET(CPACK_GENERATOR "PackageMaker")
    ELSE(APPLE)
      SET(CPACK_GENERATOR "STGZ")
    ENDIF(APPLE)
  ELSE(UNIX)
    SET(CPACK_GENERATOR "NSIS")
  ENDIF(UNIX)
ENDIF(NOT CPACK_GENERATOR)

endif(NOT DEFINED CPACK_INFO_CMAKE)
