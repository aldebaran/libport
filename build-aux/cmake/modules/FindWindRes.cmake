# - Find windres.exe

if(NOT WINDRES_EXECUTABLE)

  find_program(WINDRES_EXECUTABLE NAMES windres)
  if(NOT WINDRES_EXECUTABLE)
    message(SEND_ERROR "Cannot find 'windres' executable.")
  endif(NOT WINDRES_EXECUTABLE)

endif(NOT WINDRES_EXECUTABLE)
