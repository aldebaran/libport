# A collection of macros used to create Gostai executables and libraries.

if(NOT GOSTAI_CMAKE_GUARD)
set(GOSTAI_CMAKE_GUARD TRUE)

include(Tools)
include(InstallQtPlugins)
include(Dirs)
include(DynLibDep)
include(GenLibLoader)
include(GostaiHintDirs)

# Add a Gostai executable target named _name_
# Arguments:
#  SOURCES	- the list of sources of the executable
#  MOCS		- the list of sources of the executable that need to be moc'ed
#  UIS		- the list of UI files used by the executable
#  CPPFLAGS	- the list of CPPFLAGS to add
#  INCLUDE_DIRS - the list of include directories to search in.
#  LIBRARIES	- the list of libraries to link with.
#  RESOURCES	- the list of resources files.
macro(gostai_add_executable name)

  parse_arguments(
    ${name}
    "SOURCES;MOCS;UIS;CPPFLAGS;INCLUDE_DIRS;LIBRARIES;RESOURCES"
    "NO_CONSOLE;INSTALL_NO_DEPS"
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

  set(${name}_MOC_OPTIONS "")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND ${name}_MOC_OPTIONS "-DQT_NO_DEBUG")
    list(APPEND ${name}_MOC_OPTIONS "-DNDEBUG")
  endif(CMAKE_BUILD_TYPE STREQUAL "Release")
  qt4_wrap_cpp(${name}_MOC_SOURCES ${${name}_MOCS}
    OPTIONS ${${name}_MOC_OPTIONS})

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
      #FIXME: Make this macro a function to avoid cluttering the namespace
      # with the icon_path variable.
      if(CMAKE_CROSSCOMPILING)
	set(icon_path "${${name}_ICON_PATH}")
      else(CMAKE_CROSSCOMPILNG)
	string(REPLACE "/" "\\\\" icon_path "${${name}_ICON_PATH}")
      endif(CMAKE_CROSSCOMPILING)
      #FIXME: Touching project resource template do not trigger resource file
      #       to be compiled by windres.
      configure_file(
	${CMAKE_MODULE_PATH}/project.rc.in
	${${name}_ICON_RC_PATH}
	ESCAPE_QUOTES
	@ONLY)
      if(MINGW)
	find_package(WindRes REQUIRED)
	set(${name}_ICON_RC_PATH_OUTPUT "${${name}_ICON_RC_PATH}.o")
	add_custom_command(
	  OUTPUT ${${name}_ICON_RC_PATH_OUTPUT}

          COMMAND
	  ${WINDRES_EXECUTABLE}
	  -I${CMAKE_CURRENT_SOURCE_DIR}
          -i${${name}_ICON_RC_PATH}
          -o ${${name}_ICON_RC_PATH_OUTPUT}

	  DEPENDS
	  ${WINDRES_EXECUTABLE}
	  ${${name}_ICON_RC_PATH}
	  ${${name}_ICON_PATH}

	  COMMENT
	  "Building resource file '${${name}_ICON_RC_PATH_OUTPUT}'")
	list(APPEND ${name}_ALL_SOURCES ${${name}_ICON_RC_PATH_OUTPUT})
      else(MINGW)
	list(APPEND ${name}_ALL_SOURCES ${${name}_ICON_RC_PATH})
      endif(MINGW)
    elseif(EXISTS ${${name}_ICON_PATH})
      message(STATUS
	"warning: cannot find the application icon '${${name}_ICON_PATH}' "
	"for '${name}'")
    endif(EXISTS ${${name}_ICON_PATH})
  endif(WIN32)

  add_executable(${name} MACOSX_BUNDLE ${${name}_ALL_SOURCES})
  if(${name}_NO_CONSOLE)
    set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
  else(${name}_NO_CONSOLE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE FALSE)
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
      list(APPEND ${name}_CPPFLAGS "QT_NO_DEBUG")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  endif(${name}_NO_CONSOLE)

  if(${name}_CPPFLAGS)
    set_target_properties(${name} PROPERTIES
      COMPILE_DEFINITIONS "${${name}_CPPFLAGS}")
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
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf.build.in)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf.build.in
      ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
      ESCAPE_QUOTES
      @ONLY)
  endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf.build.in)
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

  if(NOT ${name}_INSTALL_NO_DEPS)
    gen_lib_loader(${name})
    dldep_install(${name})
  endif(NOT ${name}_INSTALL_NO_DEPS)

  # ================== #
  # Add custom targets #
  # ================== #

  add_exec_target(${name})

