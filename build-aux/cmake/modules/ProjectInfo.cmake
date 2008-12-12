# - Set project information variables.

include(Tools)
include(Git)

if(NOT PROJECT_INFO_CMAKE)
  set(PROJECT_INFO_CMAKE TRUE)

  option(RELEASE_CANDIDATE
    "Whether we are building a release candidate version."
    OFF)

  set(PROJECT_VENDOR ""
    CACHE STRING
    "The name of the vendor of this project. Use an empty string if it is 'Gostai'")

  # Extract the project information obtained by gathering the git repository.
  # Example of git describe return format:
  #
  # in master or candidates/* or perso/* branch:
  #  <major>.<minor>
  #  <major>.<minor>-RC-<rc>
  #  <major>.<minor>-<vendor>-<vendor_ver>
  #  <major>.<minor>-<vendor>-<vendor_ver>-RC-<rc>
  # in <major>.<minor>.x branch:
  #  <major>.<minor>.<patch>
  #  <major>.<minor>.<patch>-RC-<rc>
  # in <major>.<minor>-<vendor>-<vendor_ver>.x branch:
  #  <major>.<minor>-<vendor>-<vendor_ver>.<patch>
  #  <major>.<minor>-<vendor>-<vendor_ver>.<patch>-RC-<rc>
  #
  # There is no branch on candidate releases.
  #
  # There is always a trailing -<n>-<sha1> added by git-describe(1) since we
  # use --long option.
  function(project_describe_info major minor patch rc vendor vendor_ver n sha1)

    git_current_branch(branch)
    message(STATUS "Project branch: ${branch}")
    if(branch STREQUAL "(no branch)")
      message(SEND_ERROR "project_describe_info: cannot extract information from no branch.")
      return()
    endif(branch STREQUAL "(no branch)")
    git_describe(HEAD desc --match "[0-9]*")

    # Try to remove optional trailing -<n>-<sha1> added by git-describe(1).
    set(desc_rx "^(.*)-([0-9]+)-g([0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f])$")
    if(desc MATCHES ${desc_rx})
      set(tag ${CMAKE_MATCH_1})
      set(${n} ${CMAKE_MATCH_2} PARENT_SCOPE)
      set(${sha1} ${CMAKE_MATCH_3} PARENT_SCOPE)
    else(desc MATCHES ${desc_rx})
      message(SEND_ERROR
	"project_describe_info: cannot parse git-describe(1) output: '${desc}'")
      return()
    endif(desc MATCHES ${desc_rx})

    # Try to remove optional release candidate number.
    set(rc_rx "^(.*)-RC-([0-9]+)$")
    if(tag MATCHES ${rc_rx})
      set(tag ${CMAKE_MATCH_1})
      set(${rc} ${CMAKE_MATCH_2} PARENT_SCOPE)
    else(tag MATCHES ${rc_rx})
      set(${rc} 0 PARENT_SCOPE)
    endif(tag MATCHES ${rc_rx})

    set(ver_base_rx "([0-9]+)\\.([0-9]+)")
    set(vendor_ver_base_rx "${ver_base_rx}-([^-]*)-([0-9]+)")

    set(patch -1 PARENT_SCOPE)

    # Are we are on master branch?
    if(branch MATCHES "^master$"
	OR branch MATCHES "^candidates/.*$"
	OR branch MATCHES "^perso/.*$")
      message(STATUS "Project on the master branch.")
      set(ver1_rx "^${ver_base_rx}$")
      set(ver2_rx "^${ver_base_rx}-(.*)-([0-9]+)$")
      if(tag MATCHES ${ver1_rx})
	set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	if(PROJECT_VENDOR)
	  git_describe(HEAD vendor_tag --match "[0-9]*-${PROJECT_VENDOR}-*")
	  if(vendor_tag MATCHES "^${vendor_ver_base_rx}")
	    set(${vendor_ver} ${CMAKE_MATCH_4} PARENT_SCOPE)
	  else(vendor_tag MATCHES "^${vendor_ver_base_rx}")
	    set(${vendor_ver} 0 PARENT_SCOPE)
	  endif(vendor_tag MATCHES "^${vendor_ver_base_rx}")
	  set(${vendor} ${PROJECT_VENDOR} PARENT_SCOPE)
	else(PROJECT_VENDOR)
	  set(${vendor} "" PARENT_SCOPE)
	  set(${vendor_ver} 0 PARENT_SCOPE)
	endif(PROJECT_VENDOR)
      elseif(tag MATCHES ${ver2_rx})
	set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	set(${vendor} ${CMAKE_MATCH_3} PARENT_SCOPE)
	set(${vendor_ver} ${CMAKE_MATCH_4} PARENT_SCOPE)
      else(tag MATCHES ${ver1_rx})
	message(SEND_ERROR
	  "project_describe_info: cannot parse tag '${tag}' on branch '${branch}'.")
	return()
      endif(tag MATCHES ${ver1_rx})

    endif()

    # Are we are on a Gostai bug fix branch?
    if(branch MATCHES "^${ver_base_rx}\\.x$")

      message(STATUS "Project on a Gostai bug fix branch.")
      if(PROJECT_VENDOR)
	message(SEND_ERROR
	  "project_describe_info: mismatch project vendor '${PROJECT_VENDOR}' on branch '${branch}'")
	return()
      endif(PROJECT_VENDOR)

      set(ver1_rx "^${ver_base_rx}\\.([0-9]+)$")
      if(tag MATCHES ${ver1_rx})
	set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	set(${patch} ${CMAKE_MATCH_3} PARENT_SCOPE)
      elseif(tag MATCHES "^${ver_base_rx}$")
	if("${tag}.x" STREQUAL "${branch}")
	  set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	  set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	  set(${patch} 0 PARENT_SCOPE)
	else("${tag}.x" STREQUAL "${branch}")
	  message(SEND_ERROR
	    "project_describe_info: mismatch tag '${tag}' on branch '${branch}'")
	  return()
	endif("${tag}.x" STREQUAL "${branch}")
      else(tag MATCHES ${ver1_rx})
	message(SEND_ERROR
	  "project_describe_info: cannot parse tag '${tag}' on branch '${branch}'.")
	return()
      endif(tag MATCHES ${ver1_rx})

    endif(branch MATCHES "^${ver_base_rx}\\.x$")

    # Are we on a vendor bug fix branch?
    if(branch MATCHES "^${vendor_ver_base_rx}\\.x$")

      message(STATUS "Project on a ${PROJECT_VENDOR} bug fix branch.")
      set(br_vendor ${CMAKE_MATCH_3})
      if(PROJECT_VENDOR)
	if(NOT ${br_vendor} STREQUAL ${PROJECT_VENDOR})
	  message(SEND_ERROR
	    "project_describe_info: mismatch project vendor '${PROJECT_VENDOR}' on branch '${branch}'")
	  return()
	endif(NOT ${br_vendor} STREQUAL ${PROJECT_VENDOR})
      else(PROJECT_VENDOR)
	message(SEND_ERROR
	  "project_describe_info: no project vendor specified on branch '${branch}'")
	return()
      endif(PROJECT_VENDOR)

      set(ver1_rx "^${vendor_ver_base_rx}\\.([0-9]+)$")
      if(tag MATCHES ${ver1_rx})
	set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	set(${vendor} ${CMAKE_MATCH_3} PARENT_SCOPE)
	set(${vendor_ver} ${CMAKE_MATCH_4} PARENT_SCOPE)
	set(${patch} ${CMAKE_MATCH_5} PARENT_SCOPE)
      elseif(tag MATCHES "^${vendor_ver_base_rx}$")
	if("${tag}.x" STREQUAL "${branch}")
	  set(${major} ${CMAKE_MATCH_1} PARENT_SCOPE)
	  set(${minor} ${CMAKE_MATCH_2} PARENT_SCOPE)
	  set(${vendor} ${CMAKE_MATCH_3} PARENT_SCOPE)
	  set(${vendor_ver} ${CMAKE_MATCH_4} PARENT_SCOPE)
	  set(${patch} 0 PARENT_SCOPE)
	else("${tag}.x" STREQUAL "${branch}")
	  message(SEND_ERROR
	    "project_describe_info: mismatch tag '${tag}' on branch '${branch}'")
	  return()
	endif("${tag}.x" STREQUAL "${branch}")
      else(tag MATCHES ${ver1_rx})
	message(SEND_ERROR
	  "project_describe_info: cannot parse tag '${tag}' on branch '${branch}'.")
	return()
      endif(tag MATCHES ${ver1_rx})

    endif(branch MATCHES "^${vendor_ver_base_rx}\\.x$")

  endfunction(project_describe_info)

  # Set PROJECT_XXXX variables according to project_describe_info results.
  function(project_set_info)
    project_describe_info(major minor patch rc vendor vendor_ver n sha1)
    echo("Previous release info: major=${major} minor=${minor} patch=${patch} rc=${rc} vendor=${vendor} vendor_ver=${vendor_ver} n=${n} sha1=${sha1}")
    # Minor
    if(patch EQUAL -1) # Not on a bug fix branch
      if(PROJECT_VERSION_MAJOR GREATER ${major})
	set(minor 0)
      elseif(PROJECT_VERSION_MAJOR EQUAL ${major})
	if(NOT n EQUAL 0)
	  if(NOT RELEASE_CANDIDATE AND rc EQUAL 0)
 	    math(EXPR minor "${minor} + 1")
	  endif(NOT RELEASE_CANDIDATE AND rc EQUAL 0)
	endif(NOT n EQUAL 0)
      else(PROJECT_VERSION_MAJOR GREATER ${major})
	message(SEND_ERROR
	  "project_set_info: required project major version cannot be less than current major version.")
	return()
      endif(PROJECT_VERSION_MAJOR GREATER ${major})
    else(patch EQUAL -1)
      if(NOT PROJECT_VERSION_MAJOR EQUAL ${major})
	message(FATAL_ERROR
	  "Cannot change major version number on a bug fix branch.")
      endif(NOT PROJECT_VERSION_MAJOR EQUAL ${major})
    endif(patch EQUAL -1)
    set(PROJECT_VERSION_MINOR ${minor} PARENT_SCOPE)
    # Patch
    if(patch GREATER -1)
      if(NOT n EQUAL 0)
	math(EXPR patch "${patch} + 1")
      endif(NOT n EQUAL 0)
    endif(patch GREATER -1)
    set(PROJECT_VERSION_PATCH ${patch} PARENT_SCOPE)
    # Vendor version
    if(vendor)
      if(NOT n EQUAL 0)
	if(NOT RELEASE_CANDIDATE AND rc EQUAL 0)
	  math(EXPR vendor_ver "${vendor_ver} + 1")
	endif(NOT RELEASE_CANDIDATE AND rc EQUAL 0)
      endif(NOT n EQUAL 0)
    elseif(vendor)
      set(vendor_ver -1)
    endif(vendor)
    set(PROJECT_VENDOR_VERSION ${vendor_ver} PARENT_SCOPE)
    # Release candidate
    if(RELEASE_CANDIDATE)
      if(NOT n EQUAL 0)
	math(EXPR rc "${rc} + 1")
      endif(NOT n EQUAL 0)
    endif(RELEASE_CANDIDATE)
    set(PROJECT_VERSION_RC ${rc} PARENT_SCOPE)
    # Additional commit
    set(PROJECT_ADDITIONAL_COMMIT ${n} PARENT_SCOPE)
    # Sha1
    set(PROJECT_REVISION ${sha1} PARENT_SCOPE)
    echo("Next release info: major=${major} minor=${minor} patch=${patch} rc=${rc} vendor=${vendor} vendor_ver=${vendor_ver} n=${n} sha1=${sha1}")
  endfunction(project_set_info)

  check_cmake_vars_exists(
    PROJECT_VERSION_MAJOR
    PROJECT_LONGNAME
    )

  # Set version information
  if(DEFINED PROJECT_VERSION_MINOR)
    set_if_not_set(PROJECT_VERSION_PATCH 0)
    set_if_not_set(PROJECT_VERSION_RC 0)
    set_if_not_set(PROJECT_VENDOR "")
    set_if_not_set(PROJECT_VENDOR_VERSION 0)
    set_if_not_set(PROJECT_ADDITIONAL_COMMIT 0)
  else(DEFINED PROJECT_VERSION_MINOR)
    project_set_info()
  endif(DEFINED PROJECT_VERSION_MINOR)
  message(STATUS "Project major version: ${PROJECT_VERSION_MAJOR}")
  message(STATUS "Project minor version: ${PROJECT_VERSION_MINOR}")
  if(RELEASE_CANDIDATE)
    message(STATUS "Project release candidate number: ${PROJECT_VERSION_RC}")
  endif(RELEASE_CANDIDATE)
  if(PROJECT_VERSION_PATCH GREATER 0)
    message(STATUS "Project patch version: ${PROJECT_VERSION_PATCH}")
  endif(PROJECT_VERSION_PATCH GREATER 0)
  if(PROJECT_VENDOR)
    message(STATUS "Project vendor: ${PROJECT_VENDOR}")
    message(STATUS "Project vendor version: ${PROJECT_VENDOR_VERSION}")
  endif(PROJECT_VENDOR)
  if(PROJECT_ADDITIONAL_COMMIT GREATER 0)
    message(STATUS "Project additional commit: ${PROJECT_ADDITIONAL_COMMIT}")
  endif(PROJECT_ADDITIONAL_COMMIT GREATER 0)

  # Copyright information
  if(PROJECT_VENDOR)
    check_cmake_vars_exists(PROJECT_COPYRIGHT_HOLDER PROJECT_COPYRIGHT_YEARS)
  else(PROJECT_VENDOR)
    set(PROJECT_COPYRIGHT_HOLDER "Gostai S.A.S")
    set(PROJECT_COPYRIGHT_YEARS "2006-2008")
  endif(PROJECT_VENDOR)
  set(PROJECT_COPYRIGHT_STRING
    "(C) ${PROJECT_COPYRIGHT_YEARS} ${PROJECT_COPYRIGHT_HOLDER}")

  # Project repository information.
  include(Revision)
  revision(PROJECT_DATE PROJECT_ID PROJECT_REVISION)
  message(STATUS "Project repository date: ${PROJECT_DATE}")
  message(STATUS "Project repository ID: ${PROJECT_ID}")
  message(STATUS "Project repository revision: ${PROJECT_REVISION}")

  # Compute version string.
  set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
  if(PROJECT_VENDOR)
    set(PROJECT_VERSION
      "${PROJECT_VERSION}-${PROJECT_VENDOR}-${PROJECT_VENDOR_VERSION}")
  endif(PROJECT_VENDOR)
  if(PROJECT_VERSION_PATCH GREATER 0)
    set(PROJECT_VERSION "${PROJECT_VERSION}.${PROJECT_VERSION_PATCH}")
  endif(PROJECT_VERSION_PATCH GREATER 0)
  if(RELEASE_CANDIDATE)
    set(PROJECT_VERSION "${PROJECT_VERSION}-RC-${PROJECT_VERSION_RC}")
  endif(RELEASE_CANDIDATE)
  if(PROJECT_ADDITIONAL_COMMIT GREATER 0)
    set(PROJECT_VERSION
      "${PROJECT_VERSION}-${PROJECT_ADDITIONAL_COMMIT}-${PROJECT_REVISION}")
  endif(PROJECT_ADDITIONAL_COMMIT GREATER 0)
  message(STATUS "Project version: ${PROJECT_VERSION}")

  set(PROJECT_VERSION_REV "version ${PROJECT_VERSION} rev. ${PROJECT_REVISION}")
  set(PROJECT_STRING "${PROJECT_LONGNAME} ${PROJECT_VERSION}")
  if(PROJECT_VENDOR STREQUAL "")
    set(vendor_prefix "${GOSTAI}")
  else(PROJECT_VENDOR STREQUAL "")
    set(vendor_prefix "${PROJECT_VENDOR}")
  endif(PROJECT_VENDOR STREQUAL "")
  # Do not prefix by vendor if it is already in the project's name.
  if(${PROJECT_NAME} MATCHES ${vendor_prefix})
    set(vendor_prefix)
  else(${PROJECT_NAME} MATCHES ${vendor_prefix})
    set(vendor_prefix "${vendor_prefix}-")
  endif(${PROJECT_NAME} MATCHES ${vendor_prefix})
  set(PROJECT_TARNAME "${vendor_prefix}${PROJECT_NAME}-${PROJECT_VERSION}")

  # ========================== #
  # Add the tag release target #
  # ========================== #

  # Format the next tag name.
  # We remove the trailing additional commit number and sha1
  string(REGEX REPLACE
    "-([0-9]+)-([0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]+)$"
    ""
    PROJECT_TAG_NAME
    "${PROJECT_VERSION}")

  # Format the next tag message.
  set(PROJECT_TAG_MSG "Release ${PROJECT_NAME}")
  if(PROJECT_VENDOR)
    set(PROJECT_TAG_MSG
      "${PROJECT_TAG_MSG} ${PROJECT_VENDOR_VERSION} for ${PROJECT_VENDOR}")
  else(PROJECT_VENDOR)
    set(PROJECT_TAG_MSG "${PROJECT_TAG_MSG} for Gostai")
  endif(PROJECT_VENDOR)
  set(PROJECT_TAG_MSG
    "${PROJECT_TAG_MSG} version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
  if(RELEASE_CANDIDATE)
    set(PROJECT_TAG_MSG "${PROJECT_TAG_MSG} candidate ${PROJECT_VERSION_RC}")
  endif(RELEASE_CANDIDATE)
  set(PROJECT_TAG_MSG "${PROJECT_TAG_MSG}.")

  add_custom_target(tag-release
    COMMAND
    ${GIT_EXECUTABLE} tag -a -m "${PROJECT_TAG_MSG}" "${PROJECT_TAG_NAME}" HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Tagging release ${PROJECT_TAG_NAME}")

endif(NOT PROJECT_INFO_CMAKE)
