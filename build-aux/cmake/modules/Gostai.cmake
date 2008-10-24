# A collection of macro used to create Gostai executable and libraries

if(NOT GOSTAI_CMAKE_GUARD)
set(GOSTAI_CMAKE_GUARD TRUE)

include(Tools)
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
      #FIXME: Make this macro a function to avoid cluttering the namespace
      # with the icon_path variable.
      string(REPLACE "/" "\\\\" icon_path "${${name}_ICON_PATH}")
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
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE FALSE)
  else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
  endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

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

  gen_lib_loader(${name})
  dldep_install(${name})

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
    "SOURCES;EXTRA;IMPORT_BUILD"
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
    set(${prefix}_MAIN_SOURCE ${name}.xml)
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
    configure_file(
      ${CMAKE_MODULE_PATH}/project-info.docbook.in
      ${CMAKE_CURRENT_BINARY_DIR}/project-info.xml
      ESCAPE_QUOTES)
    # \" => &quot;
    file(READ
      ${CMAKE_CURRENT_BINARY_DIR}/project-info.xml
      XML_FILE)
    string(REPLACE "\\\"" "&quot;"
      XML_FILE
      ${XML_FILE})
    file(WRITE
      ${CMAKE_CURRENT_BINARY_DIR}/project-info.xml
      "${XML_FILE}")
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

endif(NOT GOSTAI_CMAKE_GUARD)

# LocalWords:  docbook
