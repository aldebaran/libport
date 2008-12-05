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

include(Tools)
include(Gostai)

check_cmake_vars_exists(
  CPACK_RESOURCE_FILE_LICENSE
  CPACK_RESOURCE_FILE_README
  CPACK_RESOURCE_FILE_WELCOME
  CPACK_PACKAGE_DESCRIPTION_SUMMARY
  PROJECT_HELP_LINK
  PROJECT_HOMEPAGE
  PROJECT_BUGREPORT
  PROJECT_NAME
  PROJECT_TARNAME
  )

# ============= #
# All generator #
# ============= #

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
if(PROJECT_VENDOR STREQUAL "")
  set(CPACK_PACKAGE_VENDOR "${GOSTAI}")
else(PROJECT_VENDOR STREQUAL "")
  set(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
endif(PROJECT_VENDOR STREQUAL "")
# Do not prefix by vendor if it is already in the project's name.
if(${CPACK_PACKAGE_NAME} MATCHES ${CPACK_PACKAGE_VENDOR})
  set(CPACK_PACKAGE_VENDOR)
endif(${CPACK_PACKAGE_NAME} MATCHES ${CPACK_PACKAGE_VENDOR})
set(CPACK_PACKAGE_CONTACT "${PROJECT_BUGREPORT}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CPACK_RESOURCE_FILE_LICENSE})
set(CPACK_SOURCE_STRIP_FILES TRUE)
set(CPACK_STRIP_FILES TRUE)
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} ${PROJECT_STRING})
set(CPACK_PACKAGE_REGISTRY_KEY "${PROJECT_NAME}-${PROJECT_VERSION}")

# Set the package filename.
set(pkg_sys_name ${CMAKE_SYSTEM_NAME})
if(${pkg_sys_name} MATCHES Windows)
  if(CMAKE_CL_64)
    set(pkg_sys_name win64)
  else(CMAKE_CL_64)
    set(pkg_sys_name win32)
  endif(CMAKE_CL_64)
endif(${pkg_sys_name} MATCHES Windows)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(build_type "-debug")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(build_type "")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(CPACK_PACKAGE_FILE_NAME
  "${PROJECT_TARNAME}-${pkg_sys_name}${build_type}")

# Installation directory.
set(cpack_package_vendor_separator "")
if(CPACK_PACKAGE_VENDOR)
  if(WIN32)
    set(cpack_package_vendor_separator " ")
  else(WIN32)
    set(cpack_package_vendor_separator "-")
  endif(WIN32)
endif(CPACK_PACKAGE_VENDOR)
if(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY
    "${CPACK_PACKAGE_VENDOR}${cpack_package_vendor_separator}${PROJECT_NAME} ${PROJECT_VERSION}")
else(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY
    "${CPACK_PACKAGE_VENDOR}${cpack_package_vendor_separator}${PROJECT_NAME}-${PROJECT_VERSION}")
endif(WIN32)

# Set generators
if(NOT CPACK_GENERATOR)
  if(UNIX)
    if(APPLE)
      set(CPACK_GENERATOR "PackageMaker")
      set(CPACK_PACKAGE_FILE_NAME_EXTENSION ".pkg")
    else(APPLE)
      set(CPACK_GENERATOR "STGZ")
      set(CPACK_PACKAGE_FILE_NAME_EXTENSION ".sh")
    endif(APPLE)
  else(UNIX)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_FILE_NAME_EXTENSION ".exe")
  endif(UNIX)
endif(NOT CPACK_GENERATOR)

# ============== #
# NSIS generator #
# ============== #

