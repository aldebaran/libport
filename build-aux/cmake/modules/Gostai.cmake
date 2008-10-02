# A collection of macro used to create Gostai executable and libraries

if(NOT COMMAND gostai_add_executable)

include(ParseArguments)
include(InstallQtPlugins)
include(Dirs)
include(DynLibDep)
include(GenLibLoader)

# Add a Gostai executable target named _name_
# Arguments:
#  SOURCES	- the list of sources of the executable
#  MOC		- the list of sources of the executable that need to be moc'ed
#  UI		- the list of UI files used by the executable
#  CPPFLAGS	- the list of CPPFLAGS to add
#  LIBRARIES	- the list of libraries to link with.
#  RESOURCES	- the list of resources files.
macro(gostai_add_executable name)

  parse_arguments(
    ${name}
    "SOURCES;MOCS;UIS;CPPFLAGS;INCLUDE_DIRS;LIBRARIES;RESOURCES"
    ""
    ${ARGN})

  # =================== #
  # Include directories #
  # =================== #
  if(${name}_INCLUDE_DIRS)
    include_directories(${${name}_INCLUDE_DIRS})
  endif(${name}_INCLUDE_DIRS)

  # =================== #
  # Prepare the sources #
  # =================== #
  qt4_wrap_ui(${name}_UI_SOURCES ${${name}_UIS})
  qt4_add_resources(${name}_RES_SOURCES ${${name}_RESOURCES})
  qt4_wrap_cpp(${name}_MOC_SOURCES ${${name}_MOCS})

  set(${name}_ALL_SOURCES
    ${${name}_SOURCES}
    ${${name}_MOC_SOURCES}
    ${${name}_RES_SOURCES}
    ${${name}_UI_SOURCES}
    )

  if(NOT ${name}_ALL_SOURCES)
    message(FATAL_ERROR "No sources provided for target executable '${name}'")
  endif(NOT ${name}_ALL_SOURCES)

  # Add the icon if necessary
  if(WIN32)
    set(${name}_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${name}.ico)
    if(EXISTS ${${name}_ICON_PATH})
      set(${name}_ICON_RC_PATH ${CMAKE_CURRENT_BINARY_DIR}/${name}.rc)
      file(WRITE
	${${name}_ICON_RC_PATH}
	"IDI_ICON1   ICON  DISCARDABLE  \"${${name}_ICON_PATH}\"\n"
	)
      list(APPEND ${name}_ALL_SOURCES ${${name}_ICON_RC_PATH})
    elseif(EXISTS ${${name}_ICON_PATH})
      message(STATUS
	"warning: cannot find the application icon '${${name}_ICON_PATH}' "
	"for '${name}'")
    endif(EXISTS ${${name}_ICON_PATH})
  endif(WIN32)

  add_executable(${name} WIN32 MACOSX_BUNDLE ${${name}_ALL_SOURCES})

  if(${name}_CPPFLAGS)
    set_target_properties(${name} PROPERTIES
      COMPILE_DEFINITIONS ${${name}_CPPFLAGS})
  endif(${name}_CPPFLAGS)

  # ===================== #
  # Prepare the libraries #
  # ===================== #
  if(WIN32)
    list(APPEND ${name}_LIBRARIES ${QT_QTMAIN_LIBRARY})
  endif(WIN32)

  if(${name}_LIBRARIES)
    target_link_libraries(${name} ${${name}_LIBRARIES})
  endif(${name}_LIBRARIES)

  # =============== #
  # Configure files #
  # =============== #

  configure_file(${CMAKE_MODULE_PATH}/qt.conf.install.in
    ${CMAKE_CURRENT_BINARY_DIR}/qt.conf.install
    ESCAPE_QUOTES
    @ONLY)
  configure_file(
    ${CMAKE_MODULE_PATH}/qt.conf.build.in
    ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
    ESCAPE_QUOTES
    @ONLY)
  configure_file(
    ${CMAKE_MODULE_PATH}/project-info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/project-info.h
    ESCAPE_QUOTES
    @ONLY)
  configure_file(
    ${CMAKE_MODULE_PATH}/gostai-info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/gostai-info.h
    ESCAPE_QUOTES
    @ONLY)
  configure_file(
    ${CMAKE_MODULE_PATH}/qt-info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/qt-info.h
    ESCAPE_QUOTES
    @ONLY)

  # ============= #
  # Install files #
  # ============= #

  install(TARGETS ${name} DESTINATION ${BINARIES_DIR})
  install_qt_plugins(${PLUGINS_DIR})
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt.conf.install
    DESTINATION ${BINARIES_DIR}
    RENAME qt.conf)

  # ========================== #
  # Deploy dependent libraries #
  # ========================== #

  gen_lib_loader(${name})
  dldep_install(${name})

endmacro(gostai_add_executable)

endif(NOT COMMAND gostai_add_executable)
