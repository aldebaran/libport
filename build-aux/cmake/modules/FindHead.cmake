# - Find the head program.
# Output variables:
#  HEAD_EXECUTABLE - the path to the head program
#

if(NOT HEAD_EXECUTABLE)

find_program(
  HEAD_EXECUTABLE
  head
  DOC "The path to the head program used."
  )
if(NOT HEAD_EXECUTABLE)
  message(SEND_ERROR "Cannot find head")
endif()

endif(NOT HEAD_EXECUTABLE)
