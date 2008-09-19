# -helper macro to add a "doc" target with CMake build system.
# and configure doxy.config.in to doxy.config
#
# target "doc" allows building the documentation with doxygen/dot on WIN32 and Linux
# Creates .chm windows help file if MS HTML help workshop
# (available from http://msdn.microsoft.com/workshop/author/htmlhelp)
# is installed with its DLLs in PATH.
#
#
# Please note, that the tools, e.g.:
# doxygen, dot, latex, dvips, makeindex, gswin32, etc.
# must be in path.
#
# Note about Visual Studio Projects:
# MSVS has its own path environment which may differ from the shell.
# See "Menu Tools/Options/Projects/VC++ Directories" in VS 7.1
#
# author Jan Woetzel 2004-2006
# www.mip.informatik.uni-kiel.de/~jw

# Extension by despres@gostai.com 2008
#
# * Search for configuration file name in the current source directory. The used
#   name is in DOXYGEN_CONFIG_FILE. Default value is doxygen.config.in.
#   The '.in' extension is dropped for the output file name which is generated
#   in the current binary directory.
#
# * Set DOXYGEN_HAVE_DOT to YES/NO whether the DOXYGEN_DOT_EXECUTABLE exists.

# TODO: handle dependency in order to build the documentation only if needed.
# TODO: add generated files to the clean target.

find_package(Doxygen)

