# - Find the git executable.

if(NOT GIT_EXECUTABLE)

  find_program(
    GIT_EXECUTABLE
    git
    DOC "The path to the git program used."
    )
  if(NOT GIT_EXECUTABLE)
    message(SEND_ERROR "Cannot find git")
  endif(NOT GIT_EXECUTABLE)

endif(NOT GIT_EXECUTABLE)
