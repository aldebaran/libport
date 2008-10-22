# - Extension to the cmake 'file' built-in command.

if(NOT FILE_CMAKE_GUARD)
  set(FILE_CMAKE_GUARD TRUE)

  # Do the same job as 'readlink -f' on system supporting 'readlink' and
  # return the result in _out_. Otherwise always return _filepath_.
  function(file_readlink filepath out)

    find_program(readlink_executable readlink)
    if(readlink_executable)
      execute_process(
	COMMAND ${readlink_executable} -f ${filepath}
	OUTPUT_VARIABLE out_
	ERROR_QUIET
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)
      set(${out} ${out_} PARENT_SCOPE)
    else(readlink_executable)
      set(${out} ${filepath} PARENT_SCOPE)
    endif(readlink_executable)

  endfunction(file_readlink)

endif(NOT FILE_CMAKE_GUARD)
