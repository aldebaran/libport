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
    endif(UNIX)

    set(${outlist} ${outlist_} PARENT_SCOPE)

  endfunction(dldep)

  # Schedule for installation all dependent dynamic libraries of _binary_.
  # NOTE: _binary_ might be a target name.
  function(dldep_install binary)

    include(Dirs)
    if(TARGET ${binary})
      get_target_property(binary_loc ${binary} LOCATION_${CMAKE_BUILD_TYPE})
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
