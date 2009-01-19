# - Find the cygcheck executable

if(WIN32)
  find_program(CYGCHECK_EXECUTABLE cygcheck)
  if(CYGCHECK_EXECUTABLE)
    message(STATUS "Found cygcheck at '${CYGCHECK_EXECUTABLE}'")
  else(CYGCHECK_EXECUTABLE)
    message(SEND_ERROR "Cannot find cygcheck.")
  endif(CYGCHECK_EXECUTABLE)
endif(WIN32)
