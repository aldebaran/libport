# - Find the env executable

find_program(ENV_EXECUTABLE env)
if(ENV_EXECUTABLE)
  message(STATUS "Found env at '${ENV_EXECUTABLE}'")
else(ENV_EXECUTABLE)
  message(SEND_ERROR "Cannot find env.")
endif(ENV_EXECUTABLE)
