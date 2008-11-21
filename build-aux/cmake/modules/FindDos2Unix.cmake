# - Find dox2unix utility.

if(NOT DOS2UNIX_EXECUTABLE)
  find_program(DOS2UNIX_EXECUTABLE
    NAMES dos2unix
    DOC "The path to the dos2unix executable")
  if(NOT DOS2UNIX_EXECUTABLE)
    message(SEND_ERROR "Cannot find dos2unix.")
  else(NOT DOS2UNIX_EXECUTABLE)
    message(STATUS "Found dos2unix: ${DOS2UNIX_EXECUTABLE}")
  endif(NOT DOS2UNIX_EXECUTABLE)
endif(NOT DOS2UNIX_EXECUTABLE)