endmacro(gostai_add_executable)


# Add a Gostai Qt assistant documentation target named _name_ based on docbook
# output.
# Arguments:
#  SOURCES	- the list of all XML files involved in the docbook.
#  EXTRA	- the list of all extra files (others than generated HTML
#		  files) Qt assistant requires (images for instance).
#  IMPORT_BUILD - the path where to find already built file. This is useful
#                 when your building the package on a platform that do not
#		  support the tool chain needed to build the documentation.
function(gostai_add_qt_assistant name)

  set(assistant_dir ${CMAKE_MODULE_PATH}/assistant)
  set(stylesheets_dir ${assistant_dir}/stylesheets)
  set(css_filename styles.css)
  set(chunk_dir ${name}_assistant)

  # Find tools
  find_package(QtAssistant REQUIRED)

  # Parse arguments.
  set(prefix ${name}_assistant)
  parse_arguments(
    ${prefix}
    "SOURCES;EXTRA;IMPORT_BUILD;MAIN_SOURCE"
    ""
    ${ARGN})
  set(${prefix}_SOURCES ${${prefix}_SOURCES} PARENT_SCOPE)
  set(${prefix}_EXTRA ${${prefix}_EXTRA} PARENT_SCOPE)
  set(compressed_help_name ${name}.qch)

  # We import already built documentation.
  if(${prefix}_IMPORT_BUILD)

    if(EXISTS ${${prefix}_IMPORT_BUILD})

      set(collection_filename ${${prefix}_IMPORT_BUILD}/${name}.qhc)
      set(compressed_help_filename
	${${prefix}_IMPORT_BUILD}/${compressed_help_name})

    else(EXISTS ${${prefix}_IMPORT_BUILD})

      message(SEND_ERROR
	"documentation import directory '${${prefix}_IMPORT_BUILD}' does "
	"not exists")

    endif(EXISTS ${${prefix}_IMPORT_BUILD})

  # otherwise we compile it from source
  else(${prefix}_IMPORT_BUILD)

    # Find tools.
    find_package(XSLTProc REQUIRED)
    find_package(QHelpGenerator REQUIRED)
    find_package(QCollectionGenerator REQUIRED)

    # Search for the main file.
    if(NOT ${prefix}_MAIN_SOURCE)
      set(${prefix}_MAIN_SOURCE ${name}.xml)
    endif(NOT ${prefix}_MAIN_SOURCE)
    set(${prefix}_MAIN_SOURCE ${${prefix}_MAIN_SOURCE} PARENT_SCOPE)
    list(FIND ${prefix}_SOURCES ${${prefix}_MAIN_SOURCE} main_idx)
    if(main_idx STREQUAL -1)
      message(FATAL_ERROR
	"cannot find assistant docbook main file '${${prefix}_MAIN_SOURCE}'")
    endif(main_idx STREQUAL -1)

    # Configure extra files style sheet.
    set(extra_files ${css_filename} index.html)
    list(APPEND extra_files ${${prefix}_EXTRA})
    foreach(extra_file ${extra_files})
      set(extra_files_xml
	"${extra_files_xml}&lt;file&gt;${chunk_dir}/${extra_file}&lt;/file&gt;")
    endforeach(extra_file)
    configure_file(${stylesheets_dir}/extra_files.xsl.in
      ${CMAKE_CURRENT_BINARY_DIR}/extra_files.xsl)

    # Copy css.
    configure_file(${assistant_dir}/${css_filename}
      ${CMAKE_CURRENT_BINARY_DIR}/${chunk_dir}/${css_filename}
      COPYONLY)

    # Copy extra files.
    foreach(extra_file ${${prefix}_EXTRA})
      configure_file(
	${extra_file}
	${CMAKE_CURRENT_BINARY_DIR}/${chunk_dir}/${extra_file}
	COPYONLY)
    endforeach(extra_file)

    # Configure project information.
    configure_xml(
      ${CMAKE_MODULE_PATH}/project-info.docbook.in
      ${CMAKE_CURRENT_BINARY_DIR}/project-info.xml)
    # Configure Gostai information.
    configure_file(
      ${CMAKE_MODULE_PATH}/gostai-info.docbook.in
      ${CMAKE_CURRENT_BINARY_DIR}/gostai-info.xml)

    set(xsltproc_path
      "${CMAKE_CURRENT_BINARY_DIR}")

    # Compile chunk HTML version.
    set(chunk_index ${chunk_dir}/index.html)
    add_custom_command(
      OUTPUT ${chunk_index}
      COMMAND ${XSLTPROC_EXECUTABLE}

      ARGS
      --xinclude
      --output ${CMAKE_CURRENT_BINARY_DIR}/${chunk_dir}/
      --path ${xsltproc_path}
      ${stylesheets_dir}/chunk-stylesheet.xsl
      ${${prefix}_MAIN_SOURCE}

      DEPENDS
      ${${prefix}_SOURCES}
      ${stylesheets_dir}/chunk-stylesheet.xsl

      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )

    # Compile single page HTML version (not really maintained!)
    set(html_filename ${name}.html)
    add_custom_command(
      OUTPUT ${html_filename}
      COMMAND ${XSLTPROC_EXECUTABLE}

      ARGS --xinclude
      --output ${CMAKE_CURRENT_BINARY_DIR}/${html_filename}
      --path ${xsltproc_path}
      ${stylesheets_dir}/html-stylesheet.xsl
      ${${prefix}_MAIN_SOURCE}

      DEPENDS
      ${${prefix}_SOURCES}
      ${stylesheets_dir}/html-stylesheet.xsl

      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )

    # Compile Qt Help project (.qhp)
    set(project_help_filename ${name}.qhp)
    add_custom_command(
      OUTPUT ${project_help_filename}
      COMMAND ${XSLTPROC_EXECUTABLE}

      ARGS
      --xinclude
      --output ${CMAKE_CURRENT_BINARY_DIR}/${project_help_filename}
      --path ${xsltproc_path}
      ${stylesheets_dir}/assistant-stylesheet.xsl
      ${${prefix}_MAIN_SOURCE}

      DEPENDS
      ${${prefix}_SOURCES}
      ${stylesheets_dir}/assistant-stylesheet.xsl
      ${CMAKE_CURRENT_BINARY_DIR}/extra_files.xsl

      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )

    # Compile Qt Compressed help file. (.qch)
    set(compressed_help_filename
      ${CMAKE_CURRENT_BINARY_DIR}/${compressed_help_name})
    add_custom_command(
      OUTPUT ${compressed_help_filename}
      COMMAND ${QHELPGENERATOR_EXECUTABLE}
      ARGS ${project_help_filename} -o ${compressed_help_filename}
      DEPENDS ${project_help_filename} ${QHELPGENERATOR_EXECUTABLE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )

    # Configure Qt Help Collection Project
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(ADDR_BAR_ENABLED "true")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(ADDR_BAR_ENABLED "false")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(REGISTER_SECTION "<file>${compressed_help_name}</file>")
    set(collection_project_filename ${CMAKE_CURRENT_BINARY_DIR}/${name}.qhcp)
    configure_file(${assistant_dir}/collection-help-project.qhcp.in
      ${collection_project_filename}
      ESCAPE_QUOTES
      @ONLY
      )

    # Make Qt Collection file
    configure_file(${assistant_dir}/gostai-doc.png
      ${CMAKE_CURRENT_BINARY_DIR}/gostai-doc.png
      COPYONLY
      )
    set(collection_filename ${CMAKE_CURRENT_BINARY_DIR}/${name}.qhc)
    add_custom_command(
      OUTPUT ${collection_filename}
      COMMAND ${QCOLLECTIONGENERATOR_EXECUTABLE}
      ARGS ${collection_project_filename} -o ${collection_filename}

      DEPENDS
      ${collection_project_filename}
      ${QCOLLECTIONGENERATOR_EXECUTABLE}
      ${compressed_help_filename}

      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )

    # Add associated target
    add_custom_target(${prefix}
      DEPENDS
      ${chunk_index}
      ${html_filename}
      ${project_help_filename}
      ${compressed_help_filename}
      ${collection_filename}
      )
    if(TARGET doc)
      add_dependencies(doc ${prefix})
    endif(TARGET doc)

  endif(${prefix}_IMPORT_BUILD)

  # Schedule for installation.
  install(
    FILES ${collection_filename} ${compressed_help_filename}
    DESTINATION ${DOCUMENTATION_DIR}
    )

  # Generate library loader for Qt assistant.
  gen_lib_loader(${QT_QTASSISTANT_EXECUTABLE})

  # Install Qt assistant.
  install(
    FILES ${QT_QTASSISTANT_EXECUTABLE}

    DESTINATION ${BINARIES_DIR}

    PERMISSIONS
    OWNER_EXECUTE OWNER_READ
    GROUP_EXECUTE GROUP_READ
    WORLD_EXECUTE WORLD_READ
    )

  dldep_install(${QT_QTASSISTANT_EXECUTABLE})

endfunction(gostai_add_qt_assistant name)

function(gostai_add_library name)

  parse_arguments(
    ${name}
    "SOURCES;MOCS;UIS;CPPFLAGS;INCLUDE_DIRS;LIBRARIES;RESOURCES"
    "QTPLUGIN;NO_INSTALL"
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
  if(${name}_QTPLUGIN)
    set(${name}_MOC_OPTS
      -DQT_PLUGIN
      -DQT_SHARED
      )
  else(${name}_QTPLUGIN)
    set(${name}_MOC_OPTS "")
  endif(${name}_QTPLUGIN)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND ${name}_MOC_OPTS -DQT_NO_DEBUG)
  endif(CMAKE_BUILD_TYPE STREQUAL "Release")
  qt4_wrap_ui(${name}_UI_SOURCES ${${name}_UIS})
  qt4_add_resources(${name}_SOURCES ${${name}_RESOURCES})
  qt4_wrap_cpp(${name}_MOC_SOURCES ${${name}_MOCS}
    OPTIONS ${${name}_MOC_OPTS})

  set(${name}_ALL_SOURCES
    ${${name}_SOURCES}
    ${${name}_MOC_SOURCES}
    ${${name}_RES_SOURCES}
    ${${name}_UI_SOURCES}
    )

  if(NOT ${name}_ALL_SOURCES)
    message(FATAL_ERROR "No sources provided for target library '${name}'")
  endif(NOT ${name}_ALL_SOURCES)

  # =================== #
  # Compile the sources #
  # =================== #
  if(${name}_QTPLUGIN)
    set(TYPE MODULE)
  else(${name}_QTPLUGIN)
    set(TYPE SHARED)
  endif(${name}_QTPLUGIN)
  add_library(${name} ${TYPE} ${${name}_ALL_SOURCES})
  gostai_add_hint_dir(${CMAKE_CURRENT_BINARY_DIR})

  if(NOT ${name}_CPPFLAGS)
    set(${name}_CPPFLAGS "")
  endif(NOT ${name}_CPPFLAGS)
  if(${name}_QTPLUGIN)
    list(APPEND ${name}_CPPFLAGS QT_PLUGIN QT_SHARED)
  endif(${name}_QTPLUGIN)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND ${name}_CPPFLAGS QT_NO_DEBUG)
  endif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_target_properties(${name} PROPERTIES
    COMPILE_DEFINITIONS "${${name}_CPPFLAGS}")

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
  configure_file(
    ${CMAKE_MODULE_PATH}/system-info.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/system-info.h
    ESCAPE_QUOTES
    @ONLY)

  # ============= #
  # Install files #
  # ============= #

  if(NOT ${name}_NO_INSTALL)
    if(${name}_QTPLUGIN)
      install(TARGETS ${name} DESTINATION ${PLUGINS_DIR})
    else(${name}_QTPLUGIN)
      install(TARGETS ${name} DESTINATION ${LIBRARIES_DIR})
    endif(${name}_QTPLUGIN)
  endif(NOT ${name}_NO_INSTALL)

  # ========================== #
  # Deploy dependent libraries #
  # ========================== #

  if(NOT ${name}_NO_INSTALL)
    dldep_install(${name})
  endif(NOT ${name}_NO_INSTALL)