if(DOXYGEN_FOUND)

  # Doxygen configuration file.
  if(NOT DOXYGEN_CONFIG_FILE)
    set(DOXYGEN_CONFIG_FILE "doxygen.config.in")
  endif(NOT DOXYGEN_CONFIG_FILE)

  string(REGEX REPLACE ".in$" ""
    DOXYGEN_OUT_CONFIG_FILE ${DOXYGEN_CONFIG_FILE})

  # Generate doxygen mainpage
  set(DOXYGEN_GEN_MAINPAGE "${CMAKE_MODULE_PATH}/gen_doxygen_mainpage")
  set(DOXYGEN_MAINPAGE "${CMAKE_CURRENT_BINARY_DIR}/doxygen-mainpage")
  add_custom_target(doxygen_mainpage
    ${DOXYGEN_GEN_MAINPAGE} ${DOXYGEN_MAINPAGE} ${CMAKE_SOURCE_DIR}
    COMMENT "Generate the doxygen mainpage to ${DOXYGEN_MAINPAGE}"
    VERBATIM)
  # Add some file to clean.
  set_directory_properties(PROPERTIES
    ADDITIONAL_MAKE_CLEAN_FILES "doxygen.log;${DOXYGEN_MAINPAGE}")

  # Set DOXYGEN_HAVE_DOT
  if(DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_HAVE_DOT "YES")
  else(DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_HAVE_DOT "NO")
    message(STATUS "warning: dot command not found '${DOXYGEN_DOT_EXECUTABLE}'")
  endif(DOXYGEN_DOT_EXECUTABLE)

  # click+jump in Emacs and Visual Studio (for DOXYGEN_OUT_CONFIG_FILE) (jw)
  if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    set(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
  else(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    set(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
  endif(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")

  # we need latex for doxygen because of the formulas
  find_package(LATEX)
  if(NOT LATEX_COMPILER)
    message(STATUS "latex command LATEX_COMPILER not found but usually "
      "required. You will probably get warnings and user inetraction on "
      "doxy run.")
  endif(NOT LATEX_COMPILER)
  if(NOT MAKEINDEX_COMPILER)
    message(STATUS "makeindex command MAKEINDEX_COMPILER not found but "
      "usually required.")
  endif(NOT MAKEINDEX_COMPILER)
  if(NOT DVIPS_CONVERTER)
    message(STATUS "dvips command DVIPS_CONVERTER not found but usually "
      "required.")
  endif(NOT DVIPS_CONVERTER)

  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE}")
    message(STATUS "configured "
      "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} "
      "--> ${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE}
      ${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}
      @ONLY )
    # use (configured) DOXYGEN_OUT_CONFIG_FILE from (out of place) BUILD tree:
    set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
  else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE}")
    # use static hand-edited DOXYGEN_OUT_CONFIG_FILE from SOURCE tree:
    set(DOXY_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
      message(STATUS "WARNING: using existing "
	"${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUT_CONFIG_FILE} "
	"instead of configuring from "
	"${DOXYGEN_CONFIG_FILE} file.")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
      if(EXISTS "${CMAKE_MODULE_PATH}/${DOXYGEN_CONFIG_FILE}")
        # using template DOXYGEN_CONFIG_FILE
        message(STATUS "configured "
	  "${CMAKE_MODULE_PATH}/${DOXYGEN_CONFIG_FILE} "
	  "--> ${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
        configure_file(${CMAKE_MODULE_PATH}/${DOXYGEN_CONFIG_FILE}
          ${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}
          @ONLY )
        set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
      else(EXISTS "${CMAKE_MODULE_PATH}/${DOXYGEN_CONFIG_FILE}")
        # failed completely...
        message(SEND_ERROR "Please create "
	  "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} "
	  "(or ${DOXYGEN_OUT_CONFIG_FILE} as fallback)")
      endif(EXISTS "${CMAKE_MODULE_PATH}/${DOXYGEN_CONFIG_FILE}")

    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_OUT_CONFIG_FILE}")
  endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE}")

  add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})
  add_dependencies(doxygen doxygen_mainpage)

  # create a windows help .chm file using hhc.exe
  # HTMLHelp DLL must be in path!
  # fallback: use hhw.exe interactively
  if(WIN32)
    find_package(HTMLHelp)
    if(HTML_HELP_COMPILER)
      set(TMP "${CMAKE_CURRENT_BINARY_DIR}\\Doc\\html\\index.hhp")
      string(REGEX REPLACE "[/]" "\\\\" HHP_FILE ${TMP} )
      # MESSAGE(SEND_ERROR "DBG  HHP_FILE=${HHP_FILE}")
      add_custom_target(winhelp ${HTML_HELP_COMPILER} ${HHP_FILE})
      add_dependencies(winhelp doc)

      if(NOT TARGET_DOC_SKIP_INSTALL)
      # install windows help?
      # determine useful name for output file
      # should be project and version unique to allow installing
      # multiple projects into one global directory
      if(EXISTS "${PROJECT_BINARY_DIR}/Doc/html/index.chm")
        if(PROJECT_NAME)
          set(OUT "${PROJECT_NAME}")
        else(PROJECT_NAME)
          set(OUT "Documentation") # default
        endif(PROJECT_NAME)
        if(${PROJECT_NAME}_VERSION_MAJOR)
          set(OUT "${OUT}-${${PROJECT_NAME}_VERSION_MAJOR}")
          if(${PROJECT_NAME}_VERSION_MINOR)
            set(OUT  "${OUT}.${${PROJECT_NAME}_VERSION_MINOR}")
            if(${PROJECT_NAME}_VERSION_PATCH)
              set(OUT "${OUT}.${${PROJECT_NAME}_VERSION_PATCH}")
            endif(${PROJECT_NAME}_VERSION_PATCH)
          endif(${PROJECT_NAME}_VERSION_MINOR)
        endif(${PROJECT_NAME}_VERSION_MAJOR)
        # keep suffix
        set(OUT "${OUT}.chm")

        #MESSAGE("DBG ${PROJECT_BINARY_DIR}/Doc/html/index.chm \n${OUT}")
        # create target used by install and package commands
        install(FILES "${PROJECT_BINARY_DIR}/Doc/html/index.chm"
          DESTINATION "doc"
          RENAME "${OUT}"
        )
      endif(EXISTS "${PROJECT_BINARY_DIR}/Doc/html/index.chm")
      endif(NOT TARGET_DOC_SKIP_INSTALL)

    endif(HTML_HELP_COMPILER)
    # MESSAGE(SEND_ERROR "HTML_HELP_COMPILER=${HTML_HELP_COMPILER}")
  endif(WIN32)
endif(DOXYGEN_FOUND)
