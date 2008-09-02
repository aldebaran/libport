##
## ProjectInfo.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

include(CheckCMakeVarsExists)

#FIXME: Remove guards ASAP
# if(NOT PROJECT_INFO_CMAKE)
#   set(PROJECT_INFO_CMAKE TRUE)

  check_cmake_vars_exists(
    PROJECT_VERSION_MAJOR
    PROJECT_VERSION_MINOR
    PROJECT_VERSION_PATCH
    PROJECT_LONGNAME
    )

  set(PROJECT_COPYRIGHT_HOLDER "Gostai S.A.S")
  set(PROJECT_COPYRIGHT_YEARS "2006-2008")
  set(PROJECT_COPYRIGHT_STRING
    "Copyright (C) ${PROJECT_COPYRIGHT_YEARS} ${PROJECT_COPYRIGHT_HOLDER}")

  include(Revision)
  revision(PROJECT_DATE PROJECT_ID PROJECT_REVISION)

  message(STATUS "Project repository date: ${PROJECT_DATE}")
  message(STATUS "Project repository ID: ${PROJECT_ID}")
  message(STATUS "Project repository revision: ${PROJECT_REVISION}")

  set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
  set(PROJECT_VERSION_REV "version ${PROJECT_VERSION} rev. ${PROJECT_REVISION}")

  set(PROJECT_STRING "${PROJECT_LONGNAME} ${PROJECT_VERSION}")

  set(PROJECT_TARNAME "${PROJECT_NAME}-${PROJECT_VERSION}")

# endif(NOT PROJECT_INFO_CMAKE)
