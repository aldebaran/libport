# - Useful set of functions

if(NOT TOOLS_CMAKE_GUARD)
  set(TOOLS_CMAKE_GUARD TRUE)

  # Display the value of the variables given as argument.
  macro(PRINT_VAR)
    foreach(i ${ARGV})
      message(STATUS "${i}='${${i}}'")
    endforeach(i)
  endmacro(PRINT_VAR)

  # Print all cmake vars.
  function(PRINT_CMAKE_VARS)
    get_cmake_property(vars VARIABLES)
    foreach(var in ${vars})
      print_var(${var})
    endforeach(var)
  endfunction(PRINT_CMAKE_VARS)

  # Shorthand for message(STATUS "blabla")
  macro(ECHO)
    message(STATUS ${ARGV})
  endmacro(ECHO)

  # Find the hostname of machine running cmake.
  # outvar: the name of the variable where to store the result.
  macro(HOSTNAME outvar)
    EXECUTE_PROCESS(
      COMMAND hostname
      OUTPUT_VARIABLE ${outvar}
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
  endmacro(HOSTNAME)

  # Escape double quote string
  macro(ESCAPE_DOUBLE_QUOTE out in)
    string(REGEX REPLACE "([\"\\])" "\\\\\\1" out ${in})
  endmacro(ESCAPE_DOUBLE_QUOTE)

  # Set the given variable with the given value if the variable is not already
  # set.
  macro(SET_IF_NOT_SET var)
    IF(NOT DEFINED "${var}")
      SET(${var} ${ARGN})
    ENDIF(NOT DEFINED "${var}")
  endmacro(SET_IF_NOT_SET var val)

  # Set the variable with the given path if it exists.
  function(set_if_exists var path)
    if(EXISTS ${path})
      set(${var} ${path} PARENT_SCOPE)
    endif(EXISTS ${path})
  endfunction(set_if_exists)

  # Check whether the given variables exists. Send an error message if it
  # doesn't.
  macro(CHECK_CMAKE_VARS_EXISTS)
    foreach(i ${ARGV})
      if(NOT DEFINED ${i})
	message(SEND_ERROR "Variables '${i}' must be defined.")
      endif()
    endforeach(i)
  endmacro(CHECK_CMAKE_VARS_EXISTS)

  ## Create a symlink of OLD in NEW.
  ## For Windows, copy instead of symlinking.
  function(SYMLINK OLD NEW)
    ## Prepare
    ## -------
    ## cmake create_symlink does not support syntax "a la"
    ## `ln [-fs] source_file ... target_dir',
    ## so we emulate it.

    if(IS_DIRECTORY ${OLD})
      string(REGEX REPLACE "/+$" "" OLD ${OLD})
      set(OLD "${OLD}/")
    endif(IS_DIRECTORY ${OLD})
    if(NEW MATCHES "/$")
      if(IS_DIRECTORY ${OLD})
        string(REGEX MATCH "/[^/]+/$" FILENAME ${OLD})
      else(IS_DIRECTORY ${OLD})
        string(REGEX MATCH "/[^/]+$" FILENAME ${OLD})
      endif(IS_DIRECTORY ${OLD})
      if(FILENAME STREQUAL "")
        message(SEND_ERROR "Failed to extract filename from ${OLD}")
      endif(FILENAME STREQUAL "")
      string(REPLACE "/" "" FILENAME ${FILENAME})
      set(NEW "${NEW}/${FILENAME}")
    endif(NEW MATCHES "/$")
    if(EXISTS ${NEW})
      message("-- Skipping symlink `${OLD}' to `${NEW}': ${NEW} already exists")
      return()
    endif(EXISTS ${NEW})
    message("-- *** Symlinking `${OLD}' to `${NEW}'")

    ## Do
    if(UNIX)
      execute_process(COMMAND ${CMAKE_COMMAND}
        -E create_symlink
        "${OLD}" "${NEW}"
        )
    else(UNIX)
      if(IS_DIRECTORY ${OLD})
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy_directory
          "${OLD}" "${NEW}"
          )
      else(IS_DIRECTORY ${OLD})
        execute_process(COMMAND ${CMAKE_COMMAND}
          -E copy
          "${OLD}" "${NEW}"
          )
      endif(IS_DIRECTORY ${OLD})
    endif(UNIX)

    ## Check
    if(NOT EXISTS ${NEW})
      message(SEND_ERROR "!!! Symlink creating failed!")
    endif(NOT EXISTS ${NEW})

  endfunction(SYMLINK)

  # This macro can be found on the wiki page of CMake.
  # Example given :
  # --------------
  #
  # SET(arguments
  #   hello OPTION3 world
  #   LIST3 foo bar
  #   OPTION2
  #   LIST1 fuz baz
  #   )
  # parse_arguments(ARG
  #   "LIST1;LIST2;LIST3"
  #   "OPTION1;OPTION2;OPTION3"
  #   ${arguments})
  #
  # * results
  #     > ARG_DEFAULT_ARGS: hello;world
  #     > ARG_LIST1: fuz;baz
  #     > ARG_LIST2:
  #     > ARG_LIST3: foo;bar
  #     > ARG_OPTION1: FALSE
  #     > ARG_OPTION2: TRUE
  #     > ARG_OPTION3: TRUE
  macro(parse_arguments prefix arg_names option_names)

    # Init. all section variables
    set(DEFAULT_ARGS)
    foreach(arg_name ${arg_names})
      set(${prefix}_${arg_name})
    endforeach(arg_name)

    foreach(option ${option_names})
      set(${prefix}_${option} FALSE)
    endforeach(option)

    set(current_arg_name DEFAULT_ARGS)
    set(current_arg_list)

    # Browse all the arguments
    foreach(arg ${ARGN})

      # Look for name of sections
      set(larg_names ${arg_names})
      list(FIND larg_names "${arg}" is_arg_name)

      if(is_arg_name GREATER -1)

	set(${prefix}_${current_arg_name} ${current_arg_list})
	set(current_arg_name ${arg})
	set(current_arg_list)

      else(is_arg_name GREATER -1)

	set(loption_names ${option_names})
	list(FIND loption_names "${arg}" is_option)

	# Options
	if(is_option GREATER -1)
	  set(${prefix}_${arg} TRUE)
	else(is_option GREATER -1)
	  set(current_arg_list ${current_arg_list} ${arg})
	endif(is_option GREATER -1)

      endif(is_arg_name GREATER -1)
    endforeach(arg)

    set(${prefix}_${current_arg_name} ${current_arg_list})

  endmacro(parse_arguments)

  # Wrapper around 'configure_file'
  # _in_: the input file
  # _out_ the output file
  # OPTIONS: the list of options to give to 'configure_file'.
  # VARIABLES: the list of variables to convert. The result is stored in a
  #            variable suffixed by '_in'. Use this variable in your template.
  function(configure_file_with_native_paths in out)

    parse_arguments(
      ${in}
      "OPTIONS;VARIABLES;"
      ""
      ${ARGN})

    foreach(var ${${in}_VARIABLES})
      string(REPLACE "/" "\\" ${var}_in "${${var}}")
    endforeach(var)

    configure_file(${in} ${out} ${${in}_OPTIONS})
  endfunction(configure_file_with_native_paths)

  # Escape XML code in _in_. Store the result in _in_.
  function(xml_escape in)
    set(variable ${${in}})
    string(REPLACE "<" "&lt;" variable "${variable}")
    string(REPLACE ">" "&gt;" variable "${variable}")
    string(REPLACE "\"" "&quot;" variable "${variable}")
    set(${in} "${variable}" PARENT_SCOPE)
  endfunction(xml_escape)

  function(configure_xml in out)
    configure_file(
      ${in}
      ${out}
      ESCAPE_QUOTES)
    # \" => &quot;
    file(READ
      ${out}
      XML_FILE)
    string(REPLACE "\\\"" "&quot;"
      XML_FILE
      ${XML_FILE})
    file(WRITE
      ${out}
      "${XML_FILE}")
  endfunction(configure_xml)

  # Convert all file path passed in argument to the Unix text format.
  function(dos2unix)
    find_package(Dos2Unix REQUIRED)
    if(DOS2UNIX_EXECUTABLE)
      execute_process(
	COMMAND ${DOS2UNIX_EXECUTABLE} ${ARGV}
	RESULT_VARIABLE dos2unix_status
	)
      if(NOT dos2unix_status EQUAL 0)
	message(SEND_ERROR "${DOS2UNIX_EXECUTABLE} failed on '${output_sh}'")
      endif(NOT dos2unix_status EQUAL 0)
    endif(DOS2UNIX_EXECUTABLE)
  endfunction(dos2unix)

endif(NOT TOOLS_CMAKE_GUARD)
