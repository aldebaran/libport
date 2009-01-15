# - A set of function dealing with the underlying git repository.

find_package(Git REQUIRED)

if(NOT GIT_CMAKE_GUARD)
  set(GIT_CMAKE_GUARD TRUE)

  if(NOT GIT_EXECUTABLE)
    message(FATAL_ERROR "We need git installed")
  endif(NOT GIT_EXECUTABLE)

  # Puts in _out_ the current branch name.
  function(git_current_branch out)

    execute_process(
      COMMAND ${GIT_EXECUTABLE} branch --no-color
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      TIMEOUT 15
      RESULT_VARIABLE res
      OUTPUT_VARIABLE gitout
      ERROR_VARIABLE err
      )

    if(res)
      message(FATAL_ERROR
	"failed to list git branches:\nstatus=${res}\nerror=${err}")
    else(res)
      string(REGEX REPLACE "^.*\\* ([^\n]*)\n.*$" "\\1" branch ${gitout})
    endif(res)

    set(${out} ${branch} PARENT_SCOPE)
  endfunction(git_current_branch)

  # Puts in _out_ the result of git describe on the given _treeish_
  function(git_describe treeish out)

    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe --long --abbrev=8 ${ARGN} ${treeish}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      TIMEOUT 30
      RESULT_VARIABLE res
      OUTPUT_VARIABLE gitout
      ERROR_VARIABLE err
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if(res)
      echo("failed to git describe '${treeish}':\nstatus=${res}\nerror=${err}")
      set(${out} NOTFOUND PARENT_SCOPE)
    else(res)
      set(${out} ${gitout} PARENT_SCOPE)
    endif(res)

  endfunction(git_describe)

endif(NOT GIT_CMAKE_GUARD)