endfunction(gostai_add_library name)


# Add a Gostai test executable target named test-_name_.
# Arguments:
#  SOURCES	- the list of sources of the executable
#  MOCS		- the list of sources of the executable that need to be moc'ed
#  UIS		- the list of UI files used by the executable
#  CPPFLAGS	- the list of CPPFLAGS to add
#  INCLUDE_DIRS - the list of include directories to search in.
#  LIBRARIES	- the list of libraries to link with.
#  RESOURCES	- the list of resources files.
function(gostai_add_test name)

  set(prefix "test-${name}")
  parse_arguments(
    "${prefix}"
    "SOURCES;MOCS;UIS;CPPFLAGS;INCLUDE_DIRS;LIBRARIES;RESOURCES"
    "NO_CONSOLE;INSTALL_NO_DEPS"
    ${ARGN})

  # Ensure test is enabled
  enable_testing()

  # =================== #
  # Include directories #
  # =================== #
  if(${prefix}_INCLUDE_DIRS)
    include_directories(${${prefix}_INCLUDE_DIRS})
  endif(${prefix}_INCLUDE_DIRS)

  # =================== #
  # Prepare the sources #
  # =================== #
  qt4_wrap_ui(${prefix}_UI_SOURCES ${${prefix}_UIS})
  qt4_add_resources(${prefix}_RES_SOURCES ${${prefix}_RESOURCES})

  set(${prefix}_MOC_OPTIONS "")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND ${prefix}_MOC_OPTIONS "-DQT_NO_DEBUG")
    list(APPEND ${prefix}_MOC_OPTIONS "-DNDEBUG")
  endif(CMAKE_BUILD_TYPE STREQUAL "Release")
  qt4_wrap_cpp(${prefix}_MOC_SOURCES ${${prefix}_MOCS}
    OPTIONS ${${prefix}_MOC_OPTIONS})

  set(${prefix}_ALL_SOURCES
    ${${prefix}_SOURCES}
    ${${prefix}_MOC_SOURCES}
    ${${prefix}_RES_SOURCES}
    ${${prefix}_UI_SOURCES}
    )

  if(NOT ${prefix}_ALL_SOURCES)
    message(FATAL_ERROR "No sources provided for test '${name}'")
  endif(NOT ${prefix}_ALL_SOURCES)

  add_executable(${prefix} ${${prefix}_ALL_SOURCES})

  if(${prefix}_CPPFLAGS)
    set_target_properties(${prefix} PROPERTIES
      COMPILE_DEFINITIONS "${${prefix}_CPPFLAGS}")
  endif(${prefix}_CPPFLAGS)

  # ===================== #
  # Prepare the libraries #
  # ===================== #
  if(WIN32)
    list(APPEND ${prefix}_LIBRARIES ${QT_QTMAIN_LIBRARY})
  endif(WIN32)

  if(${prefix}_LIBRARIES)
    target_link_libraries(${prefix} ${${prefix}_LIBRARIES})
  endif(${prefix}_LIBRARIES)

  # ================= #
  # Register the test #
  # ================= #

  add_test(${prefix} ${prefix})

  # ================== #
  # Add custom targets #
  # ================== #

  add_exec_target(${prefix})

endfunction(gostai_add_test)

endif(NOT GOSTAI_CMAKE_GUARD)

# LocalWords:  docbook
