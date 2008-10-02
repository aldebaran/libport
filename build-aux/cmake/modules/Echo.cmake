# - Shorthand for message(STATUS "blabla")

if(NOT COMMAND ECHO)

  macro(ECHO)
    message(STATUS ${ARGV})
  endmacro(ECHO)

endif(NOT COMMAND ECHO)
