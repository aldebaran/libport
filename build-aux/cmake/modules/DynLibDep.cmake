# Provide function computing the list of dependent dynamic libraries of a given
# binary. It can also schedule installation.

if(NOT DYN_LIB_DEP_CMAKE_GUARD)
  set(DYN_LIB_DEP_CMAKE_GUARD TRUE)

  include(Tools)
  include(String)

  # The list of excluded Unix libraries.
  set(DLDEP_UNIX_EXCLUSION_LIST
    "^libc.so"
    "^libdl.so"
    "^libpthread.*"     # Fedora 9
    )

  # The list of excluded Windows libraries.
  set(DLDEP_WIN32_EXCLUSION_LIST
    "^KERNEL32.dll"
    "^ntdll.dll"
    "^USER32.dll"
    "^GDI32.dll"
    "^ADVAPI32.dll"
    "^COMCTL32.dll"
    "^COMDLG32.DLL"
    "^IMM32.DLL"
    "^OLE32.dll"
    "^OLEAUT32.DLL"
    "^RPCRT4.dll"
    "^SHELL32.dll"
    "^SHLWAPI.dll"
    "^WINMM.DLL"
    "^WINSPOOL.DRV"
    )

  set(DLDEP_VERBOSE TRUE)

  # Display message _msg_ if DLDEP_VERBOSE is true.
  function(dldep_info msg)
    if(DLDEP_VERBOSE)
      echo("dldep: info: ${msg}")
    endif(DLDEP_VERBOSE)
  endfunction(dldep_info)

  # Display message _msg_ and stops.
  function(dldep_error msg)
    message(FATAL_ERROR "dldep: error: ${msg}")
  endfunction(dldep_error)

  # Puts respectively the name and the path of the dependent libraries of the
  # binary _binary_ in _list_names_ and _lib_dirs_.
  #
  # It uses 'ldd' to gather the list of dependencies.
  function(dldep_ldd binary lib_names lib_dirs)

    set(tools ldd)
    find_program(tool ${tools})
    if(tool)
      dldep_info("use '${tool}' to list dependent dynamic libraries.")
    else(tool)
      dldep_error("cannot find an executable called '${tool}'")
    endif(tool)

    execute_process(
      COMMAND ${tool} ${binary}
      OUTPUT_VARIABLE out
      ERROR_VARIABLE err
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if(err)
      dldep_info("${tool} prints some error: '${err}'")
    endif(err)

    set(_lib_names "")
    set(_lib_dirs "")

    string(REGEX REPLACE "\n" ";" lines ${out})
    set(regex "\t+([^ ]+) +=> +([^ ]+) +")
    foreach(line ${lines})
      if(line MATCHES ${regex})

	set(lib_name ${CMAKE_MATCH_1})
	set(lib_path ${CMAKE_MATCH_2})

	list(APPEND _lib_names ${lib_name})

	if(${lib_path} STREQUAL "not found")
	  dldep_info("cannot find '${lib_name}'")
	  list(APPEND _lib_dirs "")
	else(${lib_path} STREQUAL "not found")
	  get_filename_component(lib_dir ${lib_path} PATH)
	  list(APPEND _lib_dirs ${lib_dir})
	endif(${lib_path} STREQUAL "not found")

      else(line MATCHES ${regex})
	dldep_info("cannot parse '${line}'")
      endif(line MATCHES ${regex})
    endforeach(line)

    set(${lib_names} ${_lib_names} PARENT_SCOPE)
    set(${lib_dirs} ${_lib_dirs} PARENT_SCOPE)

  endfunction(dldep_ldd)

  # Puts respectively the name and the path of the dependent libraries of the
  # binary _binary_ in _list_names_ and _lib_dirs_.
  #
  # It uses 'cygcheck.exe' to gather the list of dependencies.
  function(dldep_cygcheck binary lib_names lib_dirs)

    find_package(Cygcheck REQUIRED)
    if(CYGCHECK_EXECUTABLE)
      dldep_info("use '${CYGCHECK_EXECUTABLE}' to list dependent dynamic "
	"libraries.")
    else(CYGCHECK_EXECUTABLE)
      dldep_error("cannot find an executable called '${CYGCHECK_EXECUTABLE}'")
    endif(CYGCHECK_EXECUTABLE)

    execute_process(
      COMMAND ${CYGCHECK_EXECUTABLE} ${binary}
      OUTPUT_VARIABLE out
      ERROR_VARIABLE err
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if(err)
      dldep_info("${CYGCHECK_EXECUTABLE} prints some error: '${error}'")
    endif(err)

    set(_lib_names "")
    set(_lib_dirs "")

    string(REPLACE "\\" "/" lines ${out})
    string(REPLACE "\n" ";" lines ${lines})
    set(found_regexp "^   *([^ ]+)/([^ ]+)$")
    set(not_found_regexp "^Error: could not find ([^ ]+)$")
    foreach(line ${lines})
      # We found one library
      if(line MATCHES ${found_regexp})

	list(APPEND _lib_names ${CMAKE_MATCH_2})
	list(APPEND _lib_dirs ${CMAKE_MATCH_1})

      # or one library has not been found.
      elseif(line MATCHES ${not_found_regexp})

	set(lib_name ${CMAKE_MATCH_1})
	get_filename_component(lib_name_we ${lib_name} NAME_WE)

	# Try to search in programs.
	find_program(${lib_name_we}_prog ${lib_name})
	if(${lib_name_we}_prog)
	  get_filename_component(lib_dir ${${lib_name_we}_prog} PATH)
	  list(APPEND _lib_names ${lib_name})
	  list(APPEND _lib_dirs ${lib_dir})
	else(${lib_name_we}_prog)
	  # Try to search in libraries.
	  find_library(${lib_name_we}_lib ${lib_name})
	  if(${lib_name_we}_lib)
	    get_filename_component(lib_dir ${${lib_name_we}_lib} PATH)
	    list(APPEND _lib_names ${lib_name})
	    list(APPEND _lib_dirs ${lib_dir})
	  else(${lib_name_we}_lib)
	    # Failed to find the library.
	    dldep_info("cannot find '${lib_name}'")
	  endif(${lib_name_we}_lib)
	endif(${lib_name_we}_prog)

      # or we cannot parse the line.
      else(line MATCHES ${found_regexp})
	dldep_info("cannot parse '${line}'")
      endif(line MATCHES ${found_regexp})
    endforeach(line)

    set(${lib_names} ${_lib_names} PARENT_SCOPE)
    set(${lib_dirs} ${_lib_dirs} PARENT_SCOPE)

  endfunction(dldep_cygcheck)

  # Filter library names from _lib_names_ that match one of the regexp from
  # _exclusions_. Corresponding library paths from _lib_dirs_ are stored in
  # _result_ with library name appended.
  function(dldep_filter lib_names lib_dirs exclusions result)

    set(_result "")

    list(LENGTH lib_names lib_count)

    if(NOT lib_count EQUAL 0)

      math(EXPR lib_count "${lib_count} - 1")
      foreach(i RANGE ${lib_count})

	list(GET lib_names ${i} lib_name)
	list(GET lib_dirs ${i} lib_dir)

	string_list_match("${exclusions}" ${lib_name} index)

	if(index EQUAL -1)
	  list(APPEND _result ${lib_dir}/${lib_name})
	else(index EQUAL -1)
	  dldep_info("exclude '${lib_name}'")
	endif(index EQUAL -1)

      endforeach(i)

    endif(NOT lib_count EQUAL 0)

    set(${result} ${_result} PARENT_SCOPE)

  endfunction(dldep_filter)

  # Puts in _outlist_ the dependent dynamic libraries of _binary_. It is
  # supposed to be portable. The libraries are given with there full absolute
  # path name.
  function(dldep binary outlist)

    if(UNIX)
      dldep_ldd(${binary} lib_names lib_dirs)
      dldep_filter(
	"${lib_names}"
	"${lib_dirs}"
	"${DLDEP_UNIX_EXCLUSION_LIST}"
	outlist_)
    else(UNIX)
      if(WIN32)
	dldep_cygcheck(${binary} lib_names lib_dirs)
	dldep_filter(
	  "${lib_names}"
	  "${lib_dirs}"
	  "${DLDEP_WIN32_EXCLUSION_LIST}"
	  outlist_)
      endif(WIN32)
    endif(UNIX)

    set(${outlist} ${outlist_} PARENT_SCOPE)

  endfunction(dldep)

  # Schedule for installation all dependent dynamic libraries of _binary_.
  # NOTE: _binary_ might be a target name.
  function(dldep_install binary)

    if(CMAKE_CROSSCOMPILING)
      echo("Cannot list dependent dynamic libraries when cross-compiling.")
      return()
    endif(CMAKE_CROSSCOMPILING)

    include(Dirs)
    if(TARGET ${binary})
      get_target_location(${binary} binary_loc)
      set(binary_name ${binary})
      get_filename_component(binary_path ${binary_loc} PATH)
    else(TARGET ${binary})
      set(binary_loc ${binary})
      get_filename_component(binary_name ${binary} NAME)
      set(binary_path ${CMAKE_CURRENT_BINARY_DIR})
    endif(TARGET ${binary})
    configure_file(
      ${CMAKE_MODULE_PATH}/install-dyn-lib-dep.cmake.in
      ${binary_path}/install-dyn-lib-dep.cmake
      @ONLY)
    install(SCRIPT ${binary_path}/install-dyn-lib-dep.cmake)

  endfunction(dldep_install)

endif(NOT DYN_LIB_DEP_CMAKE_GUARD)