if(WIN32 AND NOT UNIX)
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
  set(CPACK_NSIS_HELP_LINK ${PROJECT_HELP_LINK})
  set(CPACK_NSIS_URL_INFO_ABOUT ${PROJECT_HOMEPAGE})
  set(CPACK_NSIS_CONTACT ${PROJECT_BUGREPORT})
  set(CPACK_NSIS_MODIFY_PATH OFF)

  # Icons and banner.
  set(CPACK_NSIS_MUI_ICON
    "${CMAKE_MODULE_PATH}\\\\cpack\\\\nsis\\\\installer.ico")
  set(CPACK_NSIS_MUI_UNIICON
    "${CMAKE_MODULE_PATH}\\\\cpack\\\\nsis\\\\uninstaller.ico")
  set(CPACK_PACKAGE_ICON
    "${CMAKE_MODULE_PATH}\\\\cpack\\\\nsis\\\\topleft-banner.bmp")

  # Extra shortcut
  set(CPACK_NSIS_CREATE_ICONS_EXTRA "
     CreateShortCut \\\"$DESKTOP\\\\${PROJECT_STRING}.lnk\\\" \\\"$INSTDIR\\\\${BINARIES_DIR}\\\\${PROJECT_NAME}.exe\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\README.lnk\\\" \\\"$INSTDIR\\\\${DOCUMENTATION_DIR}\\\\README.html\\\"
    ")
  set(CPACK_NSIS_DELETE_ICONS_EXTRA "
    Delete \\\"$DESKTOP\\\\${PROJECT_STRING}.lnk\\\"
    Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\README.lnk\\\"
    ")

  # The compressor type
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CPACK_NSIS_COMPRESSOR "zlib")
  else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CPACK_NSIS_COMPRESSOR "lzma")
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

  # Run executable at end
  if(CPACK_RUN_EXENAME)
    list(GET CPACK_RUN_EXENAME 0 CPACK_RUN_EXENAME_TEXT)
    list(GET CPACK_RUN_EXENAME 1 CPACK_RUN_EXENAME_CMD)
    #FIXME: Escape " with $\" in the value
    list(GET CPACK_RUN_EXENAME 2 CPACK_RUN_EXENAME_PARAMS)
    list(GET CPACK_RUN_EXENAME 3 CPACK_RUN_EXENAME_CHECK)
    set(CPACK_NSIS_FINISHPAGE_RUN "
      !define MUI_FINISHPAGE_RUN \\\"$INSTDIR\\\\@CPACK_RUN_EXENAME_CMD@\\\"
      !define MUI_FINISHPAGE_RUN_TEXT \\\"@CPACK_RUN_EXENAME_TEXT@\\\"
      ")
    if(NOT CPACK_RUN_EXENAME_PARAMS STREQUAL "")
      set(CPACK_NSIS_FINISHPAGE_RUN "${CPACK_NSIS_FINISHPAGE_RUN}
        !define MUI_FINISHPAGE_RUN_PARAMETERS @CPACK_RUN_EXENAME_PARAMS@
        ")
    endif(NOT CPACK_RUN_EXENAME_PARAMS STREQUAL "")
    if(NOT CPACK_RUN_EXENAME_CHECK)
      set(CPACK_NSIS_FINISHPAGE_RUN "${CPACK_NSIS_FINISHPAGE_RUN}
        !define MUI_FINISHPAGE_RUN_NOTCHECKED \\\"$INSTDIR\\\\@CPACK_RUN_EXENAME_PARAMS@\\\
        ")
    endif(NOT CPACK_RUN_EXENAME_CHECK)
  endif(CPACK_RUN_EXENAME)

endif(WIN32 AND NOT UNIX)

# ========================= #
# Add upload-package target #
# ========================= #

set(UPLOAD_PACKAGE_URL "'build@gate-bf:release/${CPACK_PACKAGE_NAME}'"
  CACHE STRING "The URL where to upload the generated package.")
add_custom_target(upload-package
  COMMAND
  scp -q
  "${CPACK_PACKAGE_FILE_NAME}${CPACK_PACKAGE_FILE_NAME_EXTENSION}"
  "${UPLOAD_PACKAGE_URL}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Uploading package to ${UPLOAD_PACKAGE_URL}")
configure_file(
  ${CMAKE_MODULE_PATH}/internal-release-mail.txt.in
  ${CMAKE_BINARY_DIR}/internal-release-mail.txt
  @ONLY
  )

endif(NOT DEFINED CPACK_INFO_CMAKE)
