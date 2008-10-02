# - Extension to the cmake 'if' built-in command.

if(NOT IF_CMAKE_GUARD)
  set(IF_CMAKE_GUARD TRUE)

  # Do the same job as 'test -L' on system supporting 'test' and return the
  # result in _out_. Otherwise, return always FALSE.
  function(if_is_symlink filepath out)

    find_program(test_executable test)
    if (test_executable)
      execute_process(
	COMMAND ${test_executable} -L ${filepath}
	RESULT_VARIABLE res
	OUTPUT_QUIET
	ERROR_QUIET
	)
      if(res STREQUAL 0)
	set(${out} TRUE PARENT_SCOPE)
      else(res STREQUAL 0)
	set(${out} FALSE PARENT_SCOPE)
      endif(res STREQUAL 0)
    else(test_executable)
      set(${out} FALSE PARENT_SCOPE)
    endif(test_executable)

  endfunction(if_is_symlink)

endif(NOT IF_CMAKE_GUARD)
