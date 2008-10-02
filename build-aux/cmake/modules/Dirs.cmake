# Defined directory paths of the install skeleton depending on the platform.
# Paths are relative to the prefix path which is CMAKE_INSTALL_PREFIX

if(NOT DEFINED DIRS_CMAKE)
  set(DIRS_CMAKE TRUE)

  set(PREFIX_DIR ${CMAKE_INSTALL_PREFIX})
  set(HEADERS_DIR include)
  set(BINARIES_DIR bin)
  if(WIN32)
    if(NOT UNIX)
      # Pure Win32
      set(DOCUMENTATION_DIR doc)
      set(PLUGINS_DIR plugins)
      set(DATA_DIR data)
      set(TRANSLATIONS_DIR translations)
      set(SETTINGS_DIR etc)
      set(EXAMPLES_DIR examples)
      set(DEMOS_DIR demos)
      set(LIBRARIES_DIR ${BINARIES_DIR})
      set(EXTRA_LIBRARIES_DIR ${BINARIES_DIR})
    endif(NOT UNIX)
  else(WIN32)
    if(APPLE)
      # Apple, MacOSX
      #FIXME: Implement me!
    else(APPLE)
      # Unix, cygwin, mingw
      include(CheckCMakeVarsExists)
      check_cmake_vars_exists(PROJECT_NAME PROJECT_VERSION)
      set(_share_prefix share/${PROJECT_NAME}/${PROJECT_VERSION})

      set(LIBRARIES_DIR lib)
      set(DOCUMENTATION_DIR ${_share_prefix}/doc)
      set(PLUGINS_DIR ${_share_prefix}/plugins)
      set(DATA_DIR ${_share_prefix}/data)
      set(TRANSLATIONS_DIR ${_share_prefix}/translations)
      set(SETTINGS_DIR ${_share_prefix}/etc)
      set(EXAMPLES_DIR ${_share_prefix}/examples)
      set(DEMOS_DIR ${_share_prefix}/demos)
      set(EXTRA_LIBRARIES_DIR ${_share_prefix}/lib)
    endif(APPLE)
  endif(WIN32)

endif(NOT DEFINED DIRS_CMAKE)
